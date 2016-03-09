#ifndef THERMISTOR_H_
#define THERMISTOR_H_

#include "Utilities.h"

void Thermistor_initialize(uint32_t biasResistorValueInOhms);
int32_t Thermistor_getTemperatureInTenthsOfDegC(uint32_t temperatureInMV, uint32_t vddaInMV);
int32_t Thermistor_getTemperatureInTenthsOfDegF(uint32_t temperatureInMV, uint32_t vddaInMV);

#endif /* THERMISTOR_H_ */
