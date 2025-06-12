#include "States.h"

void PotentiometerTest::initialize_impl() {
    Serial.println("Starting Potentiometer Test State");
}

State* PotentiometerTest::loop_impl() {
    // Read and print raw potentiometer values from both flaps
    int horzPotValue = ctx->horzFlapServo.readRaw();
    int vertPotValue = ctx->vertFlapServo.readRaw(); 
    
    Serial.print(">horzFlap_raw:"); Serial.println(horzPotValue);
    Serial.print(">vertFlap_raw:"); Serial.println(vertPotValue);

    // No transitions needed
    return nullptr;
}
