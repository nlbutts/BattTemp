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
 * File: CommandParser.h
 * Creator: Nick Butts
 * Date: Jan 1, 2015
 *
 * Copyright Version 1.0
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include "Utilities.h"

void CommandParser_pushData(uint8_t * buf, uint16_t len);
void CommandParser_execute();

#endif /* COMMANDPARSER_H_ */
