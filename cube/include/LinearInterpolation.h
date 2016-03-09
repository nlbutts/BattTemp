#ifndef LINEARINTERPOLATION_H_
#define LINEARINTERPOLATION_H_

#include "Utilities.h"

void LinearInterpolation_initialize(const float * x, const float * y, uint8_t tableSize);
float LinearInterpolation_interpolate(float xi);

#endif /* LINEARINTERPOLATION_H_ */
