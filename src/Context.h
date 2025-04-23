#pragma once

#include "config.h"
#include "servos/ServoController.h"
struct Context {
#if defined(MARS)
    ASM330 accel;
    LPS22 baro;
    ICM20948 mag;

    //Add Servos here using servo class in boilerplate/Servo/Servo.h

#elif defined(POLARIS)
    ICM42688_* accel;
    MS5611* baro;
    MMC5983* mag;
#endif
    MAX10S gps;
    ServoController auger;
    ServoController rack_pinion;
    ServoController pinion_door_1;
    ServoController pinion_door_2;
    ServoController lead_screw;
    ServoController axon1;
    ServoController axon2;
    File logFile;
    bool flightMode = false;
};
