#include "States.h"

#define DEBUG

void PreLaunch::initialize_impl() {
  Serial.println("PreLaunch initialized!");
}

State *PreLaunch::loop_impl() {

  Serial.println("Prelaunch Looped");

  const auto magData = ctx->mag.getData();
  const auto baroData = ctx->baro.getData();

  //TODO: Check gyZBiasAvg ✅
  if (magData.getLastUpdated() != lastAccelReadingTime) {
    gyZBiasAvg.update(magData->gyrZ);
    
    #ifdef DEBUG
        Serial.print(">gyro_bias_avg:"); Serial.println(gyZBiasAvg.getAvg());
    #endif

    lastAccelReadingTime = magData.getLastUpdated();
    //TODO: Check LaunchThreshold ✅
    if (accelDebouncer.update(magData->accelZ > LAUNCH_THRESHHOLD, ::millis()) && (currentTime > 5000)) {
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
