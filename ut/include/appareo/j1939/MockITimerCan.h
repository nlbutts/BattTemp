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
 * File: MockITimerCan.h
 * Creator: Nick Butts
 * Date: May 1, 2015
 *
 * Copyright Version 1.0
 */

#ifndef MOCK_ITIMER_CAN_H_
#define MOCK_ITIMER_CAN_H_

#include <stdint.h>
#include <gmock/gmock.h>
#include <appareo/j1939/ITimer.h>

namespace Appareo {
namespace J1939 {

class MockITimerCan : public ITimer
{
public:
    MOCK_METHOD1(setTimerMs, void(uint32_t timeoutInMs));
    MOCK_METHOD1(setTimerUs, void(uint32_t timeoutInUs));
    MOCK_CONST_METHOD0(isTimerExpired, bool());
};

} // namespace J1939
} // namespace Appareo

#endif /* MOCK_ITIMER_CAN_H_ */

