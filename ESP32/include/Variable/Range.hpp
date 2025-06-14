#ifndef __SMART_PLANT_RANGE_HPP__
#define __SMART_PLANT_RANGE_HPP__

#include "Variable.hpp"
#include "Utilities.hpp"

class Range : public Variable
{
public:
	Range(const char* name, bool isMutable, uint8_t min = 0, uint8_t max = 100)
		: Variable(name, isMutable), m_min((min > 100) ? 100 : (min > max) ? max : min), m_max((max > 100) ? 100 : (min > max) ? min : max)
	{}

	bool set(uint8_t min, uint8_t max)
	{
		if(min > 100)
			min = 100;
		if(max > 100)
			max = 100;

		if(min > max)
		{
			m_min = max;
			m_max = min;
			return false;
		}

		m_min = min;
		m_max = max;
		return true;
	}
	uint8_t min() const
	{
		return m_min;
	}
	uint8_t max() const
	{
		return m_max;
	}
protected:
	String set(uint8_t argc, const String* argv) override
	{
		if(argc != 2)
			return Responce::InvalidNumberOfArguments;

		long min = toInt(argv[0]), max = toInt(argv[1]);
		if(min == -1 || max == -1)
			return Responce::InvalidData;
		if(min > 100 || max > 100)
			return Responce::ValueOutOfRange;
		
		if(!set(min, max))
			return Responce::WARNING("Values swapped");
		return Responce::OK(get());
	}
	String get() const override
	{
		return '[' + (String)m_min + "; " + (String)m_max + ']';
	}
	String get_json() const override
	{
		return "[ " + (String)m_min + ", " + (String)m_max + " ]";
	}
private:
	uint8_t m_min, m_max;
};

#endif