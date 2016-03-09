#include <stdint.h>
#include "DataLogger.h"
#include "i2c.h"

static uint32_t CurrentLocation;
static const uint8_t EEPROMAddress = EEPROM_ADDRESS;
static const uint8_t RecordLength = 8;

static uint32_t bytesToWord(uint8_t * data);


void DataLogger_init()
{
    CurrentLocation = 0;
}

void DataLogger_storeTemp(uint32_t currentTime, int32_t temp)
{
    uint8_t buf[8];
    buf[0] = (currentTime      ) & 0xFF;
    buf[1] = (currentTime >>  8) & 0xFF;
    buf[2] = (currentTime >> 16) & 0xFF;
    buf[3] = (currentTime >> 24) & 0xFF;

    buf[4] = (temp      ) & 0xFF;
    buf[5] = (temp >>  8) & 0xFF;
    buf[6] = (temp >> 16) & 0xFF;
    buf[7] = (temp >> 24) & 0xFF;
    for (int i = 0; i < 8; ++i)
    {
        i2c_writeByte(EEPROMAddress, CurrentLocation, buf[i]);
        HAL_Delay(10);
        CurrentLocation++;
    }
}

void DataLogger_readTemp(uint32_t location, uint32_t * recordTime, int32_t *temp)
{
    uint8_t eepData[RecordLength];
    uint32_t address = location * RecordLength;
    uint8_t readLen = i2c_readData(EEPROMAddress, address, eepData, RecordLength);
    if (readLen == RecordLength)
    {
        *recordTime = bytesToWord(eepData);
        *temp       = bytesToWord(eepData + 4);
    }
}

static uint32_t bytesToWord(uint8_t * data)
{
    uint32_t value;
    value  = *(data++);
    value |= *(data++) << 8;
    value |= *(data++) << 16;
    value |= *(data++) << 24;
    return value;
}

