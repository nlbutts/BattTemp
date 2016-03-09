#include "GPIO.h"
#include "stm32l0xx_hal_conf.h"

void GPIO_configureRunning()
{
    /* GPIO in use
    * PA0 - User button (not populated on latest boards)
    * PA3 - VDD_ANA (voltage divider of VDD)
    * PA4 - Thermistor
    * PA5 - Power to the thermistor
    * PA8 - Write protect (active high) for EEPROM
    * PB3 - EEPROM Power
    * PB8 - EEPROM I2C clock
    * PB9 - EEPROM I2C data
    * PB13 - Yello LED
    * PB14 - Green LED
    * PB15 - Red LED
    * PC13 - USB Power present
    **/
    GPIO_InitTypeDef cfg;
    cfg.Pin         = GPIO_PIN_4;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin         = GPIO_PIN_5;
    cfg.Mode        = GPIO_MODE_OUTPUT_PP;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin         = GPIO_PIN_8;
    cfg.Mode        = GPIO_MODE_OUTPUT_PP;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin         = GPIO_PIN_3;
    cfg.Mode        = GPIO_MODE_OUTPUT_PP;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_8;
    cfg.Mode        = GPIO_MODE_AF_OD;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    cfg.Alternate   = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_9;
    cfg.Mode        = GPIO_MODE_AF_OD;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    cfg.Alternate   = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_13;
    cfg.Mode        = GPIO_MODE_OUTPUT_PP;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_14;
    cfg.Mode        = GPIO_MODE_OUTPUT_PP;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_15;
    cfg.Mode        = GPIO_MODE_OUTPUT_PP;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_13;
    cfg.Mode        = GPIO_MODE_INPUT;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &cfg);
}

void GPIO_configureForSleep()
{
    /* GPIO in use
    * PA0 - User button (not populated on latest boards)
    * PA3 - VDD_ANA (voltage divider of VDD)
    * PA4 - Thermistor
    * PA5 - Power to the thermistor
    * PA8 - Write protect (active high) for EEPROM
    * PB3 - EEPROM Power
    * PB8 - EEPROM I2C clock
    * PB9 - EEPROM I2C data
    * PB13 - Yello LED
    * PB14 - Green LED
    * PB15 - Red LED
    * PC13 - USB Power present
    **/
    GPIO_InitTypeDef cfg;
    cfg.Pin         = GPIO_PIN_4;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin         = GPIO_PIN_5;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin         = GPIO_PIN_8;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &cfg);

    cfg.Pin         = GPIO_PIN_3;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_8;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_9;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_13;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_14;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_15;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &cfg);

    cfg.Pin         = GPIO_PIN_13;
    cfg.Mode        = GPIO_MODE_ANALOG;
    cfg.Pull        = GPIO_NOPULL;
    cfg.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &cfg);
}

void GPIO_setEEPROMWriteProtect()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
}

void GPIO_ThermistorPowerSet(uint8_t value)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, value);
}

void GPIO_LEDToggle(uint8_t led)
{
    if (led > 0)
    {
        uint16_t mask = GPIO_PIN_15;
        mask >>= (led - 1);
        HAL_GPIO_TogglePin(GPIOB, mask);
    }
}

void GPIO_LEDSet(uint8_t led, uint8_t value)
{
    if (led > 0)
    {
        uint16_t mask = GPIO_PIN_15;
        mask >>= (led - 1);
        HAL_GPIO_WritePin(GPIOB, mask, value);
    }
}

void GPIO_LEDPulse(uint8_t led, uint8_t numberOfPulses)
{
    if (led > 0)
    {
        uint16_t mask = GPIO_PIN_15;
        mask >>= (led - 1);
        for (uint8_t i = 0; i < numberOfPulses; ++i)
        {
            HAL_GPIO_WritePin(GPIOB, mask, 1);
            HAL_Delay(100);
            HAL_GPIO_WritePin(GPIOB, mask, 0);
            HAL_Delay(100);
        }
    }
}

void GPIO_PowerOnEEPROM()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
}

void GPIO_PowerOffEEPROM()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
}

uint8_t GPIO_3V3LDORead()
{
    return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
}

