#ifndef __SMART_PLANT_VARIABLE_HPP__
#define __SMART_PLANT_VARIABLE_HPP__

#include <cstdint>
#include <cstring>
#include "Responce.hpp"

namespace Responce
{
	constexpr const char* UnknownVariable = "ERROR\nUnknown variable!\n";
	constexpr const char* VariableNotMutable = "ERROR\nVariable is read-only!\n";
}

class Variable
{
public:
    static Variable* Find(const char* name)
    {
        for(Variable* p = _Head; p; p = p->m_next)
        {
            if(strcmp(p->m_name, name) == 0)
                return p;
        }
        return 0;
    }
	static String Set(const char* name, uint8_t argc, const String* argv)
	{
		Variable* v = Find(name);
		if(!v)
			return Responce::UnknownVariable;
		if(!v->m_isMutable)
			return Responce::VariableNotMutable;
		return v->set(argc, argv);
	}
	static String Get(const char* name)
	{
		Variable* v = Find(name);
		if(!v) 
			return Responce::UnknownVariable;
		return Responce::OK(v->get());
	}
	static String AsJSON()
	{
		String responce = "{\n\t\"";
		for(Variable* v=_Head; v; v = v->m_next)
		{
			responce += v->m_name + (String)"\": " + v->get_json();
			if(!v->m_next)
				responce += "\n}";
			else	
				responce += ",\n\t\"";
		}
		return responce;
	}

	Variable(const char* name, bool isMutable) : m_name(name), m_isMutable(isMutable), m_next(0)
	{
		if(!_Head)
		{
			_Head = this;
			return;
		}

		Variable* p = _Head;
		for(; p->m_next; p = p->m_next) {}
		p->m_next = this;
	}
	virtual ~Variable()
	{
		if(_Head == this)
		{
			_Head = _Head->m_next;
			return;
		}

		for(Variable *p = _Head->m_next, *p_prev = _Head; p; p_prev = p, p = p->m_next)
		{
			if(p == this)
			{
				p_prev->m_next = p->m_next;
				return;
			}
		}
	}

    const char* name() const
    {
        return m_name;
    }
	bool isMutable() const
	{
		return m_isMutable;
	}	
protected:
	virtual String set(uint8_t argc, const String* argv) = 0;
	virtual String get() const = 0;
	virtual String get_json() const = 0;
private:
	const char* m_name;
	bool m_isMutable = false;
	Variable* m_next;

	static Variable* _Head;
};
Variable* Variable::_Head = 0;

#endif