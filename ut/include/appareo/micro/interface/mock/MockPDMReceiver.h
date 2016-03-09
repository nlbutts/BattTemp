
#include <stdint.h>
#include <gmock/gmock.h>
#include <appareo/micro/interface/IPDMReceiver.h>

namespace Appareo {
namespace Micro {
namespace Interface {
namespace Mock {
    
class MockPDMReceiver : public IPDMReceiverU16
{
public:
    MockPDMReceiver() {}
    virtual ~MockPDMReceiver() {}

    MOCK_METHOD2(startAsyncRead, bool(uint16_t*, size_t));
    MOCK_METHOD0(isAsyncReadInProgress, bool(void));
};

} // namespace Mock
} // namespace Interface
} // namespace Micro
} // namespace Appareo