uint32_t GPIOA_MODER = 0, GPIOA_OTYPER = 0, GPIOA_OSPEEDR = 0, GPIOA_AFRL = 0, GPIOA_AFRH = 0;
uint32_t GPIOB_MODER = 0, GPIOB_OTYPER = 0, GPIOB_OSPEEDR = 0, GPIOB_AFRL = 0, GPIOB_AFRH = 0;
uint32_t GPIOC_MODER = 0, GPIOC_OTYPER = 0, GPIOC_OSPEEDR = 0, GPIOC_AFRL = 0, GPIOC_AFRH = 0;
uint32_t GPIOD_MODER = 0, GPIOD_OTYPER = 0, GPIOD_OSPEEDR = 0, GPIOD_AFRL = 0, GPIOD_AFRH = 0;
uint32_t GPIOH_MODER = 0, GPIOH_OTYPER = 0, GPIOH_OSPEEDR = 0, GPIOH_AFRL = 0, GPIOH_AFRH = 0;

void IDD_SaveContext(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIOA_MODER = GPIOA->MODER;
    GPIOA_OTYPER = GPIOA->OTYPER;
    GPIOA_OSPEEDR = GPIOA->OSPEEDR;
    GPIOA_AFRL = GPIOA->AFR[0];
    GPIOA_AFRH = GPIOA->AFR[1];

    GPIOB_MODER = GPIOB->MODER;
    GPIOB_OTYPER = GPIOB->OTYPER;
    GPIOB_OSPEEDR = GPIOB->OSPEEDR;
    GPIOB_AFRL = GPIOB->AFR[0];
    GPIOB_AFRH = GPIOB->AFR[1];

    GPIOC_MODER = GPIOC->MODER;
    GPIOC_OTYPER = GPIOC->OTYPER;
    GPIOC_OSPEEDR = GPIOC->OSPEEDR;
    GPIOC_AFRL = GPIOC->AFR[0];
    GPIOC_AFRH = GPIOC->AFR[1];

    GPIOD_MODER = GPIOD->MODER;
    GPIOD_OTYPER = GPIOD->OTYPER;
    GPIOD_OSPEEDR = GPIOD->OSPEEDR;
    GPIOD_AFRL = GPIOD->AFR[0];
    GPIOD_AFRH = GPIOD->AFR[1];

    GPIOH_MODER = GPIOH->MODER;
    GPIOH_OTYPER = GPIOH->OTYPER;
    GPIOH_OSPEEDR = GPIOH->OSPEEDR;
    GPIOH_AFRL = GPIOH->AFR[0];
    GPIOH_AFRH = GPIOH->AFR[1];

    /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

    /* Disable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
}

/**
 * @brief Restore Demonstration context (GPIOs Configurations, peripherals,...).
 * @param None
 * @retval None
 */
void IDD_RestoreContext(void)
{
    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIOA->MODER = GPIOA_MODER;
    GPIOA->OTYPER = GPIOA_OTYPER;
    GPIOA->OSPEEDR = GPIOA_OSPEEDR;
    GPIOA->AFR[0] = GPIOA_AFRL;
    GPIOA->AFR[1] = GPIOA_AFRH;

    GPIOB->MODER = GPIOB_MODER;
    GPIOB->OTYPER = GPIOB_OTYPER;
    GPIOB->OSPEEDR = GPIOB_OSPEEDR;
    GPIOB->AFR[0] = GPIOB_AFRL;
    GPIOB->AFR[1] = GPIOB_AFRH;

    GPIOC->MODER = GPIOC_MODER;
    GPIOC->OTYPER = GPIOC_OTYPER;
    GPIOC->OSPEEDR = GPIOC_OSPEEDR;
    GPIOC->AFR[0] = GPIOC_AFRL;
    GPIOC->AFR[1] = GPIOC_AFRH;

    GPIOD->MODER = GPIOD_MODER;
    GPIOD->OTYPER = GPIOD_OTYPER;
    GPIOD->OSPEEDR = GPIOD_OSPEEDR;
    GPIOD->AFR[0] = GPIOD_AFRL;
    GPIOD->AFR[1] = GPIOD_AFRH;

    GPIOH->MODER = GPIOH_MODER;
    GPIOH->OTYPER = GPIOH_OTYPER;
    GPIOH->OSPEEDR = GPIOH_OSPEEDR;
    GPIOH->AFR[0] = GPIOH_AFRL;
    GPIOH->AFR[1] = GPIOH_AFRH;
}

