#include <stdint.h>

/**
* Initialize the I2C system
*
*/
void i2c_initialize();

/**
* Unitialize the I2C and turn the clock off
*
*/
void i2c_sleep();

/**
* Write a single byte
*
* @param addr the I2C address of the peripheiral
* @param reg the register or address to set the write pointer
* @param data the byte to write
*/
void i2c_writeByte(uint8_t addr, uint16_t reg, uint8_t data);

/**
* Write a single page (32 bytes)
*
* @param addr the I2C address of the peripheiral
* @param reg the register or address to set the write pointer
* @param data pointer to an array of 32 bytes
* @param len the length of data to write
*/
void i2c_writeData(uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len);

/**
* Write a single page (32 bytes)
*
* @param addr the I2C address of the peripheiral
* @param reg the register or address to set the write pointer
* @param data pointer to an array of 32 bytes
*/
void i2c_writePage(uint8_t addr, uint16_t reg, uint8_t *data);

/**
* Reads a single byte
*
* @param addr the I2C address of the peripheiral
* @param reg the register or address to set the write pointer
* @return uint8_t the byte read from memory
*/
uint8_t i2c_readByte(uint8_t addr, uint16_t reg);

/**
* Reads a single page (32 bytes)
*
* @param addr the I2C address of the peripheiral
* @param reg the register or address to set the write pointer
* @param data a pointer to 32 bytes to receive the read data
* @return uint8_t the number of bytes read
*/
uint8_t i2c_readData(uint8_t addr, uint16_t reg, uint8_t * data, uint16_t length);
