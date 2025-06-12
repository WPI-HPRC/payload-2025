#include "States.h"

void DeadServoTest::initialize_impl() {
    Serial.println("Starting Potentiometer Test State");
}

State* DeadServoTest::loop_impl() {
    // Read and print raw potentiometer values from both flaps

    
    if (increasing) {
        pos += 10;
        if (pos > 2500) increasing = false;
    }
    else {
        pos -= 10;
        if (pos <= 30) increasing = true;
    }


    Serial.print(">drill_servo_Pos:"); Serial.println(pos);
    ctx->drillServo.writeMicroseconds(pos);

    // No transitions needed
    return nullptr;
}
