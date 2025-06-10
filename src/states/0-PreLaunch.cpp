#include "States.h"

void PreLaunch::initialize_impl() {
  Serial.println("PreLaunch initialized!");
}

//TODO: Test Rolling Average for GyroBias

State *PreLaunch::loop_impl() {

  const auto accelData = ctx->mag.getData();
  const auto baroData = ctx->baro.getData();


  if (accelData.getLastUpdated() != lastAccelReadingTime) {
    gyZBiasAvg.update(accelData->gyrZ);
    lastAccelReadingTime = accelData.getLastUpdated();
    //TODO: Check LaunchThreshold
    if (accelDebouncer.update(accelData->accelZ > LAUNCH_THRESHHOLD,
                                    ::millis())) {
        ctx->gyZBias = gyZBiasAvg.getAvg();
        return new Coast(ctx);
    }
}

if (!altAverager.isBufferSaturated() && baroData.getLastUpdated() != lastBaroReadingTime) {
    lastBaroReadingTime = baroData.getLastUpdated();
    altAverager.update(baroData->altitude);

    if (altAverager.isBufferSaturated()) {
        ctx->initialAltitude = altAverager.getAvg();
        ctx->errorLogFile.printf("[%u] Initial altitude: %f m\n", ::millis(), ctx->initialAltitude);
    }
}
  return nullptr;
}
