#include "States.h"

void ExtendingAuger::initialize_impl() {
  Serial.println("Extend Auger initialized!");
}

State *ExtendingAuger::loop_impl() {
    Serial.println("Extend Auger looped");
    
    if (this->currentTime > 5000) {
        return (State *)new Drill(this->ctx);
    }
    return nullptr;
}
