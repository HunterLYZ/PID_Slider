#include <Arduino.h>

#ifndef HC_SR04_H
#define HC_SR04_H
typedef enum
{
  Distance = '0',
  Distance_Centimeter = '1',
  Distance_Integer = '2',
  Distance_Derivative = '3',
} ReturnType;

class UltrasonicSensor
{
public:
  UltrasonicSensor(uint8_t echoPin, uint8_t trigPin, uint8_t freqHz)
  {
    this->echoPin = echoPin;
    this->trigPin = trigPin;
    this->loopT = 1000 / freqHz;
    checkVal();
  }

  void loop()
  {
    now = millis();
    if (now - timestamp > loopT)
    {
      timestamp = now;
      update();
    }
  }
  int get(ReturnType type)
  {
    switch (type)
    {
    case Distance:
      return distanceLog[0];
    case Distance_Centimeter:
      return distanceLog[0] / 58;
    default:
      Serial.println("Undefined ReturnType");
    }
  }
  bool available()
  {
    if (gotNew)
    {
      gotNew = false;
      return true;
    }
    return false;
  }

private:
  uint8_t echoPin, trigPin, loopT;
  unsigned long now;
  unsigned long timestamp = 0;
  unsigned int distanceLog[32];
  bool gotNew = false;
  void update()
  {
    for (int i = 32; i > 0; i--)
      distanceLog[i] = distanceLog[i - 1];
    digitalWrite(trigPin, LOW);              // 保持发出超声波信号接口低电平
    distanceLog[0] = pulseIn(echoPin, HIGH); // 读出脉冲时间
    digitalWrite(trigPin, HIGH);             // 使发出发出超声波信号接口高电平至少10μs
  }
  void checkVal()
  {
    if (loopT >= 1000)
    {
      Serial.print("HC-SR04false");
      while (1)
        ;
    }
  }
};

#endif