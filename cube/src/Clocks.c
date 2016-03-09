#include "Clocks.h"
#include "stm32l0xx_hal_conf.h"

static RCC_OscInitTypeDef _RCC_OscInitStruct;
static RCC_ClkInitTypeDef _RCC_ClkInitStruct;

void Clocks_initialize()
{
    __PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    _RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    _RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    _RCC_OscInitStruct.MSICalibrationValue = 0;
    _RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
    _RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    _RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    HAL_RCC_OscConfig(&_RCC_OscInitStruct);

    _RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    _RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    _RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    _RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    _RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&_RCC_ClkInitStruct, FLASH_LATENCY_1);

    __SYSCFG_CLK_ENABLE();
}

void Clocks_switchToMSIClock(MSI_CLOCKS_E clk)
{
    _RCC_ClkInitStruct.ClockType            = RCC_CLOCKTYPE_SYSCLK;
    _RCC_ClkInitStruct.SYSCLKSource         = RCC_SYSCLKSOURCE_MSI;
    _RCC_ClkInitStruct.AHBCLKDivider        = RCC_SYSCLK_DIV1;
    _RCC_ClkInitStruct.APB1CLKDivider       = RCC_HCLK_DIV1;
    _RCC_ClkInitStruct.APB2CLKDivider       = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&_RCC_ClkInitStruct, FLASH_LATENCY_1);

    _RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_MSI;
    _RCC_OscInitStruct.HSEState             = RCC_HSE_OFF;
    _RCC_OscInitStruct.LSEState             = RCC_LSE_OFF;
    _RCC_OscInitStruct.HSIState             = RCC_HSI_OFF;
    _RCC_OscInitStruct.HSICalibrationValue  = 0;
    _RCC_OscInitStruct.LSIState             = RCC_LSI_ON;
    _RCC_OscInitStruct.HSI48State           = RCC_HSI48_OFF;
    _RCC_OscInitStruct.MSIState             = RCC_MSI_ON;
    _RCC_OscInitStruct.MSICalibrationValue  = 0;
    _RCC_OscInitStruct.MSIClockRange        = clk;
    _RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&_RCC_OscInitStruct);

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
}

void Clocks_switchToHSIClock()
{
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    _RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI;
    _RCC_OscInitStruct.HSEState             = RCC_HSE_OFF;
    _RCC_OscInitStruct.LSEState             = RCC_LSE_OFF;
    _RCC_OscInitStruct.HSIState             = RCC_HSI_ON;
    _RCC_OscInitStruct.HSICalibrationValue  = 0;
    _RCC_OscInitStruct.LSIState             = RCC_LSI_ON;
    _RCC_OscInitStruct.HSI48State           = RCC_HSI48_OFF;
    _RCC_OscInitStruct.MSIState             = RCC_MSI_ON;
    _RCC_OscInitStruct.MSICalibrationValue  = 0;
    _RCC_OscInitStruct.MSIClockRange        = 0;
    _RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_OFF;
    HAL_RCC_OscConfig(&_RCC_OscInitStruct);

    _RCC_ClkInitStruct.ClockType        = RCC_CLOCKTYPE_SYSCLK;
    _RCC_ClkInitStruct.SYSCLKSource     = RCC_SYSCLKSOURCE_HSI;
    _RCC_ClkInitStruct.AHBCLKDivider    = RCC_SYSCLK_DIV1;
    _RCC_ClkInitStruct.APB1CLKDivider   = RCC_HCLK_DIV1;
    _RCC_ClkInitStruct.APB2CLKDivider   = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&_RCC_ClkInitStruct, FLASH_LATENCY_1);
}

