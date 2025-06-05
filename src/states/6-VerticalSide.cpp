#include "States.h"

void VerticalSide::initialize_impl() {
  Serial.println("Vertical Side initialized!");
}

State *VerticalSide::loop_impl() {
    Serial.println("Vertical Side looped");
    ctx->vertFlap.write(FLAP_EXTENDED_POS); //money buys pid :) i love axons

    const auto gyroData = ctx->accel.getData();
    if (gyroData.getLastUpdated() != lastGyroReadTime) {
        lastGyroReadTime = gyroData.getLastUpdated();
        if (isRotatingDebouncer.update(abs(gyroData->gyrZ) < BURN_THRESHHOLD_G, //TODO: check that axis is correct
                                        ::millis())) {
            return new JudgeRighting(ctx);
        }
    }


    return nullptr;
}