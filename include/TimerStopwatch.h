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
        ALARM,
        TIMEREDIT
    } state;

public:
    void init(int startStop, int reset, int mode);
    void currentMode(bool timer);
    void handleSTSP(bool timer);
    void handleRES(bool timer);
    void handleMODE(bool timer);
    void stateMachine();
    void stopwatch();
    void pause(unsigned long t);
    void timer(int h, int m, int s);
    void hms(int h, int m, int s);
    void flash(int interval);
    void cycle(int interval);

};