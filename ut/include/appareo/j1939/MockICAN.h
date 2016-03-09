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
 * File: MockICAN.h
 * Creator: Nick Butts
 * Date: Apr 30, 2015
 *
 * Copyright Version 1.0
 */

#ifndef MOCK_ICAN_H
#define MOCK_ICAN_H

#include <stdint.h>
#include <gmock/gmock.h>
#include <appareo/j1939/messages/CanMessage.h>
#include <appareo/j1939/ICanHardwareInterface.h>

namespace Appareo {
namespace J1939 {

class MockICAN : public ICanHardwareInterface
{
public:
    MOCK_METHOD0(release, void());
    MOCK_METHOD0(isInitialized, bool());
    //MOCK_METHOD1(send, bool(const Messages::CanMessage& frame));
    bool send(const Messages::CanMessage& frame)    {_msg = frame; return true;}
    MOCK_METHOD1(receive, bool(Messages::CanMessage& frame));
    MOCK_METHOD0(getBusState, CANBusState());

    Messages::CanMessage _msg;
};


} // namespace J1939
} // namespace Appareo

#endif /* MOCK_ICAN_H */

