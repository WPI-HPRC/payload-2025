#include "States.h"

void DrogueDescent::initialize_impl()
{
    Serial.println("Drogue Descent initialized!");
}

State *DrogueDescent::loop_impl()
{
    Serial.println("Drogue Descent looped");
    
    if (this->currentTime > 5000)
    {
        return (State *)new MainDescent(this->ctx);
    }
    return nullptr;
}
