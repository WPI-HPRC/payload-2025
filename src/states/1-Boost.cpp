#include "States.h"

void Boost::initialize_impl() {
  Serial.println("Boost initialized!");
}

State *Boost::loop_impl() {
  const auto accelData = ctx->accel.getData();
  if (accelData.getLastUpdated() != lastAccelReadingTime) {
      lastAccelReadingTime = accelData.getLastUpdated();
      if (burnTimeDebouncer.update(abs(accelData->accelZ) < BURN_THRESHHOLD_G, //added abs for the accel!
                                      ::millis()) || currentTime > MAX_BOOST_TIME) {
          return new Coast(ctx);
      }
  }

    Serial.println("Boost looped");

    /*if (this->currentTime > 5000) {
      return (State *)new Coast(this->ctx);
    }
    */
    return nullptr;
}
