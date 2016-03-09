#ifndef STM32L0_RTC_H_
#define STM32L0_RTC_H_

#include "Utilities.h"

void RTC_initialize();
bool RTC_getWakeupFlag();
void RTC_clearWakeupFlag();
void RTC_setTime(uint32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t seconds);
void RTC_getTime(uint32_t * year, uint8_t * month, uint8_t * day, uint8_t * hour, uint8_t * minute, uint8_t * seconds);
void RTC_setWakeUpTimer(uint32_t seconds);
uint32_t RTC_checkWakeUpStatus();
uint32_t RTC_getCurrentTimeAs32bits();

#endif /* STM32L0_RTC_H_ */
