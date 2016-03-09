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
 * File: MockRoutingService.h
 * Creator: Nick Butts
 * Date: Apr 30, 2015
 *
 * Copyright Version 1.0
 */

#ifndef MOCK_ROUTING_SERVICE_H_
#define MOCK_ROUTING_SERVICE_H_

#include <stdint.h>
#include <gmock/gmock.h>
#include <appareo/j1939/messages/CanMessage.h>
#include <appareo/j1939/messages/PGNRequest.h>
#include <appareo/j1939/IRoutingService.h>

namespace Appareo {
namespace J1939 {

class MockRoutingService: public IRoutingService
{
public:
    MOCK_METHOD1(processMessage, void(Messages::CanMessage &message));
    MOCK_METHOD1(processPGNRequests, void(Messages::PGNRequest &pgnRequest));
    MOCK_METHOD0(execute, void());
    MOCK_METHOD1(transportCompleteCallback, void(bool success));
};

} // namespace J1939
} // namespace Appareo

#endif /* MOCK_ROUTING_SERVICE_H_ */

