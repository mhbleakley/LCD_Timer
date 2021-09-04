#include <Arduino.h>
#include <TimerStopwatch.h>

TimerStopwatch ts;
// LinearButton lb;
int led = 13;

void setup()
{
    Serial.begin(9600);
    ts.init(2, 3, 4);
}

void loop()
{
    ts.stateMachine();
}
