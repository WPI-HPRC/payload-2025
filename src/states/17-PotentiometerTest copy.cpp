#include "States.h"

void PotentiometerTest::initialize_impl() {
    Serial.println("Starting Potentiometer Test State");
}

State* PotentiometerTest::loop_impl() {
    // Read and print raw potentiometer values from both flaps
    float horzPotValue = ctx->horzFlapServo.readRaw();
    float vertPotValue = ctx->vertFlapServo.readRaw(); 
    float horzPotConvert = ctx->horzFlapServo.read();
    float vertPotConvert = ctx->vertFlapServo.read();
    
    Serial.print(">horzFlap_raw:"); Serial.println(horzPotValue);
    Serial.print(">vertFlap_raw:"); Serial.println(vertPotValue);
    Serial.print(">horzConvert:"); Serial.println(horzPotConvert);
    Serial.print(">vertConvert:"); Serial.println(vertPotConvert);

    // No transitions needed
    return nullptr;
}
