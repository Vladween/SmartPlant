#include <DHT11.h>

#define DHT11_PIN 2
#define PUMP_PIN  11
#define LIGHT_PIN 12
#define SENSOR1_PIN A0
#define SENSOR2_PIN A1

class OutputPin
{
public:
  OutputPin(byte pin) : m_pin(pin)
  {
    pinMode(m_pin, OUTPUT);
  }

  void turnOn()
  {
    if(m_isOn) return;

    m_isOn = true;
    digitalWrite(m_pin, HIGH);
  }
  void turnOff()
  {
    if(!m_isOn) return;

    m_isOn = false;
    digitalWrite(m_pin, LOW);
  }
  bool isOn() const
  {
    return m_isOn;
  }
  byte index() const
  {
    return m_pin;
  }
private:
  byte m_pin;
  bool m_isOn = false;
};

class InputPin
{
public:
  InputPin(byte pin) : m_pin(pin)
  {
    pinMode(m_pin, INPUT);
  }

  uint16_t analog() const
  {
    return analogRead(m_pin);
  }
  bool isHigh() const
  {
    return digitalRead(m_pin) == HIGH;
  }
  byte index() const
  {
    return m_pin;
  }
private:
  byte m_pin;
};

class ShortTime
{
public:
  byte seconds, minutes;

  ShortTime(byte minutes_ = 0, byte seconds_ = 0) : minutes(minutes_), seconds(seconds_) {}

  bool operator==(const ShortTime& time) const
  {
    return time.seconds == seconds && time.minutes == minutes;
  }
};
class LongTime : public ShortTime
{
public:
  byte hours;

  LongTime(byte hours_ = 0, byte minutes_ = 0, byte seconds_ = 0) : hours(hours_), ShortTime(minutes_, seconds_) {}

  bool operator==(const LongTime& time) const
  {
    return ShortTime::operator==(time) && time.hours == hours;
  }
};

DHT11 dht11(DHT11_PIN);
OutputPin pump(PUMP_PIN), light(LIGHT_PIN);
InputPin sensor1(SENSOR1_PIN), sensor2(SENSOR2_PIN);

ShortTime water_time = { .minutes = 1, .seconds = 0 };
byte min_moisture = 15, max_moisture = 75;
LongTime current_time(0, 0, 0), light_on(8, 0, 0), light_off(22, 0, 0);

byte Moisture(uint16_t analog)
{
  return map(analog, 0, 1023, 100, 0);
}

void split(const String& str, String*& buf, byte& size)
{
  size = 1;
  for(byte i=0; i<str.length(); i++)
  {
    if(str[i] == ' ')
      size++;
  }

  buf = new String[size];

  String temp;
  byte index = 0;
  for(byte i=0; i<str.length(); i++)
  {
    if(str[i] == ' ')
    {
      buf[index] = temp;
      index++;
      temp = "";
      continue;
    }

    temp += str[i];
  }
  buf[size - 1] = temp;
}

void WriteERROR(const String& comment, String& responce)
{
  responce = "ERROR\n" + comment;
}
void WriteOK(const String& responce)
{
  responce = "OK";
}
void WriteOK(const String& comment, String& responce)
{
  responce = "OK\n" + comment;
}
void WriteInt(byte value, String& responce)
{
  WriteOK(String(value), responce);
}
void WriteShortTime(const ShortTime& stime, String& responce)
{
  WriteOK(String(stime.minutes) + " " + String(stime.seconds), responce);
}
void WriteLongTime(const LongTime& ltime, String& responce)
{
  WriteOK(String(ltime.hours) + " " + String(ltime.minutes) + " " + String(ltime.seconds), responce);
}
void WriteBool(bool value, String& responce)
{
  WriteOK(String(value ? ("ON") : ("OFF")), responce);
}

