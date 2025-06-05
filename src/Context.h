#pragma once

#include "BasicLinearAlgebra.h"
#include "config.h"

struct Context {
#if defined(MARS)
    ASM330 accel;
    LPS22 baro;
    ICM20948 mag;
    SdFs sd;
#elif defined(POLARIS)
    ICM42688_ accel;
    MS5611 baro;
    MMC5983 mag;
#endif
    MAX10S gps;
    File logFile;
    bool flightMode;
    BLA::Matrix<13, 1> quatState;
    BLA::Matrix<6,1> pvState; 

    //Servos
    Servo vertFlapServo;
    Servo horzFlapServo;
    Servo augerExtServo;
    Servo drillServo;
    Servo SolidDeliveryServo;
    Servo LiquidDeliveryServo;

    // flags
    bool inBushTimesFlailed = 0; // if i was the only one looking at this code i would name this: its_so_over = ts pmo 🥀 ... when we get to 4 times flailed then we give up

    void logCsvHeader() {
        logFile.print("timestamp,");
        baro.logCsvHeader(logFile);
        logFile.print(",");
        accel.logCsvHeader(logFile);
        logFile.print(",");
        mag.logCsvHeader(logFile);
        logFile.print(",");
        gps.logCsvHeader(logFile);
        logFile.println();
    }
};