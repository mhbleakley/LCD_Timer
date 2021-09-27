#include <LiquidCrystal_I2C.h>
#include <LinearButton.h>
#include <Arduino.h>
#include <Wire.h>

class TimerStopwatch
{
private:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
    unsigned long refMillis;
    unsigned long difference;
    unsigned long pauseStart;
    unsigned long pauseStop;
    unsigned long pauseTime;
    bool sDown, rDown, mDown;
    enum STATE
    {
        TIMER,
        STOPWATCH,
        COUNTDOWN,
        COUNTUP,
        TPAUSE,
        SPAUSE,
        ALARM
    } state;
    

public:
    void init(int startStop, int reset, int mode);
    void stateMachine();
    void checkSTSP(int nextState);
    void checkRES(int nextState);
    void checkMODE(int nextState);
    void stopwatch();
    void pause(unsigned long t);
    void timer(int h, int m, int s);
    void hms(int h, int m, int s);
    void flash(int interval);
    void cycle(int interval);
    void loop();
};