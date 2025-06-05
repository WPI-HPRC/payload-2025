#include "States.h"

void LiquidDelivery::initialize_impl() {
  Serial.println("Liquid Delivery initialized!");
}

State *LiquidDelivery::loop_impl() {
    Serial.println("Liquid Delivery looped");
    
    if (this->currentTime > 5000) {
        return (State *)new Recovery(this->ctx);
    }
    return nullptr;
}
