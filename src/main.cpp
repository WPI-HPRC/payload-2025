#include <Arduino.h>

#include "Context.h"
#include "Wire.h"
#include "boilerplate/Looper/Looper.h"
#include "boilerplate/Sensors/Sensor/Sensor.h"
#include "boilerplate/StateEstimator/AttEkf.h"
#include "boilerplate/StateEstimator/PVKF.h"
#include "boilerplate/Utilities/SDSerialInterface.h"
#include "states/States.h"
#include <SPI.h>
#include <boilerplate/Sensors/SensorManager/SensorManager.h>
#include <boilerplate/StateMachine/StateMachine.h>
#include "boilerplate/Sensors/Impl/AxonController/AxonController.h"

#include "config.h"

#include "telemetry/XBeeProSX.h"
#include <IWatchdog.h>

#define DEBUG_ACCEL
#define DEBUG_MAG


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
    .sd = SdFs(),
#elif defined(POLARIS)
    .accel = ICM42688_(),
    .baro = MS5611(),
    .mag = MMC5983(),
#endif
    .gps = MAX10S(),
    .flightMode = false,
    .attEkfLogger = AttEkfLogger(),
    .pvKFLogger = PVEkfLogger(),
    .xbeeLoggingDelay = 50,
    .vertFlapServo = AxonController(AXON_VER_OUT_PIN, AXON_VER_IN_PIN, AXON_KP, AXON_KI, AXON_KD, AXON_VER_OUT_MIN, AXON_VER_OUT_MAX, AXON_POT_MIN, AXON_POT_MAX),
    .horzFlapServo = AxonController(AXON_HOR_OUT_PIN, AXON_HOR_IN_PIN, AXON_KP, AXON_KI, AXON_KD, AXON_HOR_OUT_MIN, AXON_HOR_OUT_MAX, AXON_POT_MIN, AXON_POT_MAX)
};

XbeeProSX xbee = XbeeProSX(&ctx, XBEE_CS, XBEE_ATTN, GROUNDSTATION_XBEE_ADDRESS,
                           &xbee_spi);

Sensor *sensors[] = {&ctx.accel, &ctx.baro, &ctx.gps, &ctx.mag};

SensorManager sensorManager(sensors, millis);

StateMachine stateMachine((State *)new PreLaunch(&ctx));

AttStateEstimator quatEkf(ctx.mag.getData(), 0.025);
PVStateEstimator pvKF(ctx.baro.getData(), ctx.mag.getData(), ctx.gps.getData(), 0.025);

bool sd_initialized = false;

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

void mainLoop();       // Main update
void xbeeLoop();       // xbee send
void EKFLoop();        // EKF
void loggingLoop();    // Logging (not called when flightMode is set)
void occasionalLoop(); // For things like flushing SD card

Looper<FunctionsList<mainLoop, xbeeLoop, loggingLoop, occasionalLoop>,
       FunctionDelaysList<10u, 50u, 250u, 1000u>>
    looper(100, 10, TIM2);
Looper<FunctionsList<EKFLoop>, FunctionDelaysList<25u>> lowPrioLooper(1000, 11,
                                                                      TIM3);

