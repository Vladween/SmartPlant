#ifndef __SMART_PLANT_STATE_HPP__
#define __SMART_PLANT_STATE_HPP__

#include "Variable.hpp"

class OutputPin : public Variable
{
public:
    OutputPin(const char* name, bool isMutable, uint8_t pin, bool isOn = false)
        : Variable(name, isMutable), m_pin(pin), m_isOn(isOn)
    {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, m_isOn);
    }

    void turnOn()
    {
        digitalWrite(m_pin, HIGH);
        m_isOn = true;
    }
    void turnOff()
    {
        digitalWrite(m_pin, LOW);
        m_isOn = false;
    }
    bool isOn() const
    {
        return m_isOn;
    }
protected:
    String set(uint8_t argc, const String* argv) override
    {
        if(argc != 1)
            return Responce::InvalidNumberOfArguments;

        if(argv[0] == "ON")
            turnOn();
        else if(argv[0] == "OFF")
            turnOff();
        else return Responce::InvalidData;

        return Responce::OK(get());
    }
    String get() const override
    {
        return (m_isOn) ? "ON" : "OFF";
    }
    String get_json() const override
    {
        return (m_isOn) ? "true" : "false";
    }
private:
    uint8_t m_pin;
    bool m_isOn;
};

#endif