#ifndef GPIO_H_
#define GPIO_H_

#include "Utilities.h"

void GPIO_configureRunning(void);
void GPIO_ThermistorPowerSet(uint8_t value);
void GPIO_LEDToggle(uint8_t led);
void GPIO_LEDSet(uint8_t led, uint8_t value);
void GPIO_LEDPulse(uint8_t led, uint8_t numberOfPulses);
void GPIO_PowerOnEEPROM();
void GPIO_PowerOffEEPROM();

uint8_t GPIO_3V3LDORead();


void IDD_SaveContext(void);
void IDD_RestoreContext(void);

#endif /* GPIO_H_ */
