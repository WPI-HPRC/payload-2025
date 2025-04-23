#include "States.h"

void PreLaunch::initialize_impl() { Serial.println("PreLaunch initialized!"); }

State *PreLaunch::loop_impl() {
    Serial.println("PreLaunch looped");
    // Serial.println("PreLaunch Looped");
    // if (this->ctx->accel->getData().zAcc > LAUNCH_THRESHHOLD) {
    //   return (State *)new Boost(this->ctx);
    // }

    if (this->currentTime > 5000) {
        return (State *)new Boost(this->ctx);
    }
    return nullptr;
}
