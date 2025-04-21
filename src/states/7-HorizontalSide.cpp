#include "States.h"

void HorizontalSide::initialize_impl() {
  Serial.println("Horizontal Side initialized!");
}

State *HorizontalSide::loop_impl() {
    Serial.println("Horizontal Side looped");
    
    if (this->currentTime > 5000) {
        return (State *)new ExtendAuger(this->ctx);
    }
    return nullptr;
}
