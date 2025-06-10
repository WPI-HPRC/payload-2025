#pragma once

#define SD_SPI_SPEED SD_SCK_MHZ(50)

#include "Servo.h"

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

    #define AXON_VER_OUT_PIN A3
    #define AXON_VER_IN_PIN A4
    #define AXON_HOR_OUT_PIN A3
    #define AXON_HOR_IN_PIN A4

    #define AXON_VER_OUT_MIN 1395
    #define AXON_VER_OUT_MAX 1660
    #define AXON_HOR_OUT_MIN 1395
    #define AXON_HOR_OUT_MAX 1660
    #define AXON_KP 0.1
    #define AXON_KI 0.1
    #define AXON_KD 0.1

    #define LED_PIN PB9
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

    #define XBEE_CS 30
    #define XBEE_ATTN 33

    #define SENSOR_SCL 19
    #define SENSOR_SDA 18

    
    #define LED_PIN 6
#endif

#define GROUNDSTATION_XBEE_ADDRESS 0x0013A200423F474C