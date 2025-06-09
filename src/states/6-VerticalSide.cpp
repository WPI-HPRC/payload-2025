#include "States.h"

void VerticalSide::initialize_impl() {
  Serial.println("Vertical Side initialized!");
}

State *VerticalSide::loop_impl() {
    Serial.println("Vertical Side looped");
    ctx->vertFlapServo.write(FLAP_EXTENDED_POS); //money buys pid :) i love axons

    const auto gyroData = ctx->mag.getData();
    if (gyroData.getLastUpdated() != lastGyroReadTime) {
        lastGyroReadTime = gyroData.getLastUpdated();
        if (isRotatingDebouncer.update(std::abs(gyroData->gyrZ - ctx->gyZBias) > IS_TUMBLING_VEL_THRESHOLD, //TODO: check that axis is correct
                                        ::millis())) {
            return new Tumbling(ctx);
        }
    }

    //if we are stuck flapping and we aren't rotating then we are in a bush bruh... BEGIN FLAILING BC ITS SO OVER
    if (this->currentTime > MAX_TRY_BEFORE_FLAIL_TIME) {
      if (ctx->inBushTimesFlailed < MAX_FLAIL_ATTEMPTS) {
        ctx->inBushTimesFlailed += 1;
        return new HorizontalSide(ctx);
      }
      else { //ik i dont need an else here but i like readability
        //we've reached max flail tries on both sets of flaps so we give up
        return new SolidDelivery(ctx);
      }
    }


    return nullptr;
}