#include <Arduino.h>

#include "Context.h"
#include "Wire.h"
#include "boilerplate/Sensors/Sensor/Sensor.h"
#include "pb.h"
#include "states/States.h"
#include <SPI.h>
#include <boilerplate/Sensors/SensorManager/SensorManager.h>
#include <boilerplate/StateMachine/StateMachine.h>

#include "config.h"

#include "telemetry/XBeeProSX.h"

#if defined(MARS)
SdFat sd;
#endif

#define SD_SPI_SPEED SD_SCK_MHZ(50)

#if defined(MARS)
SPIClass xbee_spi(XBEE_MOSI, XBEE_MISO, XBEE_SCLK);
#elif defined(POLARIS)
SPIClass xbee_spi = SPI;
#endif

Context ctx = {
#if defined(MARS)
    .accel = ASM330(),
    .baro = LPS22(),
    .mag = ICM20948(),
#elif defined(POLARIS)
    .accel = ICM42688_(),
    .baro = MS5611(),
    .mag = MMC5983(),
#endif
    .gps = MAX10S(),
    .auger = ServoController(AUGER),
    .rack_pinion = ServoController(RACK_PINION),
    .pinion_door_1 = ServoController(PINION_DOOR_1),
    .pinion_door_2 = ServoController(PINION_DOOR_2),
    .lead_screw = ServoController(LEAD_SCREW),
    .axon1 = ServoController(AXON_1_IN, AXON_1_OUT),
    .axon2 = ServoController(AXON_2_IN, AXON_2_OUT),
    .flightMode = false,
};

XbeeProSX xbee =
    XbeeProSX(&ctx, XBEE_CS, XBEE_ATTN, GROUNDSTATION_XBEE_ADDRESS, &xbee_spi);

#if defined(MARS)
Sensor *sensors[] = {&ctx.accel, &ctx.baro, &ctx.gps, &ctx.mag};
#elif defined(POLARIS)
Sensor *sensors[] = {&ctx.accel, &ctx.baro, &ctx.mag, &ctx.gps};
#endif

SensorManager<decltype(&millis), sizeof(sensors) / sizeof(Sensor *)>
    sensorManager(sensors, millis);

StateMachine stateMachine((State *)new PreLaunch(&ctx));

bool sd_initialized = false;
long lastTime = 0;
bool state = true;

long lastFlush = 0;

uint8_t error_code;

// Outputs the bits in the byte `data` in MSB order over `pin`
void output_byte(uint8_t data, uint pin) {
    digitalWrite(pin, 1);

    delay(50);
    digitalWrite(pin, 0);

    delay(50);

    digitalWrite(pin, 1);
    delay(50);
    digitalWrite(pin, 0);
    for (int bit = 0; bit < 8; bit++) {
        digitalWrite(pin, (data >> (7 - bit)) & 1);
        delay(100);
    }

    digitalWrite(pin, 0);

    delay(1000);
}

void setup() {
#if defined(MARS)
    // P_Good pins
    pinMode(PE0, OUTPUT); // PG3V3_LED
    pinMode(PE1, OUTPUT); // PG5V_LED
    pinMode(PA3, INPUT);  // PG3V3
    pinMode(PC4, INPUT);  // PG5V

    digitalWrite(PE0, digitalRead(PA3));
    digitalWrite(PE1, digitalRead(PC4));
#endif
    Serial.begin(9600);

    Wire.setSCL(SENSOR_SCL);
    Wire.setSDA(SENSOR_SDA);
    Wire.begin();

#if defined(MARS)
    SPI.setSCLK(SD_SCLK);
#elif defined(POLARIS)
    SPI.setSCK(SD_SCLK);
#endif
    SPI.setMISO(SD_MISO);
    SPI.setMOSI(SD_MOSI);
    SPI.begin();

    // while (!Serial)
    //     delay(5);

    stateMachine.initialize();
    sensorManager.sensorInit();

    Wire.setClock(400000);

    pinMode(LED_PIN, OUTPUT);

#if defined(MARS)
    sd_initialized = sd.begin(SD_CS, SD_SPI_SPEED);
    // error_code = sd.card()->errorCode();

    // Serial.printf("%d %d\n", sd_initialized, error_code);
#elif defined(POLARIS)
    sd_initialized = SD.begin(SD_CS);
#endif

    if (sd_initialized) {
        int fileIdx = 0;
        char filename[100];
        while (fileIdx < 100) {
            sprintf(filename, "flightData%d.bin", fileIdx++);

            Serial.printf("Trying file `%s`\n", filename);
#if defined(MARS)
            if (!sd.exists(filename)) {
                ctx.logFile = sd.open(filename, O_RDWR | O_CREAT | O_TRUNC);
                break;
            }
#elif defined(POLARIS)
            if (!SD.exists(filename)) {
                ctx.logFile = SD.open(filename, FILE_WRITE_BEGIN);
                break;
            }
#endif
        }
    }

#if defined(MARS)
    xbee_spi.begin();
#endif

    xbee.start();

    lastTime = millis();
    lastFlush = millis();
}

void loop() {
#if defined(MARS)
    digitalWrite(PE0, digitalRead(PA3));
    digitalWrite(PE1, digitalRead(PC4));
#endif
    stateMachine.loop();
    sensorManager.loop();
    xbee.loop();

    // ctx.accel->debugPrint(file);
    // ctx.baro->debugPrint(file);
    // ctx.gps->debugPrint(Serial);

    // file.println(millis());
    long now = millis();
    if (now - lastTime >= 250) {
        lastTime = now;

        ctx.accel.debugPrint(Serial);
        ctx.baro.debugPrint(Serial);
        ctx.gps.debugPrint(Serial);
        ctx.mag.debugPrint(Serial);
        // Serial.print("Flight mode: "); Serial.println(ctx.flightMode);
        ctx.auger.init();
        ctx.rack_pinion.init();
        ctx.pinion_door_1.init();
        ctx.pinion_door_2.init();
        ctx.lead_screw.init();
        ctx.axon1.init();
        ctx.axon2.init();
        
        if (sd_initialized) {
            state = !state;
            ctx.accel.debugPrint(ctx.logFile);
            ctx.baro.debugPrint(ctx.logFile);
            ctx.gps.debugPrint(ctx.logFile);
            ctx.mag.debugPrint(ctx.logFile);
        }
    }
    digitalWrite(LED_PIN, state);

    if (now - lastFlush >= 1000) {
        lastFlush = now;
        ctx.logFile.flush();
    }

    delay(1);
}
