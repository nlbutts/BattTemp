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
 * File: AnalogVoltage.cpp
 * Creator: Nick Butts
 * Date: Nov 23, 2014
 *
 * Copyright Version 1.0
 */

#include "AnalogVoltage.h"
 #include "GPIO.h"
#include <string.h>

static bool                 _initialized = false;
static uint16_t             _vrefcal;
static uint32_t             _vddaIn_mV;
static uint32_t             _adValues[SAMPLES_TO_CAPTURE * 2];
static uint32_t             _filteredADValues[NUMBER_OF_ADC_SAMPLES];
static ADC_HandleTypeDef    _hadc;
static uint8_t              _capturedSamples;

bool AnalogVoltage_initialize()
{
    if (!_initialized)
    {
        __DMA1_CLK_ENABLE();
        __ADC1_CLK_ENABLE();

        _vrefcal = *((uint16_t*) 0x1FF80078);

        ADC_ChannelConfTypeDef sConfig;
        // I think this resets the AD module so it will work after going to sleep
        RCC->CR |= 0x1;
        HAL_Delay(10);
        memset((void*)&_hadc, 0, sizeof(_hadc));
        RCC->APB2RSTR |= 0x200;
        HAL_Delay(10);
        RCC->APB2RSTR = 0;
        HAL_Delay(10);

        /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
        */
        _hadc.Instance = ADC1;
        _hadc.Init.OversamplingMode = DISABLE;
        _hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
        _hadc.Init.Resolution = ADC_RESOLUTION12b;
        _hadc.Init.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
        _hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
        _hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        _hadc.Init.ContinuousConvMode = ENABLE;
        _hadc.Init.DiscontinuousConvMode = DISABLE;
        _hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIG_EDGE_NONE;
        _hadc.Init.DMAContinuousRequests = DISABLE;
        _hadc.Init.EOCSelection = EOC_SEQ_CONV;
        _hadc.Init.Overrun = OVR_DATA_PRESERVED;
        _hadc.Init.LowPowerAutoWait = DISABLE;
        _hadc.Init.LowPowerFrequencyMode = DISABLE;
        //_hadc.Init.LowPowerAutoOff = DISABLE;
        if (HAL_ADC_Init(&_hadc) != HAL_OK)
        {
            // We had a failure to initialize.
            return false;
        }

        /**Configure for the selected ADC regular channel to be converted.
        */
        sConfig.Channel = ADC_CHANNEL_4|ADC_CHANNEL_VREFINT;
        if (HAL_ADC_ConfigChannel(&_hadc, &sConfig) != HAL_OK)
        {
            return false;
        }

        AnalogVoltage_initiateConversion();
        _initialized = true;
    }
    return true;
}

void AnalogVoltage_sleep()
{
    if (_initialized)
    {
        HAL_ADC_Stop(&_hadc);
        HAL_Delay(100);
        HAL_ADC_Stop_DMA(&_hadc);
        HAL_Delay(100);
        HAL_ADC_DeInit(&_hadc);
        HAL_Delay(100);
        __DMA1_CLK_DISABLE();
        __ADC1_CLK_DISABLE();
        _initialized = false;
    }
}


uint32_t AnalogVoltage_getMillivoltsFromADC(AD_CHANNEL channel)
{
    _vddaIn_mV = 3000 * _vrefcal / _filteredADValues[VREFINT];
    uint32_t voltageInMV = ((_vddaIn_mV * _filteredADValues[THERMISTOR]) + (4096/2)) / 4096;
    return voltageInMV;
}

uint32_t AnalogVoltage_getVddaIn_MV()
{
    return _vddaIn_mV;
}

/**
 * This function sets up the AD filter and blocks until all of the samples have been received
 *
 * @param samples the number of AD samples to capture
 */
void AnalogVoltage_initiateConversion()
{
    _capturedSamples = 0;
    HAL_ADC_Start_DMA(&_hadc, _adValues, SAMPLES_TO_CAPTURE * 2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    //GPIO_LEDToggle(3);
    _capturedSamples = 1;
    _filteredADValues[0] = 0;
    _filteredADValues[1] = 0;
    for (int i = 0; i < (SAMPLES_TO_CAPTURE * 2); i += 2)
    {
        _filteredADValues[0] += _adValues[i];
        _filteredADValues[1] += _adValues[i+1];
    }
    _filteredADValues[0] /= SAMPLES_TO_CAPTURE;
    _filteredADValues[1] /= SAMPLES_TO_CAPTURE;
}

bool AnalogVoltage_isCaptureComplete()
{
    return _capturedSamples;
}