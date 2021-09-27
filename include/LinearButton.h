#include <Arduino.h>
#include <Timer.h>
class LinearButton{
private:
    int p;
    enum STATE
    {
        UNPRESSED,
        PRESSED,
        HELD
    } state;
    
public:
    void init(int pin);
    void loop();
    int getState();
    bool isUnpressed();
    bool isPressed();
    bool isHeld();
};