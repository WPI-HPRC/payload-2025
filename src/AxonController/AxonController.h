#pragma once

#include <Arduino.h>
#include <Servo.h>
#include <cstdint>
#include "boilerplate/Logging/Loggable.h"
#include "boilerplate/Utilities/PIDController.h"

class AxonController {
  public:
    AxonController(uint8_t servo_pin, uint8_t feedback_pin, float kp, float ki, float kd, float outMin, float outMax)
        : servo_pin(servo_pin), feedback_pin(feedback_pin), kp(kp), ki(ki), kd(kd), outMin(outMin), outMax(outMax), maxPWMRange(outMax - outMin), servo(){}

    void init();

    void write(float pos, long currTime);

    int read();

  private:
    float kp;
    float ki;
    float kd;
    float outMin;
    float outMax;
    float maxPWMRange;
    uint8_t servo_pin;
    uint8_t feedback_pin;
    Servo servo;
    PIDController pid = PIDController(kp, ki, kd, outMin, outMax);
};