void setup() {
#if defined(MARS)
    // P_Good pins
    pinMode(PE0, OUTPUT); // PG3V3_LED
    pinMode(PE1, OUTPUT); // PG5V_LED
    pinMode(PA3, INPUT);  // PG3V3
    pinMode(PC4, INPUT);  // PG5V

    digitalWrite(PE0, digitalRead(PA3));
    digitalWrite(PE1, digitalRead(PC4));

    pinMode(PC12, INPUT_PULLDOWN);

    if (digitalRead(PC12) == HIGH) {
        setupSDInterface(&ctx);
        return;
    }
#endif
    Serial.begin(9600);

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
    sd_initialized = ctx.sd.begin(SD_CS, SD_SPI_SPEED);
#elif defined(POLARIS)
    sd_initialized = SD.begin(SD_CS);
#endif

    if (sd_initialized) {
        int fileIdx = 0;
        char filename[100];
        while (fileIdx < 100) {
            sprintf(filename, "flightData%d.csv", fileIdx++);

            Serial.printf("Trying file `%s`\n", filename);
#if defined(MARS)
            if (!ctx.sd.exists(filename)) {
                ctx.logFile = ctx.sd.open(filename, O_RDWR | O_CREAT | O_TRUNC);
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

    if (ctx.logFile) {
        ctx.logCsvHeader();
    }

#if defined(MARS)
    xbee_spi.begin();
#endif

    xbee.start();

    looper.init();
    lowPrioLooper.init();

    IWatchdog.begin(4000000);

    ctx.vertFlapServo.init();
    ctx.horzFlapServo.init();
}

void mainLoop() {
    static uint32_t lastBaroDataLogged = 0;
    static uint32_t lastAccelDataLogged = 0;
    static uint32_t lastMagDataLogged = 0;
    static uint32_t lastGpsDataLogged = 0;
    static uint32_t lastAttKfDataLogged = 0;
    static uint32_t lastPVKfDataLogged = 0;

#if defined(MARS)
    digitalWrite(PE0, digitalRead(PA3));
    digitalWrite(PE1, digitalRead(PC4));
#endif

    stateMachine.loop();
    sensorManager.loop();

    if (sd_initialized && ctx.logFile) {
        ctx.logFile.print(millis());
        ctx.logFile.print(",");
        ctx.logFile.print(stateMachine.getCurrentStateId());
        ctx.logFile.print(",");
        ctx.logFile.print(ctx.flightMode);
        ctx.logFile.print(",");

        lastBaroDataLogged = ctx.baro.logCsvRow(ctx.logFile, lastBaroDataLogged);
        ctx.logFile.print(",");

        lastPVKfDataLogged =
            ctx.pvKFLogger.logCsvRow(ctx.logFile, lastPVKfDataLogged);
        ctx.logFile.print(",");

        ctx.logFile.print(ctx.vertFlapServo.read());
        ctx.logFile.print(",");
        ctx.logFile.print(ctx.horzFlapServo.read());
        ctx.logFile.print(",");

        ctx.logFile.println();    
    }
}

void xbeeLoop() { xbee.loop(); }

void EKFLoop() {
    static TimedPointer<MAX10SData> gpsData = ctx.gps.getData();
    static TimedPointer<LPS22Data> baroData = ctx.baro.getData();
    static bool attEkfInitialized = false;
    static bool pvInitialized = false;

    if (attEkfInitialized && !pvInitialized &&
        (gpsData->gpsLockType == 3 || gpsData->gpsLockType == 2)) {
        BLA::Matrix<6, 1> initialPV = {(float)gpsData->lat, (float)gpsData->lon, baroData->altitude, 0, 0, 0};
        pvKF.init(initialPV, ctx.attEkfLogger.getState());
        pvInitialized = true;
    }

    if (!attEkfInitialized) {
        quatEkf.init();
        attEkfInitialized = true;
    }

    auto x = quatEkf.onLoop(stateMachine.getCurrentStateId() == ID_PreLaunch);

    if (pvInitialized) {
        auto pv = pvKF.onLoop();
        noInterrupts();
        ctx.pvKFLogger.newState(pv);
        interrupts();
    }

    // disabling interrupts here may not be necessary, but it guarantees we
    // don't read context from the high priority interrupt in an invalid state,
    // since that one can preempt this one.
    noInterrupts();
    ctx.attEkfLogger.newState(x);
    interrupts();
}

void loggingLoop() {
    if (ctx.flightMode) return;
    
    static bool ledState = true;

    Serial.println(millis());
    //ctx.accel.debugLog(Serial);
    //ctx.baro.debugLog(Serial);
    //ctx.gps.debugLog(Serial);
    //ctx.mag.debugLog(Serial);
    //ctx.attEkfLogger.debugLog(Serial);
    //ctx.pvKFLogger.debugLog(Serial);

    //ctx.vertFlapServo.debugLog(Serial);
    //ctx.horzFlapServo.debugLog(Serial);

#ifdef DEBUG_ACCEL
    // Accelerometer data
    auto accelData = ctx.mag.getData();
    Serial.print(">accel_x:"); Serial.println(accelData->accelX);
    Serial.print(">accel_y:"); Serial.println(accelData->accelY);
    Serial.print(">accel_z:"); Serial.println(accelData->accelZ);
#endif

#ifdef DEBUG_BARO
    // Barometer data
    auto baroData = ctx.baro.getData();
    Serial.print(">baro_altitude:"); Serial.println(baroData->altitude);
    Serial.print(">baro_pressure:"); Serial.println(baroData->pressure);
    Serial.print(">baro_temperature:"); Serial.println(baroData->temperature);
#endif

#ifdef DEBUG_GPS
    // GPS data
    auto gpsData = ctx.gps.getData();
    Serial.print(">gps_lat:"); Serial.println(gpsData->lat, 8);
    Serial.print(">gps_lon:"); Serial.println(gpsData->lon, 8);
    Serial.print(">gps_altitude:"); Serial.println(gpsData->altitude);
    Serial.print(">gps_lock_type:"); Serial.println(gpsData->gpsLockType);
    Serial.print(">gps_satellites:"); Serial.println(gpsData->satellites);
#endif

#ifdef DEBUG_MAG
    // Magnetometer data
    auto magData = ctx.mag.getData();
    Serial.print(">gyro_x:"); Serial.println(magData->gyrX);
    Serial.print(">gyro_y:"); Serial.println(magData->gyrY);
    Serial.print(">gyro_z:"); Serial.println(magData->gyrZ);
#endif

#ifdef DEBUG_EKF
    // Attitude EKF data
    auto attState = ctx.attEkfLogger.getState();
    Serial.print(">att_quat_w:"); Serial.println(attState(0));
    Serial.print(">att_quat_x:"); Serial.println(attState(1));
    Serial.print(">att_quat_y:"); Serial.println(attState(2));
    Serial.print(">att_quat_z:"); Serial.println(attState(3));
#endif

#ifdef DEBUG_PV
    // PV KF data
    auto pvState = ctx.pvKFLogger.getState();
    Serial.print(">pv_lat:"); Serial.println(pvState(0), 8);
    Serial.print(">pv_lon:"); Serial.println(pvState(1), 8);
    Serial.print(">pv_alt:"); Serial.println(pvState(2));
    Serial.print(">pv_vel_n:"); Serial.println(pvState(3));
    Serial.print(">pv_vel_e:"); Serial.println(pvState(4));
    Serial.print(">pv_vel_d:"); Serial.println(pvState(5));
#endif

#ifdef DEBUG_SERVOS
    // Servo positions
    Serial.print(">vert_flap_pos:"); Serial.println(ctx.vertFlapServo.read());
    Serial.print(">horz_flap_pos:"); Serial.println(ctx.horzFlapServo.read());
#endif

#ifdef DEBUG_STATE
    // Current state
    Serial.print(">current_state:"); Serial.println(stateMachine.getCurrentStateId());
#endif

    if (sd_initialized && ctx.logFile) {
        ledState = !ledState;
    }
    digitalWrite(LED_PIN, ledState);
}

void occasionalLoop() { ctx.logFile.flush(); }

void loop() { handleSDInterface(&ctx); IWatchdog.reload();}
