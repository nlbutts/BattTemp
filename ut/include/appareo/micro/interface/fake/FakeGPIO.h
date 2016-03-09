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
 * File: FakeGPIO.h
 * Creator: bschleusner
 * Date: Jan 20, 2015
 *
 * Copyright Version 1.0
 */

#ifndef FAKE_GPIO_H
#define FAKE_GPIO_H

#include <appareo/micro/interface/IGPIO.h>

namespace Appareo {
namespace Micro {
namespace Interface {
namespace Fake {

class FakeGPIO : public Appareo::Micro::Interface::IGPIO
{
public:
    FakeGPIO() :
        _state(&this->_innerState), _invert(false), _innerState(false), _constState(false)
    {}

    FakeGPIO(bool* state) :
        _state(state), _invert(false), _innerState(false), _constState(false)
    {}

    FakeGPIO(bool state, bool constState) :
        _state(&this->_innerState), _invert(false), _innerState(state), _constState(constState)
    {}

    virtual ~FakeGPIO() 
    {}

    virtual void set()
    {
        if(!_constState)
        {
            *_state = true;    
        }
    }

    virtual void clear()
    {
        if(!_constState)
        {
            *_state = false;    
        }
    }

    virtual void toggle()
    {
        get() ? clear() : set();
    }   

    virtual bool get() const
    {
        if(_invert)
        {
            return !(*_state);
        }
        else
        {
            return *_state;
        }
    }

    virtual void operator = (const uint32_t in) 
    {
        if (in)
            set();
        else
            clear();
    }

private:
    bool* _state;
    bool _invert;

    bool _innerState;
    bool _constState;
};

    
} // namespace Fake
} // namespace Interface
} // namespace Micro
} // namespace Appareo

#endif /* FAKE_GPIO_H */
