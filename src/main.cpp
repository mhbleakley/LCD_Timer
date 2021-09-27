#include <Arduino.h>
#include <TimerStopwatch.h>

TimerStopwatch ts;
LinearButton lb;
int led = 13;

void setup()
{
    Serial.begin(9600);
    // lb.init(2);
    ts.init(2, 3, 4); //ss res mode

}

void loop()
{
    ts.stateMachine();
    // lb.loop();
    // Serial.println(lb.getState());
}
