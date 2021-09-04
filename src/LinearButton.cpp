#include <LinearButton.h>

Timer holdTimer(1200);

void LinearButton::init(int pin)
{
    p = pin;
    pinMode(pin, INPUT);
}

void LinearButton::loop()
{
    int button = digitalRead(p);
    switch (state)
    {
    case HI:
        if (!button)
        {
            state = LO;
            holdTimer.reset();
        }
        break;
    case LO:
        if (holdTimer.isExpired())
        {
            state = HOLD;
        }
        else
        {
            state = LO;
        }
        if (button)
        {
            state = HI;
        }
        break;
    case HOLD:
        if (button)
        {
            state = HI;
        }
        break;
    default:
        state = HI;
        break;
    }
}

int LinearButton::getState()
{
    return state;
}