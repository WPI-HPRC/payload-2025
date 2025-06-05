#pragma once

//TODO: CHECK EVERY PARAM! I want to see a nice green checkmark emoji when we confirm testing on a param! ✅


constexpr float LAUNCH_THRESHHOLD_G = 5;
constexpr float BURN_THRESHHOLD_G = 0.3;
constexpr float IS_TUMBLING_VEL_THRESHOLD = 0.174; //10 dps in rad/s... idk the units for the gyro and this will obv need testing. Upper bound of hysterisis
constexpr uint32_t MAX_BOOST_TIME = 10000;
constexpr uint32_t MAX_TUMBLE_TIME = 300000; // 5 minutes

constexpr uint32_t FLAP_EXTENDED_POS = 1500; //in PWM for fully extended flap
constexpr uint32_t FLAP_RETRACTED_POS = 1000; //in PWM for fully retracted flap
