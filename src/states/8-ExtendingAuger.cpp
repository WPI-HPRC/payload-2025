#include "States.h"

void ExtendingAuger::initialize_impl() {
  Serial.println("Extend Auger initialized!");
}

State *ExtendingAuger::loop_impl() {
    Serial.println("Extend Auger looped");
    // no way to know if we are touching the ground w/o current sensing so imma j keep running this thing into the ground at full force until timer stop

    ctx->augerExtServo.write(AUGER_MAX_EXT_POST);

    //bc we want the drill to spin at a constant vel

    //TODO: Write speed controller. Biggest issue rn is knowing what pins give encoder info and then writing the class for that... i have good 2002 code but its been a while

    // ControlSpeed(ctx->drillServo, DRILL_SPEED);


    if (abs((long)(ctx->augerExtServo.read() - AUGER_MAX_EXT_POST)) < AUGER_EXT_POS_BAND // we reached max extension
        || this->currentTime > MAX_AUG_EXT_TIME//max extension couldn't be reached. probably drilling into a rock
        ) {
      return new FullyExtendedDrilling(ctx);
    }

    return nullptr;
}
