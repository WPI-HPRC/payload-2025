#include "States.h"

void MainDescent::initialize_impl() {
  Serial.println("Main Descent initialized!");
}

State *MainDescent::loop_impl() {
    Serial.println("Main Descent looped");
    
    if (this->currentTime > 5000) {
        return (State *)new JudgeRighting(this->ctx);
    }
    return nullptr;
}
