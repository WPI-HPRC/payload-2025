#include "States.h"

void Tumbling::initialize_impl() {
    Serial.println("Tumbling initialized!");
}
 
State *Tumbling::loop_impl() {
    Serial.println("Tumbling looped");
    //if we came here from VerticalSide or HorizontalSide then we need to close the flaps
    ctx->vertFlapServo.write(FLAP_RETRACTED_POS, this->currentTime);
    ctx->horzFlapServo.write(FLAP_RETRACTED_POS, this->currentTime);


    // talk w colette here about using the gyro... iirc from rbe2002 the gyro has a ton of bias so numerically integrating it to find the current gyro velocity shouldn't work and i need to use the ekf? lowk i dont remember...
    // or does the gyro measure angular velocity because in that case it works perfect lol and im being dumb... now that i think abouot it, i think im dumb!

    const auto gyroData = ctx->mag.getData();
    if (gyroData.getLastUpdated() != lastGyroReadTime) {
        lastGyroReadTime = gyroData.getLastUpdated();
        // TODO: DO TESTING TO ACCOUNT FOR BIAS ✅
        if (tumblingDebouncer.update(std::abs(gyroData->gyrZ - ctx->gyZBias) < IS_END_TUMBLING_VEL_THRESHOLD, //TODO: check that axis is correct ✅
                                        ::millis())) {
            return new ExtendingAuger(ctx);
        }
    }

    if (this->currentTime > MAX_TUMBLE_TIME) { //NOTE: if this happens then the gyro is cooked and we've tumbled forever... might as well give up and go to door deploy? :(
        ctx->errorLogFile.printf("[%d] Tumbling state timed out\n", ::millis());
        return new ExtendingAuger(ctx);

    }


    return nullptr;
} 