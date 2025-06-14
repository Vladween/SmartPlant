#ifndef __SMART_PLANT_COMMAND_HPP__
#define __SMART_PLANT_COMMAND_HPP__

#include <Arduino.h>
#include "Responce.hpp"

class Command
{
public:
	typedef String(*Action)(uint8_t, const String*);

	static void Parse(const String& request, uint8_t& argc, String*& argv)
	{
		argc = 1;

		bool prevSkipped = false;
		for(uint8_t i=0; i<request.length(); i++)
		{
			if(request[i] == ' ')
			{
				if(prevSkipped)
					continue;

				++argc;
				prevSkipped = true;
			}
			else prevSkipped = false;
		}
		if(prevSkipped)
			--argc;

		argv = new String[argc];

		uint8_t index = 0;
		String temp = "";

		for(uint8_t i=0; i<request.length(); i++)
		{
			if(request[i] == ' ')
			{
				if(temp.isEmpty())
					continue;

				argv[index] = temp;
				++index;
				temp.clear();
			}
			else temp += request[i];
		}

		if(!temp.isEmpty())
			argv[argc - 1] = temp;
	}

	constexpr Command(const char* name, Action action)
		: m_name(name), m_action(action) {}

	String execute(uint8_t argc, const String* argv) const
	{
		return m_action(argc, argv);
	}
	const char* name() const
	{
		return m_name;
	}
	Action action() const
	{
		return m_action;
	}
private:
	const char* m_name;
	Action m_action;
};

String HandleRequest(const String& request, const Command Commands[], uint8_t NumberOfCommands)
{
	if(request.isEmpty())
		return Responce::ERROR("Empty command!");

	uint8_t argc;
	String* argv;
	Command::Parse(request, argc, argv);

	String responce;

	const Command* cmd = 0;
	for(uint8_t i=0; i<NumberOfCommands; i++)
	{
		if(strcmp(Commands[i].name(), argv[0].c_str()) == 0)
		{
			cmd = &Commands[i];
			break;
		}
	}

	if(!cmd)
		responce = Responce::ERROR("Unknown command!");
	else
		responce = cmd->execute(argc - 1, (argc == 1) ? 0 : argv + 1);

	delete[] argv;
	return responce;
}

#endif