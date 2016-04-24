/**
 * Copyright (c) 2013 Appareo Systems, LLC.
 * 1810 NDSU Research Park Circle North
 * Fargo ND, 58102
 * All rights reserved.
 *
 * This is the confidential and proprietary information of Appareo Systems, LLC. You shall not
 * disclose such confidential information and shall use it only in accordance with the terms
 * of the license agreement you entered into with Appareo.
 *
 * File: CommandParser.c
 * Creator: Nick Butts
 * Date: Jan 1, 2015
 *
 * Copyright Version 1.0
 */

#include <stdlib.h>
#include "usbd_def.h"
#include "CommandParser.h"
#include "string.h"
#include "usbd_cdc_if.h"
#include "RTCC.h"
#include "AnalogVoltage.h"
#include "Thermistor.h"
#include "i2c.h"
#include "DataLogger.h"
#include "ee_printf.h"

#define LOCAL_STR_SIZE  20

#define RX_BUF_LENGTH   256
char _rxBuf[RX_BUF_LENGTH];
char _txBuf[RX_BUF_LENGTH];
uint16_t _rxBufIndex = 0;

static void printTimeDate();
static void setTimeDate();
static char * strtok_local(char * str, char delim, char * substr, uint8_t maxSubStrLen);
static uint32_t fromStrToBCD(char * str);
static char * fromBCDToStr(char * buf, uint8_t bufLength, uint32_t value);
static void printTemperature();
static void itoa_local(int32_t value, char * str, uint8_t strLen);
static void dumpStoredData();
static void readEEPROM();
static void writeEEPROM();

#define HELP_STR \
        "Battery Temperature Monitor " VERSION "\r" \
        "S - Set RTC S<year> <month> <day> <hour> <minute> <second>\r" \
        "G - Get RTC\r" \
        "D - Dump data\r" \
        "T - Get current temp\r" \
        "R - Read EEPROM R <address in hex> <size in bytes>\r" \
        "W - Write EEPROM W <address in hex> <value>"

static const char HelpStr[] = HELP_STR;
static const char HelloWorld[] = "Hello World\n";

/**
 * This function is called from the USB CDC to push data into the local buffer
 *
 * @param buf a pointer to the CDC buffer
 * @param len the number of bytes received through the USB CDC interface
 */
void CommandParser_pushData(uint8_t * buf, uint16_t len)
{
    uint16_t spaceRemaining = RX_BUF_LENGTH - _rxBufIndex;
    memccpy(&_rxBuf[_rxBufIndex], buf, spaceRemaining, len);
    _rxBufIndex +=  MIN(spaceRemaining, len);
    CDC_Transmit_FS(buf, len);
}

/**
 * This function handles parsing of any received data from the USB CDC interface
 */
void CommandParser_execute()
{
    for (uint8_t i = 0; i < _rxBufIndex; ++i)
    {
        if (_rxBuf[i] == '\r')
        {
            switch (_rxBuf[0])
            {
                case 'h':
                case 'H':
                    CDC_Transmit_FS((uint8_t*)HelpStr, strlen(HelpStr));
                    break;
                case 'G':
                    printTimeDate();
                    break;
                case 'S':
                    setTimeDate();
                    break;
                case 'D':
                    dumpStoredData();
                    break;
                case 'T':
                    AnalogVoltage_initiateConversion();
                    printTemperature();
                    break;
                case 'W':
                    writeEEPROM();
                    break;
                case 'R':
                    readEEPROM();
                    break;
                default:
                    break;
            }
            _rxBufIndex = 0;
        }
    }
}

/**
 * This function prints the time from the RTC
 */
