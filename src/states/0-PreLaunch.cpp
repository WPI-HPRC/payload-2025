#include "States.h"

void PreLaunch::initialize_impl() {
  Serial.println("PreLaunch initialized!");
}

//TODO: Try using a rolling average to measure the gyro bias... could potentially make the bias +- milli rad /s

State *PreLaunch::loop_impl() {
  const auto accelData = ctx->accel.getData();
  if (accelData.getLastUpdated() != lastAccelReadingTime) {
      lastAccelReadingTime = accelData.getLastUpdated();
      if (launchAccelDebouncer.update(abs(accelData->accelZ) > LAUNCH_THRESHHOLD_G, //added abs for the accel!
                                      ::millis())) {
          return new Coast(ctx);
      }
  }


  Serial.println("PreLaunch looped");

  /*if (this->currentTime > 5000) {
    return (State *)new Boost(this->ctx);
  }*/
  return nullptr;
}
