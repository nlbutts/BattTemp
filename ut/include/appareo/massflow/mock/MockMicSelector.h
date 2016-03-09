
#include <stdint.h>
#include <gmock/gmock.h>
#include <appareo/massflow/IMicSelector.h>

namespace Appareo {
namespace Massflow {
namespace Mock {

class MockMicSelector : public IMicSelector
{
public:
    MockMicSelector(uint32_t numberOfMics) : _numberOfMics(numberOfMics) {}
    virtual ~MockMicSelector() {}
    
    MOCK_METHOD1(select, void(uint32_t));
    uint32_t getNumberOfMics(void) { return _numberOfMics; }

private:
    uint32_t _numberOfMics;
};

} // namespace Mock
} // namespace Massflow
} // namespace Appareo

