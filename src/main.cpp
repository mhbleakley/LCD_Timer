#include <Arduino.h>
#include <TimerStopwatch.h>

TimerStopwatch ts;

void setup()
{
    Serial.begin(9600);
    ts.init(2, 3, 4); //ss res mode

}

void loop()
{
    ts.loop();
}
