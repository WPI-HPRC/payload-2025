#include "States.h"

void HorizontalSide::initialize_impl() {
  Serial.println("Horizontal Side initialized!");
}

State *HorizontalSide::loop_impl() {
  Serial.println("Horizontal Side looped");
  ctx->horzFlap.write(FLAP_EXTENDED_POS); //money buys pid :) i love axons

  const auto gyroData = ctx->accel.getData();
  if (gyroData.getLastUpdated() != lastGyroReadTime) {
      lastGyroReadTime = gyroData.getLastUpdated();
      if (isRotatingDebouncer.update(abs(gyroData->gyrZ) > IS_TUMBLING_VEL_THRESHOLD, //TODO: check that axis is correct
                                      ::millis())) {
          return new Tumbling(ctx);
      }
  }

  //if we are stuck flapping and we aren't rotating then we are in a bush bruh... BEGIN FLAILING BC ITS SO OVER
  if (this->currentTime > MAX_TRY_BEFORE_FLAIL_TIME) {
    if (ctx->inBushTimesFlailed < 4) {
      ctx->inBushTimesFlailed += 1;
      return new VerticalSide(ctx);
    }
    else { //ik i dont need an else here but i like readability
      //we've reached max flail tries on both sets of flaps so we give up
      return new SolidDelivery(ctx);
    }
  }


  return nullptr;
}

