#include "States.h"

void JudgeRighting::initialize_impl() {
  Serial.println("Judge Righting initialized!");
}

State *JudgeRighting::loop_impl() {    
    if (this->currentTime < 2000) {
        this->ctx->auger.write(1000);
    } else {
        this->ctx->auger.write(1500);
    }
    
    // if (this->currentTime > 5000) {
    //     return (State *)new VerticalSide(this->ctx);
    // }
    return nullptr;
}
