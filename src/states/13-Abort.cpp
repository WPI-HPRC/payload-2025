#include "States.h"

void Abort::initialize_impl() {
  Serial.println("Abort initialized!");
}

State *Abort::loop_impl() {
    Serial.println("Abort looped");
    
    if (this->currentTime > 5000) {
        return (State *)new Flail(this->ctx);
    }
    return nullptr;
}
