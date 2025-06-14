#ifndef __SMART_GARDEN_UTILITIES_HPP__
#define __SMART_GARDEN_UTILITIES_HPP__

#include <Arduino.h>

uint8_t toInt(char symbol)
{
	if(symbol < 48 || symbol > 57) return 10;
	return symbol - 48;
}
long toInt(const String& str)
{
	long result = 0;
	for(uint8_t i=0; i<str.length(); i++)
	{
		uint8_t num = toInt(str[i]);
		if(num == 10)
			return -1;
		result = result * 10 + num;
	}
	return result;
}

#endif