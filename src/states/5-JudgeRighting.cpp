#include "States.h"

void JudgeRighting::initialize_impl() {
  Serial.println("Judge Righting initialized!");
}

State *JudgeRighting::loop_impl() {
    Serial.println("Judge Righting looped");
    
    if (this->currentTime > 5000) {
        return (State *)new VerticalSide(this->ctx);
    }
    return nullptr;
}
