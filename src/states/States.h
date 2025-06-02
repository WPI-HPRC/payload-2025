#pragma once

#include "../Context.h"
#include "../boilerplate/StateMachine/State.h"
#include "../boilerplate/StateMachine/StateMachine.h"
#include "../boilerplate/Utilities/Debouncer.h"
#include "FlightParams.h"
#include <Arduino.h>

enum StateId {
    ID_PreLaunch,
    ID_Boost,
    ID_Coast,
    ID_DrogueDescent,
    ID_MainDescent,
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

class Boost : public State {
    STATE_INNER(Boost)

    Debouncer burnTimeDebouncer = Debouncer(500);
    long lastAccelReadingTime = 0;
};

class Coast : public State {
    STATE_INNER(Coast)

    float prevAltitude = 0;
};

class DrogueDescent : public State {
    STATE_INNER(DrogueDescent)

    float prevAltitude = 0;
};

class MainDescent : public State {
    STATE_INNER(MainDescent)
};

class JudgeRighting : public State {
    STATE_INNER(JudgeRighting)
};

class HorizontalSide : public State {
    STATE_INNER(HorizontalSide)
};

class VerticalSide : public State {
    STATE_INNER(VerticalSide)
};

class Flail : public State {
    STATE_INNER(Flail)
};

class Tumbling : public State {
    STATE_INNER(Tumbling)
};

class ExtendingAuger : public State {
    STATE_INNER(ExtendingAuger)
};

class FullyExtendedDrilling  : public State {
    STATE_INNER(FullyExtendedDrilling)
};

class Drill : public State {
    STATE_INNER(Drill)
};

class SolidDelivery : public State {
    STATE_INNER(SolidDelivery)
};

class LiquidDelivery : public State {
    STATE_INNER(LiquidDelivery)
};

class Recovery : public State {
    STATE_INNER(Recovery)
};  

class Abort : public State {
    STATE_INNER(Abort)
};