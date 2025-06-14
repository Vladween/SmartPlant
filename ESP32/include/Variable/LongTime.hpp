#ifndef __SMART_PLANT_LONG_TIME_HPP__
#define __SMART_PLANT_LONG_TIME_HPP__

// Необходимые заголовки
#include "ShortTime.hpp"

/// @brief Переменная, обозначающая длинное время (чч:мм:сс)
class LongTime : public ShortTime
{
public:
	/// @brief Стандартный конструктор
	/// @param name название переменной
	/// @param isMutable изменяемая ли переменная?
	/// @param hours количество часов (не больше 23, 0 по умолчанию)
	/// @param minutes количество минут (не больше 59, 0 по умолчанию)
	/// @param seconds количество секунд (не больше 59б 0 по умолчанию)
	LongTime(const char* name, bool isMutable, uint8_t hours = 0, uint8_t minutes = 0, uint8_t seconds = 0)
		: ShortTime(name, isMutable, minutes, seconds), m_hours(clampHours(hours)) 
	{}
	
	/// @brief Изменяет значение часов. Если аргумент >23, то значение устанавливается как 23
	/// @param hours количество часов (не больше 23)
	void setHours(uint8_t hours)
	{
		m_hours = clampHours(hours);
	}
	
	/// @return Количество часов
	uint8_t getHours() const
	{
		return m_hours;
	}
	
	/// @return Суммарное время в секундах (часы * 3600 + минуты * 60 + секунды)
	uint32_t getTotal() const
	{
		return static_cast<uint32_t>(m_hours) * 3600 + ShortTime::getTotal();
	}
	
    LongTime& operator++()
    {
        ShortTime::operator++();
        if(getSeconds() == 0 && getMinutes() == 0)
		{
			if(m_hours == 23)
				setHours(0);
			else
	         	setHours(m_hours + 1);
		}
        return *this;
    }

	bool operator==(const LongTime& other) const
	{
		return m_hours == other.m_hours && 
			   static_cast<const ShortTime&>(*this) == 
			   static_cast<const ShortTime&>(other);
	}
	
	bool operator!=(const LongTime& other) const
	{
		return !(*this == other);
	}
	
	bool operator<(const LongTime& other) const
	{
		if (m_hours != other.m_hours) 
			return m_hours < other.m_hours;
		return static_cast<const ShortTime&>(*this) < 
			   static_cast<const ShortTime&>(other);
	}
	
	bool operator<=(const LongTime& other) const
	{
		return !(other < *this);
	}
	
	bool operator>(const LongTime& other) const
	{
		return other < *this;
	}
	
	bool operator>=(const LongTime& other) const
	{
		return !(*this < other);
	}
protected:
	String set(uint8_t argc, const String* argv) override
	{
		if(argc != 3)
			return Responce::InvalidNumberOfArguments;
		
		long hours = toInt(argv[0]), minutes = toInt(argv[1]), seconds = toInt(argv[2]);
		if(hours == -1 || minutes == -1 || seconds == -1)
			return Responce::InvalidData;
		if(hours >= 24 || minutes >= 60 || seconds >= 60)
			return Responce::ValueOutOfRange;

		m_hours = hours;
		setMinutes(minutes);
		setSeconds(seconds);
		return Responce::OK(get());
	}
	String get() const override
	{
		return (String)getHours() + ':' + (String)getMinutes() + ':' + (String)getSeconds();
	}
	String get_json() const override
	{
		return "[ " + (String)getHours() + ", " + (String)getMinutes() + ", " + (String)getSeconds() + " ]"; 
	}
private:
	static constexpr uint8_t clampHours(uint8_t h)
	{
		return h >= 24 ? 23 : h;
	}
			
	uint8_t m_hours;
};

#endif