static void printTimeDate()
{
    char unixTime[10];
    uint32_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    RTC_getTime(&year, &month, &day, &hour, &minute, &second);
    strcpy(_txBuf, "Current Date: 20");
    fromBCDToStr(&_txBuf[strlen(_txBuf)], 2, year);
    strcat(_txBuf, " ");
    fromBCDToStr(&_txBuf[strlen(_txBuf)], 2, month);
    strcat(_txBuf, " ");
    fromBCDToStr(&_txBuf[strlen(_txBuf)], 2, day);
    strcat(_txBuf, "\r");
    strcat(_txBuf, "Current Time: ");
    fromBCDToStr(&_txBuf[strlen(_txBuf)], 2, hour);
    strcat(_txBuf, " ");
    fromBCDToStr(&_txBuf[strlen(_txBuf)], 2, minute);
    strcat(_txBuf, " ");
    fromBCDToStr(&_txBuf[strlen(_txBuf)], 2, second);
    strcat(_txBuf, "\r");
    int eepLen = ee_printf(unixTime, "%d", RTC_getCurrentTimeAs32bits());
    unixTime[eepLen] = '\0';
    strcat(_txBuf, unixTime);
    CDC_Transmit_FS((uint8_t*)_txBuf, strlen(_txBuf));
}

/**
 * This function assumes the data is stored in txBuf and assumes the first character is the S
 * character
 */
static void setTimeDate()
{
    char str[LOCAL_STR_SIZE];
    char * strPtr;
    strPtr = strtok_local(&_rxBuf[1], ' ', str, LOCAL_STR_SIZE);
    uint32_t year = fromStrToBCD(str);
    strPtr = strtok_local(strPtr, ' ', str, LOCAL_STR_SIZE);
    uint8_t month = fromStrToBCD(str);
    strPtr = strtok_local(strPtr, ' ', str, LOCAL_STR_SIZE);
    uint8_t day = fromStrToBCD(str);
    strPtr = strtok_local(strPtr, ' ', str, LOCAL_STR_SIZE);
    uint8_t hour = fromStrToBCD(str);
    strPtr = strtok_local(strPtr, ' ', str, LOCAL_STR_SIZE);
    uint8_t minute = fromStrToBCD(str);
    strPtr = strtok_local(strPtr, ' ', str, LOCAL_STR_SIZE);
    uint8_t second = fromStrToBCD(str);
    RTC_setTime(year, month, day, hour, minute, second);
}

/**
 * This is my version of the strtok function. This one behaves differently. It copies
 * the found string into a substring buffer and returns a pointer to one past the delimeter
 *
 * @param str a pointer to the buffer to tokenize
 * @param delim the character to use as the delimeter
 * @param substr a pointer to the substring buffer to copy the data into
 * @param maxSubStrLen the maximum length of the substring
 * @return char* a pointer one past the delimeter
 */
static char * strtok_local(char * str, char delim, char * substr, uint8_t maxSubStrLen)
{
    /*
     * This function will copy all of the characters from the starting position to the first
     * delimiter.
     */
    uint8_t len = strlen(str);
    for (uint8_t i = 0; i < len; ++i)
    {
        if ((str[i] == delim) && (i < maxSubStrLen))
        {
            uint8_t copyLen = MIN(i, maxSubStrLen - 1);
            memccpy(substr, str, maxSubStrLen - 1, i);
            substr[copyLen] = '\0';
            return &str[i+1];
        }
    }
    return NULL;
}

/**
 * This function takes a string and converts it into a BCD number. This function converts
 * the string into a uint32_t, so the string can not be longer than 8 characters.
 *
 * @param str pointer to a string to convert, this buffer should be no longer than 8 characters
 * @return uint32_t a BCD version of the string
 */
static uint32_t fromStrToBCD(char * str)
{
    uint32_t value = 0;
    uint8_t len = strlen(str);
    for (uint8_t i = 0; i < len; ++i)
    {
        value <<= 4;
        value |= str[i] - '0';
    }
    return value;
}

/**
 * This function takes a BCD value and converts it into a string. The bufLength is really how
 * many digits of the BCD value you want to convert to a string.
 *
 * @param buf a pointer a buffer that holds the BCD value
 * @param bufLength the number of nibbles to convert
 * @param value the BCD value to convert
 * @return char * a pointer to the buffer
 */
