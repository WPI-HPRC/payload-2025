#include "States.h"

void VerticalSide::initialize_impl() {
  Serial.println("Vertical Side initialized!");
}

State *VerticalSide::loop_impl() {
    Serial.println("Vertical Side looped");
    
    if (this->currentTime > 5000) {
        return (State *)new HorizontalSide(this->ctx);
    }
    return nullptr;
}