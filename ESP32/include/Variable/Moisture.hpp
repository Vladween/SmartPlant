#ifndef __SMART_PLANT_MOISTURE_HPP__
#define __SMART_PLANT_MOISTURE_HPP__

#include "Variable.hpp"

class Moisture : public Variable
{
public:
    Moisture(const char* name, uint8_t pin)
        : Variable(name, false), m_pin(pin), m_value(0)
    {}

    void update()
    {
        static constexpr uint16_t DryMoisture = 4095;
        static constexpr uint16_t WetMoisture = 1300;
        m_value = 100 * (DryMoisture - clamp(analogRead(m_pin), WetMoisture, DryMoisture)) / (DryMoisture - WetMoisture);
    }
    uint8_t pin() const
    {
        return m_pin;
    }
    uint8_t value() const
    {
        return m_value;
    }
protected:
    String set(uint8_t argc, const String* argv) { return {}; }
    String get() const override
    {
        return (String)m_value + '%';
    }
    String get_json() const override
    {
        return (String)m_value;
    }
private:
    uint8_t m_value;
    uint8_t m_pin;

    static constexpr uint16_t clamp(uint16_t value, uint16_t min, uint16_t max)
    {
        return (value < min) ? min : (value > max) ? max : value;
    }
};

#endif