#include <LiquidCrystal_I2C.h>
#include <LinearButton.h>
#include <Arduino.h>
#include <Wire.h>

class TimerStopwatch
{
private:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
    unsigned long now;
    unsigned long then;
    long seconds;
    long defaultTimer = 60;
    bool sDown, rDown, mDown;
    enum STATE
    {
        TIMER,
        STOPWATCH,
        COUNTDOWN,
        COUNTUP,
        TPAUSE,
        SPAUSE,
        ALARM,
        ZERO
    } state;
    

public:
    void init(int startStop, int reset, int mode);
    void stateMachine();
    void checkSTSP(int nextState);
    void checkRES(int nextState);
    void checkMODE(int nextState);
    void stopwatch();
    void timer();
    void pause(unsigned long t);
    void hms(int h, int m, int s);
    void flash(int interval);
    void cycle(int interval);
    void loop();
};