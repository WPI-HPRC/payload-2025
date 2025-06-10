#include "States.h"

void ExtendingAuger::initialize_impl() {
  Serial.println("Extend Auger initialized!");
}

State *ExtendingAuger::loop_impl() {
    Serial.println("Extend Auger looped");
    // no way to know if we are touching the ground w/o current sensing so imma j keep running this thing into the ground at full force until timer stop

    //TODO: Add current sensor and limit switch implementation
    ctx->augerExtServo.write(AUGER_EXT_OUT_SPEED);
    ctx->drillServo.write(DRILL_SPEED);

    //bc we want the drill to spin at a constant vel

    if (this->currentTime > MAX_AUG_EXT_TIME) {
      return new FullyExtendedDrilling(ctx);
    }

    return nullptr;
}
