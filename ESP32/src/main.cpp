// Необходимые заголовки
#include <Arduino.h>
#include <DHT11.h>
#include "Command.hpp"
#include "Variable.hpp"

// Пины
#define PUMP_PIN         0 // Пин Помпы (GPIO0)
#define LIGHT_PIN        1 // Пин УФ лампы (GPIO1)
#define DHT11_PIN        2 // Пин DHT11 (GPIO2)
#define WATER_LEVEL_PIN A1 // Пин Датчика уровня воды (GPIO3)
#define MOISTURE_PIN    A2 // Пин Датчика влажности почвы (GPIO4)

LongTime  light_on       ("light_on",       1, 8, 0, 0); // Время включения света (чч:мм:сс)
LongTime  light_off      ("light_off",      1, 20, 0, 0); // Время выключения света (чч:мм:сс)
Range     moisture_range ("moisture_range", 1, 25, 75); // Позволенная влажность почвы (промежуток [min; max])
LongTime  current_time   ("current_time",   1);
ShortTime water_time     ("water_time",     1);

DHT11Status    dht11       ("dht11",     DHT11_PIN);
OutputPin      pump        ("pump",   0, PUMP_PIN);
OutputPin      light       ("light",  0, LIGHT_PIN);
ReservoirState reservoir   ("reservoir", WATER_LEVEL_PIN);
Moisture       moisture    ("moisture",  MOISTURE_PIN);

const Command Commands[] = {
	Command("set", [](uint8_t argc, const String* argv) -> String
		{
			if(argc < 2)
				return Responce::InvalidNumberOfArguments;
			return Variable::Set(argv[0].c_str(), argc - 1, argv + 1);
		}
	),
	Command("get", [](uint8_t argc, const String* argv) -> String
		{
			if(argc != 1)
				return Responce::InvalidNumberOfArguments;
			return Variable::Get(argv[0].c_str());
		}
	),
	Command("log", [](uint8_t argc, const String* argv) -> String
		{
			if(argc != 0)
				return Responce::InvalidNumberOfArguments;
			return Responce::OK(Variable::AsJSON());
		}
	)
};
constexpr uint8_t NumberOfCommands = sizeof(Commands) / sizeof(Commands[0]);

uint32_t last_watered = millis();
uint32_t last_tick = millis();

void setup()
{
	Serial.begin(115200);
}
  
void loop()
{
	if(Serial.available())
	{
		String request = Serial.readStringUntil('\n');
		request = request.substring(0, request.length() - 1);
		Serial.print(HandleRequest(request, Commands, NumberOfCommands));
	}

	if(water_time.getTotal() != 0)
	{
		moisture.update();
		if(moisture.value() < moisture_range.min())
		{
			pump.turnOn();
			last_watered = millis();
		}
		else if(moisture.value() > moisture_range.max() || millis() - last_watered >= water_time.getTotal() * 1000)
			pump.turnOff();
	}
	else pump.turnOff();
		

	uint32_t diff = millis() - last_tick;
	if(diff >= 1000)
	{
		last_tick = millis();

		current_time += diff / 1000;
		if(current_time >= light_on && current_time < light_off)
			light.turnOn();
		else
			light.turnOff();
	}

	delay(10);
}