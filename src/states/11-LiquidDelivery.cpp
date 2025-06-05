#include "States.h"

void LiquidDelivery::initialize_impl() {
  Serial.println("Liquid Delivery initialized!");
}

State *LiquidDelivery::loop_impl() {
    Serial.println("Liquid Delivery looped");
    
    ctx->LiquidDeliveryServo.write(LIQUID_DELIVERY_POS);  
    ctx->augerExtServo.write(AUGER_MAX_CLOSED_POS); //close auger just in case its still open

    if (this->currentTime > MAX_LIQUID_DELIVERY_TIME) {
        return new Recovery(ctx);
    }
    return nullptr;
}
