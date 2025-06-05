#include "States.h"

void Tumbling::initialize_impl() {
    Serial.println("Tumbling initialized!");
}
 
State *Tumbling::loop_impl() {
    // talk w colette here about using the gyro... iirc from 2002 the gyro has a ton of bias so numerically integrating it to find the current gyro velocity shouldn't work and i need to use the ekf? lowk i dont remember...
    // or does the gyro measure angular velocity because in that case it works perfect lol and im being dumb... now that i think abouot it, i think im dumb!

    const auto gyroData = ctx->accel.getData();
    if (gyroData.getLastUpdated() != lastGyroReadTime) {
        lastGyroReadTime = gyroData.getLastUpdated();
        if (tumblingDebouncer.update(abs(gyroData->gyrZ) < BURN_THRESHHOLD_G, //TODO: check that axis is correct
                                        ::millis())) {
            return new JudgeRighting(ctx);
        }
    }

    if (this->currentTime > MAX_TUMBLE_TIME) { //NOTE: if this happens then the gyro is cooked and we've tumbled forever... might as well give up and go to door deploy? :(
        return new SolidDelivery(ctx);

    }

    Serial.println("Tumbling looped");
    return nullptr;
} 