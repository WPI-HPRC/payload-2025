#include "States.h"

void LiquidDelivery::initialize_impl() {
  Serial.println("Liquid Delivery initialized!");
}

State *LiquidDelivery::loop_impl() {
    Serial.println("Liquid Delivery looped");
    
    ctx->LiquidDeliveryServo.write(LIQUID_DELIVERY_POS);  
    
    if (this->currentTime > MAX_LIQUID_DELIVERY_TIME) {
        return new Recovery(ctx);
    }
    return nullptr;
}
