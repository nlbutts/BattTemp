#include "TempLogger.h"
#include <stdio.h>
#include "Clocks.h"
#include "AnalogVoltage.h"
#include "RTCC.h"
#include "Thermistor.h"
#include "GPIO.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "CommandParser.h"
#include "i2c.h"
#include "DataLogger.h"

static void enableUSB();
//static void clearEEPROM();
//static void saveEEPROM(int32_t temp);

void TempLogger_execute()
{
    GPIO_configureRunning();
    GPIO_ThermistorPowerSet(1);
    Thermistor_initialize(10000);
    AnalogVoltage_initialize();
    Clocks_enableLSE(LSE_MAX_DRIVE);

    RTC_initialize();
    RTC_setWakeUpTimer(5);

    while (!RTC_getWakeupFlag())
    {
        if (GPIO_3V3LDORead())
        {
            enableUSB();
            GPIO_PowerOnEEPROM();
            i2c_initialize();
            while (1)
            {
                CommandParser_execute();
                if (!GPIO_3V3LDORead())
                {
                    NVIC_SystemReset();
                }
            };
        }
    }

    DataLogger_init();

    /* Infinite loop */
    while (1)
    {
        GPIO_LEDPulse(3, 5);
        AnalogVoltage_initialize();
        GPIO_PowerOnEEPROM();
        i2c_initialize();
        GPIO_ThermistorPowerSet(1);
        HAL_Delay(10);
        AnalogVoltage_initiateConversion();
        while (AnalogVoltage_isCaptureComplete() == 0) {};
        int32_t temp = Thermistor_getTemperatureInTenthsOfDegC(AnalogVoltage_getMillivoltsFromADC(THERMISTOR), AnalogVoltage_getVddaIn_MV());
        DataLogger_storeTemp(RTC_getCurrentTimeAs32bits(), temp);
        GPIO_ThermistorPowerSet(0);
        AnalogVoltage_sleep();
        GPIO_PowerOffEEPROM();
        i2c_sleep();
        Clocks_disableHSIClock();
        IDD_SaveContext();
        /* Enable Ultra low power mode */
        HAL_PWREx_EnableUltraLowPower();
        /* Enable the fast wake up from Ultra low power mode */
        HAL_PWREx_EnableFastWakeUp();
        /* Clear Wake Up Flag */
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        RTC_setWakeUpTimer(3600);
        /* Enter Stop Mode */
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
        IDD_RestoreContext();
    }
}

static void enableUSB()
{
    // Enable the USB clock source
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
    Clocks_switchToPLLClock();
    MX_USB_DEVICE_Init();
    HAL_Delay(100);
}