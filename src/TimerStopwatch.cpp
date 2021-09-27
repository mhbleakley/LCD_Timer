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
        checkSTSP(COUNTDOWN);
        checkMODE(STOPWATCH);
        break;
    case STOPWATCH:
        checkSTSP(COUNTUP);
        checkMODE(TIMER);
        break;
    case COUNTDOWN:
        checkSTSP(TPAUSE);
        checkRES(TIMER);
        timer(0, 5, 0);
        break;
    case COUNTUP:
        checkSTSP(SPAUSE);
        checkRES(STOPWATCH);
        stopwatch();
        break;
    case TPAUSE:
        checkSTSP(COUNTDOWN);
        checkRES(TIMER);
        break;
    case SPAUSE:
        checkSTSP(COUNTUP);
        checkRES(STOPWATCH);
        break;
    case ALARM:
        break;
    }
}

void TimerStopwatch::checkSTSP(int nextState){
    if(STSP.isPressed()){
            sDown = true;
        }
        if(STSP.isUnpressed() && sDown){
            state = nextState;
            sDown = false;
        }
}

void TimerStopwatch::checkRES(int nextState){
    if(RES.isPressed()){
            rDown = true;
        }
        if(RES.isUnpressed() && rDown){
            state = nextState;
            rDown = false;
        }
}

void TimerStopwatch::checkMODE(int nextState){
    if(MODE.isPressed()){
            mDown = true;
        }
        if(MODE.isUnpressed() && mDown){
            state = nextState;
            mDown = false;
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

void TimerStopwatch::loop(){
    stateMachine();
    lcd.setCursor(0, 0);
    lcd.print("MODE:");
    lcd.setCursor(0, 1);
    lcd.print("TIME:");
    if(state == TIMER||state == COUNTDOWN||state == TPAUSE){
        lcd.setCursor(11, 0);
        lcd.print("TIMER");
    }else if (state == STOPWATCH||state == COUNTUP||state == SPAUSE){
        lcd.setCursor(7, 0);
        lcd.print("STOPWATCH");
    }
}