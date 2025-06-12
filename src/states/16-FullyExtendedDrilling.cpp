#include "States.h"

void FullyExtendedDrilling::initialize_impl() {
    Serial.println("FullyExtendedDrilling initialized!");
}
 
State *FullyExtendedDrilling::loop_impl() {
    Serial.println("FullyExtendedDrilling looped");

    ctx->drillServo.write(DRILL_SPEED);
    ctx->augerExtServo.write(AUGER_EXT_OUT_SPEED);

    if (this->currentTime > MAX_DRILL_TIME) {
      return new SolidDelivery(ctx);
    }

    return nullptr;

} 