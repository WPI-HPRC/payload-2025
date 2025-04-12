#include "States.h"

void SolidDelivery::initialize_impl() {
  Serial.println("Solid Delivery initialized!");
}

State *SolidDelivery::loop_impl() {
    Serial.println("Solid Delivery looped");
    
    if (this->currentTime > 5000) {
        return (State *)new LiquidDelivery(this->ctx);
    }
    return nullptr;
}
