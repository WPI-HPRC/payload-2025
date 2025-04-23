#include "ServoController.h"

void ServoController::init() {
    pinMode(servo_pin, OUTPUT);
    servo.attach(servo_pin);
    if (feedback_pin != 0) {
        pinMode(feedback_pin, INPUT);
    }
}

void ServoController::write(int val){
    servo.writeMicroseconds(val); 
}

int ServoController::read() {
    return analogRead(feedback_pin);
}