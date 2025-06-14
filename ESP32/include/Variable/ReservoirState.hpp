#ifndef __SMART_PLANT_RESERVOIR_STATE_HPP__
#define __SMART_PLANT_RESERVOIR_STATE_HPP__

#include "Variable.hpp"

class ReservoirState : public Variable
{
public:
    enum : uint16_t {
        Ok = 0,
        Empty = 1000,
        Low = 1200,
        NotConnected = 4095
    };

    ReservoirState(const char* name, uint8_t pin)
        : Variable(name, false), m_pin(pin), m_state(0) {}

    void update() const
    {
        uint8_t read = analogRead(m_pin);
        if(read == 4095)
            m_state = NotConnected;
        if(read > Low)
            m_state = Ok;
        if(read <= Low && read >= Empty)
            m_state = Low;
        m_state = Empty;
    }

    uint16_t& state() const
    {
        return m_state;
    }    
protected:
    String set(uint8_t argc, const String* argv) override { return {}; }
    String get() const override
    {
        update();

        switch(m_state)
        {
        case Ok:
            return "OK";
        case Low:
            return "LOW";
        case Empty:
            return "EMPTY";
        default:
            return "Unknown";
        }
    }
    String get_json() const override
    {
        return "\"" + get() + "\"";
    }
private:
    mutable uint16_t m_state;
    uint8_t m_pin;
};

#endif