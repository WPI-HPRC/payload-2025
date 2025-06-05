#include "States.h"
#include "../boilerplate/Utilities/QuaternionUtils.h"

void JudgeRighting::initialize_impl() {
  Serial.println("Judge Righting initialized!");
}

// Define pos z as facing towards cone of rocket, pos y as facing top side (bottom side is where the drill extends to), pos x towards the right side (Looking up the payload towards the cone of the rocket)
// FIX: change the axis to the quaternion one from the ekf
// TODO: recheck the quaternion value to make sure its updated
State *JudgeRighting::loop_impl() {
    //just in case flaps aren't closed:
    ctx->vertFlap.write(FLAP_RETRACTED_POS);
    ctx->horzFlap.write(FLAP_RETRACTED_POS);


    // get rotation matrix from quat
    BLA::Matrix<3,3> rot = QuaternionUtils::quatToRot(ctx->quatState);
    
    // get which side is on the ground
    QuaternionUtils::GroundSide groundSide = QuaternionUtils::getGroundSide(rot);
    
    /*Serial.print("Ground side: ");
    Serial.println(QuaternionUtils::groundSideToString(groundSide));*/
    
    // transition to appropriate state based on which side is down
    switch (groundSide) {
        case QuaternionUtils::GroundSide::BOTTOM:
        // I WOULD LOVE TO HAVE AN IR SENSOR RN  -_-
            return new ExtendingAuger(ctx);
        case QuaternionUtils::GroundSide::LEFT:
        case QuaternionUtils::GroundSide::RIGHT:
            return new HorizontalSide(ctx);
        case QuaternionUtils::GroundSide::TOP:
            return new VerticalSide(ctx);
        case QuaternionUtils::GroundSide::UNKNOWN:
            // just go to left side idek... worst case scenario it fixes itself in flailing
            return new HorizontalSide(ctx);
    }
    
    return nullptr;
}
