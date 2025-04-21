#pragma once

#include "../Context.h"
#include "../boilerplate/StateMachine/State.h"
#include "../boilerplate/StateMachine/StateMachine.h"
#include <Arduino.h>

enum StateId {
    ID_PreLaunch,
    ID_Boost,
    ID_Coast,
    ID_DrogueDescent,
    ID_MainDescent,
    ID_JudgeRighting,
    ID_VerticalSide,
    ID_HorizontalSide,
    ID_Flail,
    ID_ExtendAuger,
    ID_Drill,
    ID_SolidDelivery,
    ID_LiquidDelivery,
    ID_Recovery,
    ID_Abort
};

using State = TState<Context, StateId, decltype(&millis)>;
using StateMachine = TStateMachine<Context, StateId, decltype(&millis)>;

#define STATE(name)                                                            \
    class name : public State {                                                \
      public:                                                                  \
        name(Context *ctx) : State(ID_##name, ::millis, ctx) {}                \
                                                                               \
      private:                                                                 \
        void initialize_impl() override;                                       \
        State *loop_impl() override;                                           \
    }

STATE(PreLaunch);

STATE(Boost);

STATE(Coast);

STATE(DrogueDescent);

STATE(MainDescent);

STATE(JudgeRighting);

STATE(VerticalSide);

STATE(HorizontalSide);

STATE(Flail);

STATE(ExtendAuger);

STATE(Drill);

STATE(SolidDelivery);

STATE(LiquidDelivery);

STATE(Recovery);

STATE(Abort);
