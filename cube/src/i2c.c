#include <stdint.h>
#include "stm32l0xx_hal_conf.h"
#include "Utilities.h"
#include "i2c.h"
#include <string.h>

static I2C_HandleTypeDef I2CHandle;

void i2c_initialize()
{
    // Enable clocks
    __HAL_RCC_I2C1_CLK_ENABLE();

    /*
    *   Initialize the I2C Master
    *   Setup the timing registers
    *
    **/
    I2CHandle.Init.Timing = (3 << 28) |
                            (4 << 20) |
                            (2 << 16) |
                            (0x13 << 8) |
                            (0x0F);
    I2CHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2CHandle.Instance = I2C1;

    HAL_I2C_Init(&I2CHandle);
}

void i2c_sleep()
{
    HAL_I2C_DeInit(&I2CHandle);
    __HAL_RCC_I2C1_CLK_DISABLE();
}

void i2c_writeByte(uint8_t addr, uint16_t reg, uint8_t data)
{
    uint8_t buffer[5];
    buffer[0] = reg >> 8;
    buffer[1] = reg;
    buffer[2] = data;
    HAL_I2C_Master_Transmit(&I2CHandle, addr, buffer, 3, 1000);
}

void i2c_writeData(uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;
    uint8_t buffer[2];
    buffer[0] = reg >> 8;
    buffer[1] = reg;
    status = HAL_I2C_Master_Transmit(&I2CHandle, addr, buffer, 2, 1000);
    if (status == HAL_OK)
    {
        status = HAL_I2C_Master_Transmit(&I2CHandle, addr, data, len, 1000);
    }
}

void i2c_writePage(uint8_t addr, uint16_t reg, uint8_t *data)
{
    HAL_StatusTypeDef status;
    uint8_t buffer[32+2];
    buffer[0] = reg >> 8;
    buffer[1] = reg;
    memcpy(&buffer[2], data, 32);
    status = HAL_I2C_Master_Transmit(&I2CHandle, addr, buffer, 32+2, 1000);
}

uint8_t i2c_readByte(uint8_t addr, uint16_t reg)
{
    uint8_t buffer[5];
    buffer[0] = reg >> 8;
    buffer[1] = reg;
    HAL_I2C_Master_Transmit(&I2CHandle, addr, buffer, 2, 1000);
    HAL_I2C_Master_Receive(&I2CHandle, addr, buffer, 1, 1000);
    return buffer[0];
}

uint8_t i2c_readData(uint8_t addr, uint16_t reg, uint8_t * data, uint16_t length)
{
    uint8_t buffer[2];
    buffer[0] = reg >> 8;
    buffer[1] = reg;
    HAL_I2C_Master_Transmit(&I2CHandle, addr, buffer, 2, 1000);
    HAL_I2C_Master_Receive(&I2CHandle, addr, data, length, 1000);
    return length;
}


