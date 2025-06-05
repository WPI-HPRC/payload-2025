#include "States.h"

void FullyExtendedDrilling::initialize_impl() {
    Serial.println("FullyExtendedDrilling initialized!");
}
 
State *FullyExtendedDrilling::loop_impl() {
    Serial.println("FullyExtendedDrilling looped");
    
    //TODO: Write speed controller. Biggest issue rn is knowing what pins give encoder info and then writing the class for that... i have good 2002 code but its been a while

    // ControlSpeed(ctx->drillServo, DRILL_SPEED);


    if (this->currentTime > MAX_DRILL_TIME) {
      return new SolidDelivery(ctx);
    }

} 