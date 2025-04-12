#include "States.h"

void ExtendAuger::initialize_impl() {
  Serial.println("Extend Auger initialized!");
}

State *ExtendAuger::loop_impl() {
    Serial.println("Extend Auger looped");
    
    if (this->currentTime > 5000) {
        return (State *)new Drill(this->ctx);
    }
    return nullptr;
}
