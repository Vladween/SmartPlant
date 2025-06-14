#ifndef __SMART_PLANT_SHORT_TIME_HPP__
#define __SMART_PLANT_SHORT_TIME_HPP__

#include "Variable.hpp"
#include "Utilities.hpp"

class ShortTime : public Variable
{
public:
	ShortTime(const char* name, bool isMutable, uint8_t minutes = 0, uint8_t seconds = 0)
        : Variable(name, isMutable), m_minutes(clampValue(minutes)), m_seconds(clampValue(seconds))
    {}

    void setMinutes(uint8_t minutes)
	{
        m_minutes = clampValue(minutes);
    }
    void setSeconds(uint8_t seconds)
	{
        m_seconds = clampValue(seconds);
    }

    uint8_t getMinutes() const
	{
		return m_minutes;
	}
    uint8_t getSeconds() const
	{
		return m_seconds;
	}
    uint16_t getTotal() const
	{
        return static_cast<uint16_t>(m_minutes) * 60 + 
               static_cast<uint16_t>(m_seconds); 
    }

    ShortTime& operator++()
    {
        if(m_seconds == 59)
        {
            setSeconds(0);
			if(m_minutes == 59)
				setMinutes(0);
			else 
				setMinutes(m_minutes + 1);
        }
        else
			setSeconds(m_seconds + 1);
        return *this;
    }
    ShortTime& operator+=(uint8_t seconds)
    {
        uint16_t sum = m_seconds + seconds;
        if(sum >= 60)
            setMinutes(m_minutes + sum / 60);
        setSeconds(sum % 60);
		return *this;
    }

    bool operator==(const ShortTime& other) const
	{
        return m_minutes == other.m_minutes && 
               m_seconds == other.m_seconds;
    }

    bool operator!=(const ShortTime& other) const
	{
        return !(*this == other);
    }
    
    bool operator<(const ShortTime& other) const
	{
        if (m_minutes != other.m_minutes)
			return m_minutes < other.m_minutes;
        return m_seconds < other.m_seconds;
    }
    
    bool operator<=(const ShortTime& other) const
	{
        return !(other < *this);
    }
    
    bool operator>(const ShortTime& other) const
	{
        return other < *this;
    }
    
    bool operator>=(const ShortTime& other) const
	{
        return !(*this < other);
    }
protected:
	String set(uint8_t argc, const String* argv) override
	{
		if(argc != 2)
			return Responce::InvalidNumberOfArguments;

		long minutes = toInt(argv[0]), seconds = toInt(argv[1]);
		if(minutes == -1 || seconds == -1)
			return Responce::InvalidData;
		if(minutes >= 60 || seconds >= 60)
			return Responce::ValueOutOfRange;
		
		m_minutes = minutes;
		m_seconds = seconds;
		return Responce::OK(get());
	}
    String get() const override
    {
        return (String)m_minutes + ':' + (String)m_seconds;
    }
    String get_json() const override
    {
        return "[ " + (String)m_minutes + ", " + (String)m_seconds + " ]";
    }
private:
    static constexpr uint8_t clampValue(uint8_t value)
	{
        return (value >= 60) ? 59 : value;
    }

	uint8_t m_seconds;
    uint8_t m_minutes;
};

#endif