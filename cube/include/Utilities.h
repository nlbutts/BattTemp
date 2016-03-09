/**
 * Copyright (c) 2012 Appareo Systems, LLC.
 * 1810 NDSU Research Park Circle North
 * Fargo ND, 58102
 * All rights reserved.
 *
 * This is the confidential and proprietary information of Appareo Systems, LLC. You shall not
 * disclose such confidential information and shall use it only in accordance with the terms
 * of the license agreement you entered into with Appareo.
 *
 * File: utilities.h
 * Creator: Nick Butts
 * Date: Jan 11, 2013
 *
 * Copyright Version 1.0
 */
#ifndef APPAREO_UTILITY_UTILITIES_H_
#define APPAREO_UTILITY_UTILITIES_H_

/*==============================================================================
   INCLUDE FILES
 *============================================================================*/
#include <stdint.h>
#include "stm32l0xx_hal.h"


/*==============================================================================
   MACROS / DEFINES
 *============================================================================*/

#define BIT_TO_MASK(x)  (uint32_t)((uint32_t)1 << (x))
#define BIN_TO_VALUE(x) (uint32_t)((uint32_t)1 << (x))

#define ELEMENTS(x)     (sizeof(x) / sizeof(x[0]))

#define BYTE_TO_WORD(x) ((uint16_t)(x[0]) | ((uint16_t)(x[1])) << 8)

typedef unsigned char     bool;
#define false       0
#define true        !false

//#define MIN(a, b)   (a > b ? b : a)

/*==============================================================================
   GLOBAL / EXTERN VARIABLES
 *============================================================================*/

/*==============================================================================
   FUNCTIONS
 *============================================================================*/


#endif /* APPAREO_UTILITY_UTILITIES_H_ */


