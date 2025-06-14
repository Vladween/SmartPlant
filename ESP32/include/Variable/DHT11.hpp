#ifndef __SMART_PLANT_DHT11_HPP__
#define __SMART_PLANT_DHT11_HPP__

// Необходимые заголовки
#include "Variable.hpp"
#include <DHT11.h>

/// @brief Класс переменной, отслеживающей состояние модуля DHT11
class DHT11Status : public Variable
{
public:
    /// @brief Стандартный конструктор
    /// @param name название переменной
    /// @param pin пин DHT11
    DHT11Status(const char* name, uint8_t pin)
        : Variable(name, false), m_dht(pin)
    {
        m_dht.setDelay(0);
    }
    
    /// @brief Обновляет показания температуры и влажности воздуха
    void update() const
    {
        int temp, humd;
        m_dht.readTemperatureHumidity(temp, humd);
        m_temp = temp;
        m_humd = humd;
    }
    /// @brief Возврещает температуру во время последнего обновления датчика
    /// @return Температуру в °C
    int8_t temperature() const
    {
        return m_temp;
    }

    /// @brief Возврещает влажность воздуха во время последнего обновления датчика
    /// @return Влажность воздуха в %
    uint8_t humidity() const
    {
        return m_humd;
    }
protected:
    /// @brief Переменная этого типа неизменяемая по умолчанию, функция ничего не делает
    /// @param argc количество аргументов
    /// @param argv адрес массива аргументов
    /// @return Пустую строку
    String set(uint8_t argc, const String* argv) { return {}; }

    /// @brief Обновляет показания с дачтика и возвращает строку в формате "<температура>°C, <влажность воздуха>%"
    /// @return Строку - состояние датчика
    String get() const override
    {
        update();
        return (String)m_temp + "°C, " + (String)m_humd + "%";
    }

    /// @brief Обновляет показания с датчика и возвращает JSON переменную в формате "[ <температура>, <влажность воздуха> ]"
    /// @return JSON массив - состояние датчика
    String get_json() const override
    {
        update();
        return "[ " + (String)m_temp + ", " + (String)m_humd + " ]";
    } 
private:
    mutable int8_t m_temp = 0;  // Температура (°C)
    mutable uint8_t m_humd = 0; // Влажность воздуха (%)
    mutable DHT11 m_dht;        // Объект для взаимодействия с датчиком
};

#endif