#include <stdint.h>
#include <Constants.h>

void DataLogger_init();
void DataLogger_storeTemp(uint32_t currentTime, int32_t temp);
void DataLogger_readTemp(uint32_t location, uint32_t * recordTime, int32_t * temp);
