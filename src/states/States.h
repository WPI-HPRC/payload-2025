#pragma once

#include "../Context.h"
#include "../boilerplate/StateMachine/State.h"
#include "../boilerplate/StateMachine/StateMachine.h"
#include "../boilerplate/Utilities/Debouncer.h"
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
    ID_Abort
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

    Debouncer launchAccelDebouncer = Debouncer(500);
    long lastAccelReadingTime = 0;
};

class Coast : public State {
    STATE_INNER(Coast)

    constexpr static float alpha = 0.1; // smoothing coefficient. 0 <= alpha <= 1. Values near 0 prioritize old values (more smoothing) and values near 1 prioritize new values (less smoothing).
    bool firstVelCalculated = false;
    float prevAltitude = 0;
    float avgBaroVel = 0;
    Debouncer coastVelDebouncer = Debouncer(100);
    uint32_t lastBaroReadingTime = 0;
};

class Descent : public State {
    STATE_INNER(Descent)

    constexpr static float alpha = 0.3; // smoothing coefficient. 0 <= alpha <= 1. Values near 0 prioritize old values (more smoothing) and values near 1 prioritize new values (less smoothing).
    float prevAltitude = 0;
    bool firstVelCalculated = false;
    float avgBaroVel = 0;
    uint32_t lastBaroReadingTime = 0;
};

class JudgeRighting : public State {
    STATE_INNER(JudgeRighting)
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