void Clocks_switchToPLLClock()
{
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    _RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
    _RCC_OscInitStruct.HSEState             = RCC_HSE_OFF;
    _RCC_OscInitStruct.LSEState             = RCC_LSE_ON;
    _RCC_OscInitStruct.HSIState             = RCC_HSI_ON;
    _RCC_OscInitStruct.HSICalibrationValue  = 0;
    _RCC_OscInitStruct.LSIState             = RCC_LSI_ON;
    _RCC_OscInitStruct.HSI48State           = RCC_HSI48_ON;
    _RCC_OscInitStruct.MSIState             = RCC_MSI_ON;
    _RCC_OscInitStruct.MSICalibrationValue  = 0;
    _RCC_OscInitStruct.MSIClockRange        = 0;
    _RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
    _RCC_OscInitStruct.PLL.PLLDIV           = RCC_PLLDIV_3;
    _RCC_OscInitStruct.PLL.PLLMUL           = RCC_PLLMUL_6;
    _RCC_OscInitStruct.PLL.PLLSource        = RCC_PLLSOURCE_HSI;
    HAL_RCC_OscConfig(&_RCC_OscInitStruct);

    _RCC_ClkInitStruct.ClockType        = RCC_CLOCKTYPE_SYSCLK;
    _RCC_ClkInitStruct.SYSCLKSource     = RCC_SYSCLKSOURCE_PLLCLK;
    _RCC_ClkInitStruct.AHBCLKDivider    = RCC_SYSCLK_DIV1;
    _RCC_ClkInitStruct.APB1CLKDivider   = RCC_HCLK_DIV1;
    _RCC_ClkInitStruct.APB2CLKDivider   = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&_RCC_ClkInitStruct, FLASH_LATENCY_1);
}

void Clocks_disableHSIClock()
{
    _RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI;
    _RCC_OscInitStruct.HSIState             = RCC_HSI_OFF;
    HAL_RCC_OscConfig(&_RCC_OscInitStruct);

}

void Clocks_enableLSE(LSE_DRIVE_STRENGTH ds)
{
    uint32_t temp;
    volatile RCC_TypeDef * rcc = (volatile  RCC_TypeDef*)RCC_BASE;
    temp = rcc->CSR;
    // Turn the LSE off.
    rcc->CSR &= BIT_TO_MASK(8);
    // Set the LSE drive strengh to max
    temp &= ~(BIT_TO_MASK(12) | BIT_TO_MASK(11));
    temp |= BIT_TO_MASK(12) | BIT_TO_MASK(11);
    rcc->CSR = temp;
    // Enable the LSE
    rcc->CSR |= BIT_TO_MASK(8);
    temp = rcc->CSR;
    do
    {
        temp = rcc->CSR;
        temp &= BIT_TO_MASK(9);
    } while (temp == 0);

}

void Clocks_enableLSI()
{
    uint32_t temp;
    volatile RCC_TypeDef * rcc = (volatile  RCC_TypeDef*)RCC_BASE;
    // Turn the LSE off.
    rcc->CSR &= BIT_TO_MASK(8);
    // Turn the LSI on
    rcc->CSR |= BIT_TO_MASK(0);
    do
    {
        temp = rcc->CSR;
        temp &= BIT_TO_MASK(1);
    } while (temp == 0);

}

void Clocks_enableMCO()
{
    volatile RCC_TypeDef * rcc = (volatile  RCC_TypeDef*)RCC_BASE;
    uint32_t temp;

    // Set PA9 to ALT function mode
    GPIO_InitTypeDef cfg;
    cfg.Pin         = GPIO_PIN_9;
    cfg.Mode        = GPIO_MODE_AF_PP;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_MEDIUM;
    cfg.Alternate   = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOA, &cfg);

    // Set MCOSEL to LSE
    temp = rcc->CFGR;
    // Clear MCOPRE and MCOSEL
    temp &= ~(BIT_TO_MASK(30) |
              BIT_TO_MASK(29) |
              BIT_TO_MASK(28) |
              BIT_TO_MASK(27) |
              BIT_TO_MASK(26) |
              BIT_TO_MASK(25) |
              BIT_TO_MASK(24));
    // Set the MCO
    temp |= BIT_TO_MASK(26) |
            BIT_TO_MASK(25) |
            BIT_TO_MASK(24);
    rcc->CFGR = temp;
}