#include "States.h"

void Recovery::initialize_impl() {
  Serial.println("Recovery initialized!");
}

State *Recovery::loop_impl() {
    Serial.println("Recovery looped");

    // CLOSE EVERYTHING AND WAIT FOR RECOVERY
    ctx->augerExtServo.write(AUGER_MAX_CLOSED_POS); 
    ctx->SolidDeliveryDoorServo.write(SOLID_DELIVERY_DOOR_CLOSED_POS); 
    return nullptr;
}
