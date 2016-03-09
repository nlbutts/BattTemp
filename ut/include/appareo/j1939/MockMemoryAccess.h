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
 * File: MockMemoryAccess.h
 * Creator: Nick Butts
 * Date: Apr 30, 2015
 *
 * Copyright Version 1.0
 */

#ifndef MOCK_MEMORY_ACCESS_H_
#define MOCK_MEMORY_ACCESS_H_

#include <stdint.h>
#include <gmock/gmock.h>
#include "appareo/j1939/IMemoryAccess.h"
#include "appareo/j1939/messages/DM14MemoryAccessRequest.h"
#include "appareo/j1939/messages/DM15MemoryAccessResponse.h"
#include "appareo/j1939/messages/DM16BinaryDataTransfer.h"

namespace Appareo {
namespace J1939 {

using Appareo::J1939::Messages::DM14MemoryAccessRequest;
using Appareo::J1939::Messages::DM15MemoryAccessResponse;
using Appareo::J1939::Messages::DM16BinaryDataTransfer;

class MockMemoryAccess: public IMemoryAccess
{
public:
    MOCK_METHOD2(eraseMemory, DM15MemoryAccessResponse(uint32_t addr, uint32_t len));;
    MOCK_METHOD2(readMemory, DM15MemoryAccessResponse(DM14MemoryAccessRequest &dm14, DM16BinaryDataTransfer &dt));
    MOCK_METHOD1(scheduleWrite, DM15MemoryAccessResponse(DM14MemoryAccessRequest &dm14));
    MOCK_METHOD1(writeMemory, DM15MemoryAccessResponse(DM16BinaryDataTransfer &dt));
    MOCK_METHOD1(processEDCP, DM15MemoryAccessResponse(DM14MemoryAccessRequest &dm14));
    MOCK_METHOD0(processWrite, bool());
    MOCK_METHOD2(isMemoryRangeValid, bool(uint32_t addr, uint32_t len));
    MOCK_METHOD1(loadBootloader, void(uint8_t toolSA));
    MOCK_METHOD0(memoryAccessSessionStarted, void());
    MOCK_METHOD0(memoryAccessSessionEnded, void());
};

} // namespace J1939
} // namespace Appareo

#endif /* MOCK_MEMORY_ACCESS_H_ */

