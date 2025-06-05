#pragma once

//TODO: CHECK EVERY PARAM! I want to see a nice green checkmark emoji when we confirm testing on a param! ✅


#include <cstdint>
constexpr float LAUNCH_THRESHHOLD_G = 5;
constexpr float BURN_THRESHHOLD_G = 0.3;
constexpr float IS_TUMBLING_VEL_THRESHOLD = 0.174; //10 dps in rad/s... idk the units for the gyro and this will obv need testing. Upper bound of hysterisis
constexpr float IS_END_TUMBLING_VEL_THRESHOLD = 0.02; //made up number... lower bound of hysterisis to signify stop moving/stable
constexpr uint32_t MAX_BOOST_TIME = 10000;
constexpr uint32_t MAX_TUMBLE_TIME = 300000; // 5 minutes
constexpr uint32_t MAX_TRY_BEFORE_FLAIL_TIME = 30000; // 30 seconds
constexpr uint32_t MAX_AUG_EXT_TIME = 120000; //2 minute
constexpr uint32_t MAX_DRILL_TIME = 60000; //1 minute
constexpr uint32_t MAX_SOLID_DOOR_OPEN_TIME = 5000; //5 seconds
constexpr uint32_t MAX_SOLID_EJECTION_TIME = 20000; //20 seconds
constexpr uint32_t MAX_LIQUID_DELIVERY_TIME = 20000; //20 seconds

constexpr uint8_t MAX_FLAIL_ATTEMPTS = 4;

constexpr uint32_t FLAP_EXTENDED_POS = 1500; //in PWM for fully extended flap
constexpr uint32_t FLAP_RETRACTED_POS = 1000; //in PWM for fully retracted flap
constexpr uint32_t AUGER_MAX_EXT_POS = 1500; //in PWM
constexpr uint32_t AUGER_MAX_CLOSED_POS = 1000; //in PWM
constexpr uint32_t AUGER_EXT_POS_BAND =  50;
constexpr uint32_t SOLID_DELIVERY_DOOR_OPEN_POS= 1500; //in PWM
constexpr uint32_t SOLID_DELIVERY_DOOR_CLOSED_POS = 1000; //in PWM
constexpr uint32_t SOLID_EJECTION_POS = 1500; //in PWM
constexpr uint32_t LIQUID_DELIVERY_POS = 1500; //in PWM

constexpr float DRILL_SPEED = 0.5; //im guessing m/s?
