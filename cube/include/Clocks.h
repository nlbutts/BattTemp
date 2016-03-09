#ifndef CLOCKS_H_
#define CLOCKS_H_

#include "Utilities.h"

typedef enum
{
    MSI_65KHz,
    MSI_131KHz,
    MSI_262KHz,
    MSI_524KHz,
    MSI_1048KHz,
    MSI_2097KHz,
    MSI_4194KHz,
} MSI_CLOCKS_E;

typedef enum
{
    LSE_LOW_DRIVE,
    LSE_MEDIUM_DRIVE,
    LSE_HIGH_DRIVE,
    LSE_MAX_DRIVE
} LSE_DRIVE_STRENGTH;

void Clocks_initialize();
void Clocks_switchToMSIClock(MSI_CLOCKS_E clk);
void Clocks_switchToHSIClock();
void Clocks_switchToPLLClock();
void Clocks_disableHSIClock();
void Clocks_enableLSE(LSE_DRIVE_STRENGTH ds);
void Clocks_enableMCO();
void Clocks_enableLSI();


#endif /* CLOCKS_H_ */
