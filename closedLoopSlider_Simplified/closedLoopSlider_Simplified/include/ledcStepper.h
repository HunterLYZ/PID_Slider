#ifndef ARDUINO
#include "Arduino.h"
#endif

#ifndef STEPPER_SERIAL
#define STEPPER_SERIAL Serial
#endif

class StepperObject
{
private:
    int _stepFreq;    // dipPin输出PWM最大频率(Hz)
    uint8_t _dirPin;  // 方向引脚
    uint8_t _stepPin; // 步进引脚
    // uint8_t _enPin;  // 使能引脚
    uint8_t _ledc; // stepPin绑定到LEDC_CHANNELS

public:
    StepperObject(int maxStepFreq = 700, uint8_t dirPin = 10, uint8_t stepPin = 9, uint8_t ledc = 0)
    {
        _stepFreq = maxStepFreq;
        _dirPin = dirPin;
        _stepPin = stepPin;
        _ledc = ledc;
        pinMode(_stepPin, OUTPUT); //定义电机步进接口
        pinMode(_dirPin, OUTPUT);  //定义电机转动方向接口
        // pinMode(_enPin, OUTPUT);  //使能引脚
        ledcWriteTone(_ledc, 1); //电机怠速
        ledcAttachPin(_stepPin, _ledc);
        STEPPER_SERIAL.printf("Stepper begin:maxStepFreq(Hz):%5d, dirPin:%5d, stepPin:%5d, ledc:%5d", _stepFreq, _dirPin, _stepPin);
    }
    int runContinuous(int freq = 1)
    {
        if (abs(freq) <= _stepFreq)
        {
            if (freq > 0)
            {
                digitalWrite(_dirPin, HIGH); // dirpin为低电平电机顺时针转动，高电平电机逆时针转动
                ledcWriteTone(_ledc, freq);
            }
            else if (freq == 0)
            {
                ledcWriteTone(_ledc, 1);
            }
            else
            {
                digitalWrite(_dirPin, LOW); // dirpin为低电平电机顺时针转动，高电平电机逆时针转动
                ledcWriteTone(_ledc, -freq);
            }
            STEPPER_SERIAL.printf("runningFreq =%4d\n", freq); //打印此刻的距离
            STEPPER_SERIAL.println();
            return freq;
        }
        return 0;
    }
};
