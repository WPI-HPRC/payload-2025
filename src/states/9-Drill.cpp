#include "States.h"

void Drill::initialize_impl() {
  Serial.println("Drill initialized!");
}

State *Drill::loop_impl() {
    Serial.println("Drill looped");
    
    if (this->currentTime > 5000) {
        return (State *)new SolidDelivery(this->ctx);
    }
    return nullptr;
}
