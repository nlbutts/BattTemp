#ifndef ANALOGVOLTAGE_H_
#define ANALOGVOLTAGE_H_

#include "Utilities.h"

typedef enum
{
    THERMISTOR,
    VREFINT,
    NUMBER_OF_ADC_SAMPLES
} AD_CHANNEL;

#define SAMPLES_TO_CAPTURE 10

bool AnalogVoltage_initialize();
void AnalogVoltage_sleep();
uint32_t AnalogVoltage_getMillivoltsFromADC(AD_CHANNEL channel);
uint32_t AnalogVoltage_getVddaIn_MV();
void AnalogVoltage_initiateConversion();
bool AnalogVoltage_isCaptureComplete();

#endif /* ANALOGVOLTAGE_H_ */
