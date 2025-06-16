#include "States.h"

void SolidEjection::initialize_impl() {
    Serial.println("Solid Ejection initialized!");
}

State* SolidEjection::loop_impl() {
    Serial.println("Solid Ejection looped");
    ctx->SolidDeliveryDoorServo.write(1500);
    ctx->SolidEjectionServo.write(SOLID_EJECTION_POS);


    if (this->currentTime > MAX_SOLID_EJECTION_TIME) {
        return new LiquidDelivery(ctx);
    }
    return nullptr;
} 