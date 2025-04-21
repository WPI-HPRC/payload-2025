#include "States.h"

void Boost::initialize_impl() {
  Serial.println("Boost initialized!");
}

State *Boost::loop_impl() {
    Serial.println("Boost looped");

    if (this->currentTime > 5000) {
      return (State *)new Coast(this->ctx);
    }
    return nullptr;
}
