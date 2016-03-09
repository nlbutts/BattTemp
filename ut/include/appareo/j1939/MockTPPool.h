/**
 * Copyright (c) 2015 Appareo Systems, LLC.
 * 1810 NDSU Research Park Circle North
 * Fargo ND, 58102
 * All rights reserved.
 *
 * This is the confidential and proprietary information of Appareo Systems, LLC. You shall not
 * disclose such confidential information and shall use it only in accordance with the terms
 * of the license agreement you entered into with Appareo.
 *
 * File: MockTPPool.h
 * Creator: Nick Butts
 * Date: Jul 16, 2015
 *
 * Copyright Version 1.0
 */

#ifndef MOCK_TP_POOL_H_
#define MOCK_TP_POOL_H_

#include <stdint.h>
#include <gmock/gmock.h>
#include "appareo/j1939/transportProtocol/ITPPool.h"

namespace Appareo {
namespace J1939 {

class MockTPPool: public TransportProtocol::ITPPool
{
public:
    MOCK_METHOD1(getTransportMessage, Messages::CanMessage*(uint16_t size));;
    MOCK_METHOD1(freeTransportMessage, void(Messages::CanMessage *tpmsg));;
};

} // namespace J1939
} // namespace Appareo

#endif /* MOCK_TP_POOL_H_ */

