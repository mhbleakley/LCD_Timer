#include <Arduino.h>
#include <Timer.h>
class LinearButton{
private:
    int p;
    enum STATE
    {
        HI,
        LO,
        HOLD
    } state;
    
public:
    void init(int pin);
    void loop();
    int getState();
};