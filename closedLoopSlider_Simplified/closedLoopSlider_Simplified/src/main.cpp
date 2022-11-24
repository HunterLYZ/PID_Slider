#include "Arduino.h"
#include "ledcStepper.h"
#include "PID.h"
#include "HC-SR04.h"

StepperObject myStepper(700, 10, 9, 0);
PIDobject myPID(1, 0.1, 0.05, 1, 10, 4);
UltrasonicSensor mySensor(32, 35, 100);
int tarSpeed, stepperVal;
void setup()
{
    mySensor.loop();
    mySensor.loop();
    tarSpeed = 100;
    stepperVal = 0;
}

void loop()
{
    mySensor.loop();
    if (mySensor.available())
    {
        myPID.clac(tarSpeed, mySensor.get(Distance), &stepperVal);
        myStepper.runContinuous(stepperVal);
    }
}