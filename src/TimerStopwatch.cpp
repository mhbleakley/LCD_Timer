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

void TimerStopwatch::stateMachine()
{
    STSP.loop();
    RES.loop();
    MODE.loop();
    switch (state)
    {
    case TIMER:
        seconds = defaultTimer;
        hms((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60);
        checkSTSP(COUNTDOWN);
        checkMODE(STOPWATCH);
        break;
    case STOPWATCH:
        seconds = -1;
        hms(0, 0, 0);
        checkSTSP(COUNTUP);
        checkMODE(TIMER);
        break;
    case COUNTDOWN:
        if (seconds == 0)
        {
            state = ALARM;
            lcd.clear();
            break;
        }
        timer();
        checkSTSP(TPAUSE);
        checkRES(TIMER);
        break;
    case COUNTUP:
        stopwatch();
        checkSTSP(SPAUSE);
        checkRES(STOPWATCH);
        break;
    case TPAUSE:
        hms((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60);
        checkSTSP(COUNTDOWN);
        checkRES(TIMER);
        break;
    case SPAUSE:
        hms((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60);
        checkSTSP(COUNTUP);
        checkRES(STOPWATCH);
        break;
    case ALARM:
        for (int i = 0; i < 5; i++)
        {
            flash(250);
        }
        state = ZERO;
        break;
    case ZERO:
        hms((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60);
        checkSTSP(TIMER);
        checkRES(TIMER);
        break;
    }
}

void TimerStopwatch::checkSTSP(int nextState)
{
    if (STSP.isPressed())
    {
        sDown = true;
    }
    if (STSP.isUnpressed() && sDown)
    {
        state = (STATE)nextState;
        sDown = false;
        lcd.clear();
    }
}

void TimerStopwatch::checkRES(int nextState)
{
    if (RES.isPressed())
    {
        rDown = true;
    }
    if (RES.isUnpressed() && rDown)
    {
        state = (STATE)nextState;
        rDown = false;
        lcd.clear();
    }
}

void TimerStopwatch::checkMODE(int nextState)
{
    if (MODE.isPressed())
    {
        mDown = true;
    }
    if (MODE.isUnpressed() && mDown)
    {
        state = (STATE)nextState;
        mDown = false;
        lcd.clear();
    }
}

void TimerStopwatch::stopwatch()
{
    lcd.setCursor(8, 1);
    now = millis();
    if (now - then > 1000)
    {
        seconds++;
        then = millis();
    }
    unsigned long displaySeconds = seconds;
    unsigned long minutes = displaySeconds / 60;
    unsigned long hours = minutes / 60;
    displaySeconds %= 60;
    minutes %= 60;
    hours %= 24;
    hms(hours, minutes, displaySeconds);
}

void TimerStopwatch::timer()
{
    lcd.setCursor(8, 1);
    now = millis();
    if (now - then > 1000)
    {
        seconds--;
        then = millis();
    }
    unsigned long displaySeconds = seconds;
    unsigned long minutes = displaySeconds / 60;
    unsigned long hours = minutes / 60;
    displaySeconds %= 60;
    minutes %= 60;
    hours %= 24;
    hms(hours, minutes, displaySeconds);
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

void TimerStopwatch::loop()
{
    stateMachine();
    lcd.setCursor(0, 0);
    lcd.print("MODE:");
    lcd.setCursor(0, 1);
    lcd.print("TIME:");
    if (state == TIMER || state == COUNTDOWN || state == TPAUSE || state == ZERO)
    {
        lcd.setCursor(11, 0);
        lcd.print("TIMER");
    }
    else if (state == STOPWATCH || state == COUNTUP || state == SPAUSE)
    {
        lcd.setCursor(7, 0);
        lcd.print("STOPWATCH");
    }
    Serial.println(seconds);
}