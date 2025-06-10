#include "States.h"

void SolidDelivery::initialize_impl() {
  Serial.println("Solid Delivery initialized!");
}

State *SolidDelivery::loop_impl() {
    Serial.println("Solid Delivery looped");
    
    ctx->SolidDeliveryDoorServo.write(SOLID_DELIVERY_DOOR_OPEN_POS);
    ctx->augerExtServo.write(AUGER_EXT_IN_SPEED);

    if (this->currentTime > MAX_SOLID_DOOR_OPEN_TIME) {
        ctx->SolidDeliveryDoorServo.write(0);
        return new SolidEjection(ctx);
    }
    return nullptr;
}
