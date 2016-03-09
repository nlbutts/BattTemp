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
 * File: RTCC.cpp
 * Creator: Nick Butts
 * Date: Dec 29, 2014
 *
 * Copyright Version 1.0
 */

#include <stdint.h>
#include "RTCC.h"
#include "GPIO.h"
#include "time.h"

RTC_HandleTypeDef _hrtc;
bool _wakeup = false;

void RTC_IRQHandler()
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&_hrtc);
    // We've woken up from the RTC interrupt.
    GPIO_LEDToggle(3);
    _wakeup = true;
}

void RTC_initialize()
{
    __HAL_RCC_RTC_ENABLE();

      /**Initialize RTC and set the Time and Date
      */
    _hrtc.Instance = RTC;
    _hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    _hrtc.Init.AsynchPrediv = 127;
    _hrtc.Init.SynchPrediv = 255;
    _hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    _hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_PC13;
    _hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    _hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    HAL_RTC_Init(&_hrtc);
}

bool RTC_getWakeupFlag()
{
    return _wakeup;
}

void RTC_clearWakeupFlag()
{
    _wakeup = false;
}

void RTC_setTime(uint32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t seconds)
{
    __HAL_RCC_RTC_ENABLE();

    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

      /**Initialize RTC and set the Time and Date
      */
    _hrtc.Instance = RTC;
    _hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    _hrtc.Init.AsynchPrediv = 127;
    _hrtc.Init.SynchPrediv = 255;
    _hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    _hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_PC13;
    _hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    _hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    HAL_RTC_Init(&_hrtc);

    sTime.Hours         = hour;
    sTime.Minutes       = minute;
    sTime.Seconds       = seconds;
    sTime.SubSeconds    = 0;
    sTime.TimeFormat = RTC_HOURFORMAT12_AM;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&_hrtc, &sTime, FORMAT_BCD);

    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = month;
    sDate.Date = day;
    sDate.Year = year;
    HAL_RTC_SetDate(&_hrtc, &sDate, FORMAT_BCD);
}

void RTC_getTime(uint32_t * year, uint8_t * month, uint8_t * day, uint8_t * hour, uint8_t * minute, uint8_t * seconds)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(&_hrtc, &sTime, FORMAT_BCD);
    HAL_RTC_GetDate(&_hrtc, &sDate, FORMAT_BCD);
    *year       = sDate.Year;
    *month      = sDate.Month;
    *day        = sDate.Date;
    *hour       = sTime.Hours;
    *minute     = sTime.Minutes;
    *seconds    = sTime.Seconds;
}

void RTC_setWakeUpTimer(uint32_t seconds)
{
    /*
     * To get the Wakeup timer working I must
     * 1. Configure and enable the EXTI line(s) corresponding to the RTC event(s) in interrupt
     * mode and select the rising edge sensitivity.
     * 2.  Configure and enable the RTC IRQ channel in the NVIC.
     * 3.  Configure the RTC to generate RTC interrupt(s).
     */
    // The LSI varies widely, but should be around 38000 Hz
    //uint32_t wakeUpTime = (seconds * 38000) / 16;
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&_hrtc, RTC_FLAG_WUTF);
    /* Clear the EXTI's line Flag for RTC WakeUpTimer */
    __HAL_RTC_EXTI_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

    HAL_RTCEx_SetWakeUpTimer_IT(&_hrtc, seconds, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&_hrtc, RTC_FLAG_WUTF);
    /* Clear the EXTI's line Flag for RTC WakeUpTimer */
    __HAL_RTC_EXTI_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
}

uint32_t RTC_checkWakeUpStatus()
{
    uint32_t val = HAL_RTCEx_GetWakeUpTimer(&_hrtc);
    return val;
}

uint32_t RTC_getCurrentTimeAs32bits()
{
    struct tm tms;
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    time_t convertedTime;

    HAL_RTC_GetTime(&_hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&_hrtc, &sDate, RTC_FORMAT_BIN);

    tms.tm_sec   = sTime.Seconds;
    tms.tm_min   = sTime.Minutes;
    tms.tm_hour  = sTime.Hours;
    tms.tm_mday  = sDate.Date;
    tms.tm_mon   = sDate.Month - 1;     // This needs to be the number of months since Jan
    tms.tm_year  = sDate.Year + 100;    // This needs to be the year since 1900
    tms.tm_wday  = sDate.WeekDay;
    //tm.tm_yday  = sTime.
    tms.tm_isdst = sTime.DayLightSaving;
    convertedTime = mktime(&tms);
    return (convertedTime);
}