#include "States.h"

void DeadServoTest::initialize_impl() {
    Serial.println("Starting Potentiometer Test State");
}

State* DeadServoTest::loop_impl() {
    // Read and print raw potentiometer values from both flaps

    /*
  ctx->SolidDeliveryDoorServo.writeMicroseconds(1500); // Center
  delay(1000);
  ctx->SolidDeliveryDoorServo.writeMicroseconds(2000); // Full forward
  delay(1000);
  ctx->SolidDeliveryDoorServo.writeMicroseconds(1000); // Full back
  delay(1000);*/


    Serial.print(">drill_servo_Pos:"); Serial.println(pos);
    if (currentTime - lastTimeRun >= 100) {
        if (increasing) {
            pos += 10;
            if (pos > 2000) increasing = false;
        }
        else {
            pos -= 10;
           if (pos <= 1000) increasing = true;
        }
       ctx->LiquidDeliveryServo.writeMicroseconds(pos);
       lastTimeRun = currentTime;
     }
    
    // No transitions needed
    return nullptr;
}