static char * fromBCDToStr(char * buf, uint8_t bufLength, uint32_t value)
{
    for (int8_t i = bufLength - 1; i >= 0; --i)
    {
        buf[i] = (value & 0xF) + '0';
        value >>= 4;
    }
    buf[bufLength] = '\0';
    return buf;
}

/**
 * This function gets the current temperature
 *
 */
static void printTemperature()
{
    while (AnalogVoltage_isCaptureComplete() == 0) {};
    int32_t temp = Thermistor_getTemperatureInTenthsOfDegC(AnalogVoltage_getMillivoltsFromADC(THERMISTOR), AnalogVoltage_getVddaIn_MV());
    strcpy(_txBuf, "Current Temp: ");
    char localStr[10];
    itoa_local(temp, localStr, 10);
    strcat(_txBuf, localStr);
    strcat(_txBuf, " degC (tenths)");
    CDC_Transmit_FS((uint8_t*)_txBuf, strlen(_txBuf));
}

static void itoa_local(int32_t value, char * str, uint8_t strLen)
{
    uint8_t valueLen = 0;
    char buf[8];
    bool isNeg = false;
    if (value < 0)
    {
        isNeg = true;
        value = -value;
    }
    while (value && valueLen < 8)
    {
        buf[valueLen++] = value % 10;
        value /= 10;
    }

    // Reverse the string
    for (uint8_t i = 0; (i < valueLen) && (i < strLen); ++i)
    {
        str[i] = buf[valueLen - i - 1] + '0';
    }

    if (valueLen == 0)
    {
        str[valueLen] = 0;
    }
    str[valueLen] = '\0';
}

static void dumpStoredData()
{
    char str[LOCAL_STR_SIZE];
    char * strPtr;
    uint32_t recordTime;
    int32_t  temp;
    int32_t readLen;

    strPtr = strtok_local(&_rxBuf[1], ' ', str, LOCAL_STR_SIZE);
    readLen = strtol(strPtr, NULL, 10);

    for (uint16_t i = 0; i < readLen; ++i)
    {
        DataLogger_readTemp(i, &recordTime, &temp);
        int len = ee_printf(_txBuf, "%u,%d\r", recordTime, temp);
        _txBuf[len] = '\0';
        CDC_Transmit_FS((uint8_t*)_txBuf, strlen(_txBuf));
        HAL_Delay(10);
    }
}

static void readEEPROM()
{
    char str[LOCAL_STR_SIZE];
    uint8_t data[16];
    char * strPtr;
    strPtr = strtok_local(&_rxBuf[1], ' ', str, LOCAL_STR_SIZE);
    uint32_t address;
    address = strtol(strPtr, NULL, 16);

    strPtr = strtok_local(strPtr, ' ', str, LOCAL_STR_SIZE);
    uint32_t len;
    len = strtol(strPtr, NULL, 10);

    while (len)
    {
        _txBuf[0] = '\0';
        uint32_t readLen = len > 16 ? 16 : len;
        i2c_readData(EEPROM_ADDRESS, address, data, readLen);
        for (int i = 0; i < readLen; ++i)
        {
            int strLen = ee_printf(str, "%02X ", data[i]);
            str[strLen] = '\0';
            strcat(_txBuf, str);
        }
        strcat(_txBuf, "\r");
        CDC_Transmit_FS((uint8_t*)_txBuf, strlen(_txBuf));
        HAL_Delay(10);
        address += 16;
        len -= readLen;
    }
}

static void writeEEPROM()
{
    uint8_t buffer[8];
    uint8_t counter = 0;
    strcpy(_txBuf, "WriteEEPROM");
    CDC_Transmit_FS((uint8_t*)_txBuf, strlen(_txBuf));
    for (int i = 0; i < 10; ++i)
    {
        for (int j=0; j < 8; ++j)
        {
            buffer[j] = counter;
            i2c_writeByte(EEPROM_ADDRESS, (i*8)+j, counter);
            HAL_Delay(10);
            counter++;
        }
        HAL_Delay(10);
    }
}