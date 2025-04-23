#pragma once

#if defined(MARS)
    #include "boilerplate/Sensors/Impl/ASM330.h"
    #include "boilerplate/Sensors/Impl/ICM20948.h"
    #include "boilerplate/Sensors/Impl/LPS22.h"
    #include "boilerplate/Sensors/Impl/MAX10S.h"

    #include "SdFat.h"

    #define SD_CS PA15
    #define SD_SCLK PB3
    #define SD_MISO PB4
    #define SD_MOSI PB5

    #define XBEE_CS PA4
    #define XBEE_SCLK PA5_ALT1
    #define XBEE_MISO PA6_ALT1
    #define XBEE_MOSI PA7_ALT1

    #define XBEE_ATTN PD8

    #define SENSOR_SCL PB6
    #define SENSOR_SDA PB7

    #define LED_PIN PB9

    //GROUND PREP
    #define AUGER PA1
    #define RACK_PINION PF2
    //SOLID DELIVERY
    #define PINION_DOOR_1 PA2
    #define PINION_DOOR_2 PF4
    //LIQUID DELIVERY
    #define SOLENOID_1 PC2_C
    #define SOLENOID_2 PC3_C
    #define LEAD_SCREW PF3
    //IR SENSOR
    #define IR_SENSOR PC0
    //CURRENT SENSORS
    #define CURRENT_SENSOR_1 PF1
    #define CURRENT_SENSOR_2 PC1
    //LIMIT SWITCHES
    #define LIMIT_SWITCH_1 PA0
    #define LIMIT_SWITCH_2 PF0
    //EMATCHES
    #define EMATCH_1 PC13
    #define EMATCH_2 PE6
    //AXON 1
    #define AXON_1_IN PE2
    #define AXON_1_OUT PE1
    //AXON 2
    #define AXON_2_OUT PE5
    #define AXON_2_IN PE3


    // Servos for MARS

#elif defined(POLARIS)
    #include "boilerplate/Sensors/Impl/MAX10S.h"
    #include "boilerplate/Sensors/Impl/Polaris/ICM42688.h"
    #include "boilerplate/Sensors/Impl/Polaris/MS5611.h"
    #include "boilerplate/Sensors/Impl/Polaris/MMC5983.h"

    #include "SD.h"

    #define SD_CS 31
    #define SD_SCLK 13
    #define SD_MISO 12
    #define SD_MOSI 11

    #define SENSOR_SCL 19
    #define SENSOR_SDA 18
    
    #define LED_PIN 6

    // Servos for POLARIS

#endif

#define GROUNDSTATION_XBEE_ADDRESS 0x0013A200423F474C