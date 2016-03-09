#include "Thermistor.h"
#include "Utilities.h"
#include "LinearInterpolation.h"

static float power(float x, float y);

static uint32_t    _biasResistorValueInOhms;

const float _TableX[] =
{
        531,
        596,
        672,
        758,
        858,
        974,
       1110,
       1268,
       1452,
       1669,
       1925,
       2228,
       2586,
       3014,
       3535,
       4161,
       4917,
       5834,
       6948,
       8315,
      10000,
      12081,
      14674,
      17926,
      22021,
      27219,
      33892,
      42506,
      53650,
      68237,
      87559,
     113347,
     148171,
     195652,
};

const float _TableY[] =
{
    125,
    120,
    115,
    110,
    105,
    100,
    95 ,
    90 ,
    85 ,
    80 ,
    75 ,
    70 ,
    65 ,
    60 ,
    55 ,
    50 ,
    45 ,
    40 ,
    35 ,
    30 ,
    25 ,
    20 ,
    15 ,
    10 ,
    5  ,
    0  ,
    -5 ,
    -10,
    -15,
    -20,
    -25,
    -30,
    -35,
    -40,
};

void Thermistor_initialize(uint32_t biasResistorValueInOhms)
{
    _biasResistorValueInOhms = biasResistorValueInOhms;
    LinearInterpolation_initialize(_TableX, _TableY, ELEMENTS(_TableX));
}

int32_t Thermistor_getTemperatureInTenthsOfDegC(uint32_t temperatureInMV, uint32_t vddaInMV)
{
    float Rt = temperatureInMV * _biasResistorValueInOhms / (vddaInMV - temperatureInMV);
    float temp = LinearInterpolation_interpolate(Rt);
    return temp * 10;
}

int32_t Thermistor_getTemperatureInTenthsOfDegF(uint32_t temperatureInMV, uint32_t vddaInMV)
{
    int32_t tempInC = Thermistor_getTemperatureInTenthsOfDegC(temperatureInMV, vddaInMV);
    float tempInF = tempInC * 9;
    tempInF /= 5;
    tempInF += 320;
    return tempInF;
}

float power(float x, float y)
{
    uint8_t i;
    float out = x;
    for (i = 0; i < y; ++i)
    {
        out *= x;
    }
    return out;
}
