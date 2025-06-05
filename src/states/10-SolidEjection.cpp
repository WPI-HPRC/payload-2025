#include "States.h"

void SolidEjection::initialize_impl() {
    Serial.println("Solid Ejection initialized!");
}

State* SolidEjection::loop_impl() {
    Serial.println("Solid Ejection looped");
    ctx->SolidEjectionServo.write(SOLID_EJECTION_POS);
    ctx->augerExtServo.write(AUGER_MAX_CLOSED_POS); //close auger


    if (this->currentTime > MAX_SOLID_EJECTION_TIME) {
        return new LiquidDelivery(ctx);
    }
    return nullptr;
} 