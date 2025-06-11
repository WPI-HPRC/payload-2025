#include "States.h"
#include "../boilerplate/Utilities/QuaternionUtils.h"

void JudgeRighting::initialize_impl() {
  Serial.println("Judge Righting initialized!");
}

// Define pos z as facing towards cone of rocket, pos y as facing top side (bottom side is where the drill extends to), pos x towards the right side (Looking up the payload towards the cone of the rocket)
// FIX: change the axis to the quaternion one from the ekf
State *JudgeRighting::loop_impl() {
    //just in case flaps aren't closed:
    ctx->vertFlapServo.write(FLAP_RETRACTED_POS, this->currentTime);
    ctx->horzFlapServo.write(FLAP_RETRACTED_POS, this->currentTime);

    const auto magData = ctx->mag.getData();
    if (magData.getLastUpdated() != lastMagReadTime){
        lastMagReadTime = magData.getLastUpdated();
        
#ifdef DEBUG
        Serial.print(">accel_x:"); Serial.println(magData->accelX);
        Serial.print(">accel_y:"); Serial.println(magData->accelY);
#endif

        //TODO: Check GroundSide Found is Correct
        GroundSide currSide = getGroundSide(magData->accelX, magData->accelY);
        
#ifdef DEBUG
        Serial.print(">ground_side_raw:"); Serial.println((int)currSide);
        Serial.print(">ground_side_raw_str:"); Serial.println(groundSideToString(currSide));
#endif

        //TODO: Check multistate debouncer works
        GroundSide debouncedSide = judgeRightingDebouncer.update(currSide, ::millis());
        
#ifdef DEBUG
        Serial.print(">ground_side_debounced:"); Serial.println((int)debouncedSide);
        Serial.print(">ground_side_debounced_str:"); Serial.println(groundSideToString(debouncedSide));
#endif

        switch (debouncedSide) {
            case GroundSide::BOTTOM:
            // I WOULD LOVE TO HAVE AN IR SENSOR RN  -_-
                return new ExtendingAuger(ctx);
            case GroundSide::LEFT:
            case GroundSide::RIGHT:
                return new HorizontalSide(ctx);
            case GroundSide::TOP:
                return new VerticalSide(ctx);
        }
    }

    if (this->currentTime > MAX_TUMBLE_TIME){
        ctx->errorLogFile.printf("[%d] Judge Righting state timed out\n", ::millis());
        return new SolidDelivery(ctx);
    }
    
    return nullptr;
}

/**
 * @brief Convert GroundSide enum to string for debugging
 */
const char* JudgeRighting::groundSideToString(GroundSide side) {
    switch (side) {
        case GroundSide::TOP: return "TOP";
        case GroundSide::BOTTOM: return "BOTTOM";
        case GroundSide::LEFT: return "LEFT";
        case GroundSide::RIGHT: return "RIGHT";
        case GroundSide::UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

/**
 * @brief We use the absolute magnitude of accelX and accelY to determine what axis gravity influences more (essentially usees the concept of component based. 
 * This way we don't need to do any atan2 nonsense and compute is quicker).
 * Then we find the sign to know which side we are on (top v bottom || left v right)
 * Error returns GroundSide::UNKNOWN
 */
JudgeRighting::GroundSide JudgeRighting::getGroundSide(const float& accelX, const float& accelY) { //FIXME: check axis of accelX and accelY
    if (std::abs(accelX) >= std::abs(accelY)){
        if (accelX > 0) { //gravity is pointed in the negative X axis (since accelerometer measures normal force)
            return GroundSide::RIGHT;
        }
        else {
            return GroundSide::LEFT;
        }
    }
    else {
        if (accelY > 0){ //gravity is pointed in the negative y axis (bottom side where drill points)
            return GroundSide::BOTTOM;
        }
        else {
            return GroundSide::TOP;
        }
    }
    return GroundSide::UNKNOWN;
}