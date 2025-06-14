#ifndef __SMART_PLANT_INTEGER_HPP__
#define __SMART_PLANT_INTEGER_HPP__

// Необходимые заголовки
#include "Variable.hpp"
#include "Utilities.hpp"

/// @brief Класс целочисленной переменной, обёртка типа uint8_t
/// @tparam limit ограничитель значения. Выше этого значения переменная не может быть задана функцией set
template<uint8_t limit>
class Integer : public Variable
{
public:	
	/// @brief Стандартный конструктор
	/// @param name название переменной
	/// @param isMutable изменяемая ли переменная?
	/// @param value значение переменной (0 по умолчанию)
	Integer(const char* name, bool isMutable, uint8_t value = 0) 
        : Variable(name, isMutable), m_value(value) {}

	/// @return l-value ссылку на значение переменной
	uint8_t& value()
	{
		return m_value;
	}
	/// @return Неизменяемое значение переменной
	uint8_t value() const
	{
		return m_value;
	}
protected:
	/// @brief Функция для изменения значения переменной через CLI интерфей. Принимает 1 аргумент - целое число, не превосходящее по значению параметр шаблона limit 
	/// @param argc количество аргументов. Если значение не равно 1, то функция возвращает Responce::InvalidNumberOfArguments
	/// @param argv адрес массива аргументов
	/// @return Результат в формате "<статус>\n<сообщение>\n". При статусе OK сообщение - дублированное значение переменной, иначе комментарий к ошибке
	String set(uint8_t argc, const String* argv) override
	{
		if(argc != 1)
			return Responce::InvalidNumberOfArguments;

		long input = toInt(argv[0]);
		if(input == -1)
			return Responce::InvalidData;
		if(input > limit)
			return Responce::ValueOutOfRange;

		m_value = (uint8_t)input;
		return Responce::OK(get());
	}

	/// @brief Возвращает значение переменной в качестве строки
	/// @return Строку - значение переменной
	String get() const override
	{
		return (String)m_value;
	}

	/// @brief Возвращает значение переменной в качестве JSON объекта
	/// @return JSON объект - значение переменной
	String get_json() const override
	{
		return (String)m_value;
	}
private:
	uint8_t m_value; // Значение переменной
};

/// @brief Целочисленная переменная, обозначающая процент
class Percent : public Integer<100>
{
public:
	using Integer<100>::Integer; // Тот же конструктор, что и у Integer<100>
protected:
	/// Возвращает значение переменной в формате "<значение>%"
	String get() const override
	{
		return (String)value() + '%';
	}
};

#endif