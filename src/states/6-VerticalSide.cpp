#include "States.h"

void VerticalSide::initialize_impl() {
  Serial.println("Vertical Side initialized!");
}

State *VerticalSide::loop_impl() {
    Serial.println("Vertical Side looped");
    ctx->vertFlapServo.write(FLAP_EXTENDED_POS, this->currentTime);

    const auto gyroData = ctx->mag.getData();
    if (gyroData.getLastUpdated() != lastGyroReadTime) {
        lastGyroReadTime = gyroData.getLastUpdated();
        //TODO: Check IS_TUMBLING_VEL_THRESHOLD (includes gyZBias offset works properly)
        if (isRotatingDebouncer.update(std::abs(gyroData->gyrZ - ctx->gyZBias) > IS_TUMBLING_VEL_THRESHOLD, //TODO: check that axis is correct
                                        ::millis())) {
            return new Tumbling(ctx);
        }
    }

    //if we are stuck flapping and we aren't rotating then we are in a bush bruh... BEGIN FLAILING BC ITS SO OVER
    if (this->currentTime > MAX_TRY_BEFORE_FLAIL_TIME) {
      ctx->errorLogFile.printf("[%d] Flailing Begin\n", ::millis());
      if (ctx->inBushTimesFlailed < MAX_FLAIL_ATTEMPTS) {
        ctx->inBushTimesFlailed += 1;
        ctx->errorLogFile.printf("[%d] Times Flailed: %d\n", ::millis(), ctx->inBushTimesFlailed);
        return new HorizontalSide(ctx);
      }
      else { //ik i dont need an else here but i like readability
        //we've reached max flail tries on both sets of flaps so we give up
        ctx->errorLogFile.printf("[%d] Max Flails Reached\n", ::millis());
        return new SolidDelivery(ctx);
      }
    }


    return nullptr;
}