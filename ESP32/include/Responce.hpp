#ifndef __SMART_GARDEN_RESPONCE_HPP__
#define __SMART_GARDEN_RESPONCE_HPP__

#include <Arduino.h>

namespace Responce
{
	constexpr const char* InvalidNumberOfArguments = "ERROR\nInvalid number of arguments!\n";
	constexpr const char* InvalidData = "ERROR\nInvalid data!\n";
	constexpr const char* ValueOutOfRange = "ERROR\nValue out of range!\n";

	String OK()
	{
		return "OK\n";
	}
	String OK(const String& msg)
	{
		return "OK\n" + msg + '\n';
	}
	String ERROR(const String& msg)
	{
		return "ERROR\n" + msg + "\n";
	}
	String WARNING(const String& msg)
	{
		return "WARNING\n" + msg + "\n";
	}
}

#endif