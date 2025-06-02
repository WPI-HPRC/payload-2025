#include "States.h"


void Coast::initialize_impl() {
  Serial.println("Coast initialized!");
}

// TODO: figure out how to detect the apogee of the rocket, without knowing which way is up or down (assuming the payload is inserted incorrectly)
// probably use the derivative and detect a sign change from the initial velocity of the payload going up... hmm
State *Coast::loop_impl() {
    Serial.println("Coast looped");

    if (this->currentTime > 5000) {
      return (State *)new DrogueDescent(this->ctx);
    }
    return nullptr;
}
