#include "States.h"

void Descent::initialize_impl() {
  prevAltitude = this->ctx->baro.getData()->altitude;
  Serial.println("Descent Looped");
}

State *Descent::loop_impl() {
  Serial.println("Descent Looped");
  const auto baroData = ctx->baro.getData();

  if (lastBaroReadingTime < baroData.getLastUpdated()) {
    lastBaroReadingTime = baroData.getLastUpdated();

    ewma.update((baroData->altitude - prevAltitude) * (::millis() - lastBaroReadingTime) / 1000.);
    //TODO: Check LANDED_VEL_THRESHOLD ✅
    if (velDebouncer.update(std::abs(ewma.getAvg()) < LANDED_VEL_THRESHHOLD, ::millis())) {
      return new Tumbling(this->ctx);
    }
  }

  //TODO: Check MAIN_DESCENT_MAX_TIME ✅
  if (this->currentTime >= MAIN_DESCENT_MAX_TIME) {
    ctx->errorLogFile.printf("[%d] MainDescent state timed out\n", ::millis());
    return new Tumbling(this->ctx);
  }
  
  return nullptr;
}