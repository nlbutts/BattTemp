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
 * File: LinearInterpolation.cpp
 * Creator: Nick Butts
 * Date: Mar 6, 2013
 *
 * Copyright Version 1.0
 */

#include <LinearInterpolation.h>
#include <stdlib.h>

static const float * _x;
static const float * _y;
static uint8_t _tableSize;

void LinearInterpolation_initialize(const float * x, const float * y, uint8_t tableSize)
{
    _x = x;
    _y = y;
    _tableSize = tableSize;
}

float LinearInterpolation_interpolate(float xi)
{
    float yi;
    uint8_t i;

    /* Find the X0 value */
    for (i = 1; i < _tableSize; ++i)
    {
        if (xi < _x[i])
        {
            break;
        }
    }

    if (i == _tableSize)
    {
        // Handle the edge case where the xi is the last table element
        yi = _y[i - 1];
    }
    else
    {
        float num   = _y[i] - _y[i - 1];
        float den   = _x[i] - _x[i - 1];
        float temp  = (xi - _x[i - 1]);
        temp       *= num;
        temp       /= den;
        yi          = _y[i - 1] + temp;
    }
    return yi;
}
