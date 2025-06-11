#pragma once

#include "../Context.h"
#include "../boilerplate/StateMachine/State.h"
#include "../boilerplate/StateMachine/StateMachine.h"
#include "../boilerplate/Utilities/Debouncer.h"
#include "../boilerplate/Utilities/RunningExpAverage.h"
#include "../boilerplate/Utilities/MultipleStateDebouncer.h"
#include "../boilerplate/Utilities/TimeAverage.h"
#include "FlightParams.h"
#include <Arduino.h>

enum StateId {
    ID_PreLaunch,
    ID_Coast,
    ID_Descent,
    ID_JudgeRighting,
    ID_HorizontalSide,
    ID_VerticalSide,
    ID_Flail,
    ID_Tumbling,
    ID_ExtendingAuger,
    ID_FullyExtendedDrilling,
    ID_Drill,
    ID_SolidDelivery,
    ID_LiquidDelivery,
    ID_SolidEjection,
    ID_Recovery,
    ID_Abort,
    ID_PotentiometerTest
};

using State = TState<Context, StateId, decltype(&millis)>;
using StateMachine = TStateMachine<Context, StateId, decltype(&millis)>;

#define STATE_INNER(name)                                                      \
  public:                                                                      \
    name(Context *ctx) : State(ID_##name, ::millis, ctx) {}                    \
                                                                               \
  private:                                                                     \
    void initialize_impl() override;                                           \
    State *loop_impl() override;

class PreLaunch : public State {
    STATE_INNER(PreLaunch)

    Debouncer accelDebouncer = Debouncer(500);
    uint32_t lastAccelReadingTime = 0;
    TimeAverage<float, 50> altAverager{};
    uint32_t lastBaroReadingTime = 0;
    bool savedInitialAltitude = false;

    RunningExpAverage<double> gyZBiasAvg = RunningExpAverage(0.1);
};

class Coast : public State {
    STATE_INNER(Coast)

    RunningExpAverage<float> ewma{0.3};
    bool firstVelCalculated = false;
    float prevAltitude = 0;
    Debouncer velDebouncer = Debouncer(100);
    uint32_t lastBaroReadingTime = 0;
};

class Descent : public State {
    STATE_INNER(Descent)

    RunningExpAverage<float> ewma{0.1};
    float prevAltitude = 0;
    bool firstVelCalculated = false;
    Debouncer velDebouncer = Debouncer(50);
    uint32_t lastBaroReadingTime = 0;
};

class JudgeRighting : public State {
    STATE_INNER(JudgeRighting)

    // Enum to represent which side is on the ground
    enum class GroundSide { //FIXME: Check axis
        TOP,        // Y+ side down
        BOTTOM,     // Y- side down
        LEFT,       // X- side down
        RIGHT,      // X+ side down
        UNKNOWN     // No side is clearly down
    };

    MultipleStateDebouncer<GroundSide> judgeRightingDebouncer = MultipleStateDebouncer<GroundSide>(500, 4, GroundSide::UNKNOWN); //TODO: check debouncer timings
    long lastMagReadTime = 0;

    GroundSide getGroundSide(const float& accelX, const float& accelY);
    const char* groundSideToString(GroundSide side);
};

class HorizontalSide : public State {
    STATE_INNER(HorizontalSide)

    Debouncer isRotatingDebouncer = Debouncer(100); //TODO: is this too long? too short? needs checking
    long lastGyroReadTime = 0;

};

class VerticalSide : public State {
    STATE_INNER(VerticalSide)

    Debouncer isRotatingDebouncer = Debouncer(100); //TODO: is this too long? too short? needs checking
    long lastGyroReadTime = 0;


};

class Tumbling : public State {
    STATE_INNER(Tumbling)

    Debouncer tumblingDebouncer = Debouncer(500);
    long lastGyroReadTime = 0;
};

class ExtendingAuger : public State {
    STATE_INNER(ExtendingAuger)
};

class FullyExtendedDrilling  : public State {
    STATE_INNER(FullyExtendedDrilling)
};

class SolidDelivery : public State {
    STATE_INNER(SolidDelivery)
};

class LiquidDelivery : public State {
    STATE_INNER(LiquidDelivery)
};

class SolidEjection : public State {
    STATE_INNER(SolidEjection)
};

class Recovery : public State {
    STATE_INNER(Recovery)
};  

class Abort : public State {
    STATE_INNER(Abort)
};

class PotentiometerTest : public State {
    STATE_INNER(PotentiometerTest)
};