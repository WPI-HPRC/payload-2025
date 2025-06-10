#include "States.h"

void SolidEjection::initialize_impl() {
    Serial.println("Solid Ejection initialized!");
}

State* SolidEjection::loop_impl() {
    Serial.println("Solid Ejection looped");
    ctx->SolidEjectionServo.write(SOLID_EJECTION_POS);
    ctx->augerExtServo.write(0); //close auger


    if (this->currentTime > MAX_SOLID_EJECTION_TIME) {
        //FIXME: Turn on solenoids here
        return new LiquidDelivery(ctx);
    }
    return nullptr;
} 