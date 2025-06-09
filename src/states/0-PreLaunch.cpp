#include "States.h"

void PreLaunch::initialize_impl() {
  Serial.println("PreLaunch initialized!");
}

//TODO: Test Rolling Average for GyroBias

//FIXME: Make everything mag instead of accel (all classes)

State *PreLaunch::loop_impl() {

  const auto magData = ctx->mag.getData();
  if (magData.getLastUpdated() != lastAccelReadingTime) {
      gyZBiasAvg.update(magData->gyrZ);
      lastAccelReadingTime = magData.getLastUpdated();
      if (launchAccelDebouncer.update(abs(magData->accelZ) > LAUNCH_THRESHHOLD_G, //added abs for the accel!
                                      ::millis())) {
          ctx->gyZBias = gyZBiasAvg.getAvg();
          return new Coast(ctx);
      }
  }


  Serial.println("PreLaunch looped");

  /*if (this->currentTime > 5000) {
    return (State *)new Boost(this->ctx);
  }*/
  return nullptr;
}