void ReadInt(byte& value, byte argc, String* argv, String& responce)
{
  if(argc != 3)
  {
    WriteERROR("Invalid number of arguments!", responce);
    return;
  }

  value = argv[2].toInt();

  WriteOK(responce);
}
void ReadShortTime(ShortTime& stime, byte argc, String* argv, String& responce)
{
  if(argc != 4)
  {
    WriteERROR("Invalid number of arguments!", responce);
    return;
  }

  stime.minutes = (byte)argv[2].toInt();
  stime.seconds = (byte)argv[3].toInt();

  WriteOK(responce);
}
void ReadLongTime(LongTime& ltime, byte argc, String* argv, String& responce)
{
  if(argc != 5)
  {
    WriteERROR("Invalid number of arguments!", responce);
    return;
  }

  ltime.hours   = (byte)argv[2].toInt();
  ltime.minutes = (byte)argv[3].toInt();
  ltime.seconds = (byte)argv[4].toInt();

  WriteOK(responce);
}

void HandleRequest(const String& request, String& responce)
{
  String* argv;
  byte argc;

  split(request, argv, argc);

  if(argv[0] == "set")
  {
    if(argv[1] == "water_time")
      ReadShortTime(water_time, argc, argv, responce);
    else if(argv[1] == "min_moisture")
      ReadInt(min_moisture, argc, argv, responce);
    else if(argv[1] == "max_moisture")
      ReadInt(max_moisture, argc, argv, responce);
    else if(argv[1] == "current_time")
      ReadLongTime(current_time, argc, argv, responce);
    else if(argv[1] == "light_on")
      ReadLongTime(light_on, argc, argv, responce);
    else if(argv[1] == "light_off")
      ReadLongTime(light_off, argc, argv, responce);
    else
      WriteERROR("Unknown variable!", responce);
  }
  else if(argv[0] == "get")
  {
    if(argc != 2)
    {
      WriteERROR("Invalid number of arguments!", responce);
      goto Send;
    }

    if(argv[1] == "water_time")
      WriteShortTime(water_time, responce);
    else if(argv[1] == "min_moisture")
      WriteInt(min_moisture, responce);
    else if(argv[1] == "max_moisture")
      WriteInt(max_moisture, responce);
    else if(argv[1] == "current_time")
      WriteLongTime(current_time, responce);
    else if(argv[1] == "light_on")
      WriteLongTime(light_on, responce);
    else if(argv[1] == "light_off")
      WriteLongTime(light_off, responce);
    else if(argv[1] == "moisture_1")
      WriteInt(Moisture(sensor1.analog()), responce);
    else if(argv[1] == "moisture_2")
      WriteInt(Moisture(sensor2.analog()), responce);
    else if(argv[1] == "temperature")
      WriteInt(dht11.readTemperature(), responce);
    else if(argv[1] == "humidity")
      WriteInt(dht11.readHumidity(), responce);
    else if(argv[1] == "pump")
      WriteBool(pump.isOn(), responce);
    else if(argv[1] == "light")
      WriteBool(light.isOn(), responce);
    else
      WriteERROR("Unknown variable!", responce);
  }
  else
  {
    WriteERROR("Unknown command!", responce);
  }

Send:

  delete[] argv;
}

void setup()
{
  Serial.begin(9600);
}

unsigned long lastWatered = 0;
unsigned long lastTick = 0;

void loop()
{
  if(Serial.available())
  {
    String responce;
    HandleRequest(Serial.readString(), responce);
    Serial.println(responce);
  }

  byte avg = ((unsigned long)Moisture(sensor1.analog()) + (unsigned long)Moisture(sensor2.analog())) / 2;
  if(avg < min_moisture && !pump.isOn())
  {
    pump.turnOn();
    lastWatered = millis();
  }

  if((avg > max_moisture || millis() - lastWatered >= (unsigned long)water_time.minutes * 60u + (unsigned long)water_time.seconds) && pump.isOn())
    pump.turnOff();

  if(millis() - lastTick >= 1000)
  {
    current_time.seconds++;
    if(current_time.seconds >= 60)
    {
      current_time.seconds = 0;
      current_time.minutes++;
      if(current_time.minutes >= 60)
      {
        current_time.minutes = 0;
        current_time.hours++;
        if(current_time.hours >= 24)
          current_time.hours = 0;
      }
    }

    if(current_time == light_on)
      light.turnOn();
    else if(current_time == light_off)
      light.turnOff();
    
    lastTick = millis();
  }
}
