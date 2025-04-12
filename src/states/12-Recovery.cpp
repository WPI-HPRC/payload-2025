#include "States.h"

void Recovery::initialize_impl() {
  Serial.println("Recovery initialized!");
}

State *Recovery::loop_impl() {
    Serial.println("Recovery looped");
    
    if (this->currentTime > 5000) {
        return (State *)new Abort(this->ctx);
    }
    return nullptr;
}
