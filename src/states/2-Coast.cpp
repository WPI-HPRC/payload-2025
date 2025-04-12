#include "States.h"

void Coast::initialize_impl() {
  Serial.println("Coast initialized!");
}

State *Coast::loop_impl() {
    Serial.println("Coast looped");

    if (this->currentTime > 5000) {
      return (State *)new DrogueDescent(this->ctx);
    }
    return nullptr;
}
