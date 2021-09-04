#include <TimerStopwatch.h>
#include <Timer.h>

LinearButton STSP;
LinearButton RES;
LinearButton MODE;
Timer ignoreInput(500);

void TimerStopwatch::init(int startStop, int reset, int mode)
{
    lcd.init();
    lcd.backlight();
    Serial.begin(9600);
    STSP.init(startStop);
    RES.init(reset);
    MODE.init(mode);
    state = TIMER;
    sDown = false;
    rDown = false;
    mDown = false;
}

void TimerStopwatch::currentMode(bool timer)
{
    lcd.setCursor(0, 0);
    lcd.print("MODE:");
    lcd.setCursor(11, 0);
    if (timer)
        lcd.print("TIMER");
    else
    {
        lcd.setCursor(7, 0);
        lcd.print("STOPWATCH");
    }
    lcd.setCursor(0, 1);
    lcd.print("TIME:");
}

void TimerStopwatch::stateMachine()
{
    STSP.loop();
    RES.loop();
    MODE.loop();
    switch (state)
    {
    case TIMER:
        currentMode(true);
        handleMODE(true);
        handleSTSP(true);
        hms(0, 5, 0);
        break;
    case STOPWATCH:
        currentMode(false);
        handleMODE(false);
        handleSTSP(false);
        hms(0, 0, 0);
        break;
    case COUNTDOWN:
        currentMode(true);
        handleRES(true);
        handleSTSP(true);
        timer(0, 5, 0);
        break;
    case COUNTUP:
        currentMode(false);
        handleRES(false);
        handleSTSP(false);
        stopwatch();
        break;
    case TPAUSE:
        currentMode(true);
        handleRES(true);
        handleSTSP(true);
        break;
    case SPAUSE:
        currentMode(false);
        handleRES(false);
        handleSTSP(false);
        pause(refMillis);
        break;
    case ALARM:
        break;
    case TIMEREDIT:
        break;
    }
}

void TimerStopwatch::handleSTSP(bool timer)
{
    if (STSP.getState() == 1)
        sDown = true;
    if (sDown && STSP.getState() == 0)
    {
        if (timer)
        {
            if (state == COUNTDOWN)
                state = TPAUSE;
            else
                state = COUNTDOWN;
        }
        else
        {
            if (state == COUNTUP)
            {
                state = SPAUSE;
                pauseStart = millis();
            }
            else
            {
                pauseStop = millis();
                pauseTime = pauseStop - pauseStart;
                difference = millis() - pauseTime;
                state = COUNTUP;
            }
        }
        sDown = false;
        lcd.clear();
    }
}

void TimerStopwatch::handleRES(bool timer)
{
    if (RES.getState() == 1)
        rDown = true;
    if (rDown && RES.getState() == 0)
    {
        if (timer)
            state = TIMER;
        else
            state = STOPWATCH;
        rDown = false;
        lcd.clear();
    }
}

void TimerStopwatch::handleMODE(bool timer)
{
    if (MODE.getState() == 1)
        mDown = true;
    if (MODE.getState() == 2 && timer)
    {
        state = TIMEREDIT;
        mDown = false;
        lcd.clear();
    }
    if (mDown && MODE.getState() == 0)
    {
        if (timer)
            state = STOPWATCH;
        else
            state = TIMER;
        mDown = false;
        lcd.clear();
    }
}

void TimerStopwatch::stopwatch()
{
    lcd.setCursor(8, 1);
    unsigned long currentMillis = millis() - difference;
    refMillis = currentMillis;
    unsigned long seconds = currentMillis / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    currentMillis %= 1000;
    seconds %= 60;
    minutes %= 60;
    hours %= 24;
    hms(hours, minutes, seconds);
}

void TimerStopwatch::timer(int h, int m, int s)
{
    lcd.setCursor(8, 1);
    unsigned long currentMillis = millis();
    unsigned long seconds = currentMillis / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    currentMillis %= 1000;
    seconds %= 60;
    minutes %= 60;
    hours %= 24;
    hms(hours, minutes, seconds);
}

void TimerStopwatch::pause(unsigned long t)
{
    lcd.setCursor(8, 1);
    unsigned long seconds = t / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    t %= 1000;
    seconds %= 60;
    minutes %= 60;
    hours %= 24;
    hms(hours, minutes, seconds);
}

void TimerStopwatch::hms(int h, int m, int s)
{
    lcd.setCursor(8, 1);
    if (h < 10)
        lcd.print(0);
    lcd.print(h);
    lcd.print(":");
    if (m < 10)
        lcd.print(0);
    lcd.print(m);
    lcd.print(":");
    if (s < 10)
        lcd.print(0);
    lcd.print(s);
}

void TimerStopwatch::flash(int interval)
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            lcd.setCursor(i, j);
            lcd.print((char)255);
        }
    }
    delay(interval);
    lcd.clear();
    delay(interval);
}

void TimerStopwatch::cycle(int interval)
{
    for (int i = 0; i < 16; i++)
    {
        lcd.clear();
        lcd.setCursor(i, 0);
        lcd.print((char)255);
        delay(interval);
    }
    for (int i = 0; i < 17; i++)
    {
        lcd.clear();
        lcd.setCursor(16 - i, 1);
        lcd.print((char)255);
        delay(interval);
    }
}