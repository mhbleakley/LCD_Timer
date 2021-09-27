#include <LinearButton.h>

Timer holdTimer(1200);

void LinearButton::init(int pin)
{
    p = pin;
    pinMode(pin, INPUT);
}

void LinearButton::loop()
{
    int pressed = !digitalRead(p);
    switch (state)
    {
    case UNPRESSED:
        if (pressed)
        {
            state = PRESSED;
            holdTimer.reset();
        }
        break;
    case PRESSED:
        if (holdTimer.isExpired() && pressed)
        {
            state = HELD;
        }else if (!pressed){
            state = UNPRESSED;
        }
        break;
    case HELD:
        if (!pressed)
        {
            state = UNPRESSED;
        }
        break;
    default:
        state = UNPRESSED;
        break;
    }
}

int LinearButton::getState()
{
    return state;
}

bool LinearButton::isPressed(){
    return state == PRESSED;
}

bool LinearButton::isHeld(){
    return state == HELD;
}

bool LinearButton::isUnpressed(){
    return state == UNPRESSED;
}