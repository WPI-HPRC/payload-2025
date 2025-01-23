#include "payloadStates.h"

/* Used for reference
template <typename C> struct Transition {
  Condition<C> cond;
  std::initializer_list<int> actuatorPushCmds;
  std::initializer_list<int> actuatorPopCmds;
  std::initializer_list<UtilityCommand> utilityPushCmds;
  std::initializer_list<int> utilityPopCmds;
  State<C> *targetState;

State PreLaunch({
  Transition(
    And(Value<bool>(true), Value<bool>(false)),
    {},
    {},
    {
      DebouncerCommand(1, 10,
        LessThan(Get<float, accelX>(), Value<float>(200.f))),
    },
    {},
    &Boost
  )
});

*/
// This is gonna be a super file LOL
/*
* Transition Target States: PreLaunch_start telem
*/
State PreLaunch_Init_Sensors({ //Note: idk how this state changes and i can't be bothered ill just copy rockets code later
    Transition(
        And(Value<bool>(true), Value<bool>(true)), //always true
        {},
        {},
        {
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, accelX>(), Value<float>(4))) //is this right? I'm assuming its in g's
            //Note for Dashiell; We need an absolute value operator for the above issue^
        },
        {},
        &PreLaunch_Start_Telem
    )
});

/*
* Transition Target States: Launch_Burn
*/
State PreLaunch_Start_Telem({
    Transition(
        And(Get<DEBOUNCER_ID_1>(), Value<bool>(true)), //Do we need to and this with a true bool? im not really sure and im tired ive been working for hours
        {}, 
        {},
        {
            TimerCommand(TIMER_ID_2, BURN_TIMER_LENGTH), //I'm going to define these all as macros to be tweaked at a later time
            DebouncerCommand(DEBOUNCER_ID_2, 10, LessThan(Get<float, accelX>(), Value<float>(0.3))) //Is this in g's? I'm assuming so
        },
        {
            DEBOUNCER_ID_1
        }, 
        &Launch_Burn
    ),
    Transition(
        Not(And(Get<DEBOUNCER_ID_1>(), Value<bool>(true))), //Do we need to and this with a true bool? im not really sure and im tired ive been working for hours
        {}, 
        {},
        {},
        {},
        &PreLaunch_Start_Telem
    )
});

State Launch_Burn({
    Transition(
        And(Get<DEBOUNCER_ID_2>(), Get<TIMER_ID_2>()),
        {},
        {},
        {
            TimerCommand(TIMER_ID_1, COAST_TIMER_LENGTH), //I'm going to define these all as macros to be tweaked at a later time
            DebouncerCommand(DEBOUNCER_ID_1, 10, LessThan(Get<float, velX>(), Value<float>(0.f))) //Is this in m/s? I'm assuming so
        }
        {
            DEBOUNCER_ID_2, TIMER_ID_2
        }, 
        &Coast
    ),
    Transition(
        Not(And(Get<DEBOUNCER_ID_2>(), Get<TIMER_ID_2>())), 
        {}, 
        {},
        {},
        {},
        &Launch_Burn
    )

});

State Coast({
    Transition(
        Or(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>()), //Do we still need timers? Idk NEEDS FEEDBACK 
        {},
        {},
        {
            TimerCommand(TIMER_ID_2, DROGUE_DESCENT_TIMER_LENGTH), 
            DebouncerCommand(DEBOUNCER_ID_2, 10, LessThan(Get<float, velX>(), Value<float>(-23.f))) //Is this in m/s? I'm assuming so
        }
        {
            DEBOUNCER_ID_1, TIMER_ID_1
        }, 
        &Drogue_Descent
    ),
    Transition(
        Not(Or(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>())), 
        {}, 
        {},
        {},
        {},
        &Coast
    )
});

State Drogue_Descent({
    Transition(
        Or(Get<DEBOUNCER_ID_2>(), Get<TIMER_ID_2>()), //Do we still need timers? Idk NEEDS FEEDBACK
        {},
        {},
        {
            TimerCommand(TIMER_ID_1, MAIN_DESCENT_TIMER_LENGTH), 
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, velX>(), Value<float>(-5.f))) //Is this in m/s? I'm assuming so
        }
        {
            DEBOUNCER_ID_2, TIMER_ID_2
        }, 
        &Main_Descent
    ),
    Transition(
        Not(Or(Get<DEBOUNCER_ID_2>(), Get<TIMER_ID_2>())), 
        {}, 
        {},
        {},
        {},
        &Drogue_Descent
    )
});


// NOTE!!! idk what the sensor values are for angular rotation around the x-axis. I'm going to assume its the quaternion reading, w, but this needs to be fixed for every righting state
State Main_Descent({
    Transition(
        And(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>()), //Do we still need timers? Idk NEEDS FEEDBACK
        {},
        {},
        {
            //to check if its so over -_-
            TimerCommand(TIMER_ID_2, RIGHTING_TIMER_LENGTH), 
            //For Belly Down
            DebouncerCommand(DEBOUNCER_ID_2, 10, GreaterThan(Get<float, roll>(), Value<float>(-45.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_3, 10, LessThan(Get<float, roll>(), Value<float>(45.f))),
            //For Right Side Down
            DebouncerCommand(DEBOUNCER_ID_4, 10, GreaterThan(Get<float, roll>(), Value<float>(-135.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_5, 10, LessThan(Get<float, roll>(), Value<float>(-45.f))),
            //For Left Side Down
            DebouncerCommand(DEBOUNCER_ID_6, 10, GreaterThan(Get<float, roll>(), Value<float>(45.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_7, 10, LessThan(Get<float, roll>(), Value<float>(135.f))),
            //For Top Side Down
            DebouncerCommand(DEBOUNCER_ID_8, 10, GreaterThan(Get<float, roll>(), Value<float>(135.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_9, 10, LessThan(Get<float, roll>(), Value<float>(-135.f))),
            //IR Debouncer
            DebouncerCommand(DEBOUNCER_ID_10, 10, LessThan(Get<float, ir_sensor>(), Value<float>(IR_BLACK_THRESHOLD)))
        }
        {
            DEBOUNCER_ID_1, TIMER_ID_1
        }, 
        &Judge_Righting
    ),
    Transition(
        Not(And(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>())), 
        {}, 
        {},
        {},
        {},
        &Main_Descent
    )
});

State Judge_Righting({
    Transition( //For on RIGHT SIDE PUSH
        And(Get<DEBOUNCER_ID_4>(), Get<DEBOUNCER_ID_5>()), 
        {
            ActuatorCommand(RIGHT_SERVO_ID, SERVO_PUSH_SPEED) //this is not actually implemented in the state machine code yet but its a good place holder
        },
        {},
        {
            TimerCommand(TIMER_ID_1, SERVO_PUSH_TIMER_LENGTH),
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, roll>(), Value<float>(-0.5))),
            DebouncerCommand(DEBOUNCER_ID_11, 10, LessThan(Get<float, roll>(), Value<float>(0.5))),
        },
        {
            DEBOUNCER_ID_2, DEBOUNCER_ID_3, DEBOUNCER_ID_4,
            DEBOUNCER_ID_5, DEBOUNCER_ID_6, DEBOUNCER_ID_7,
            DEBOUNCER_ID_8, DEBOUNCER_ID_9, DEBOUNCER_ID_10,
            TIMER_ID_2
        }, 
        &Right_Side_Push
    ),
    Transition( //For on Left SIDE PUSH
        And(Get<DEBOUNCER_ID_6>(), Get<DEBOUNCER_ID_7>()), 
        {
            ActuatorCommand(LEFT_SERVO_ID, SERVO_PUSH_SPEED) //this is not actually implemented in the state machine code yet but its a good place holder
        },
        {},
        {
            TimerCommand(TIMER_ID_1, SERVO_PUSH_TIMER_LENGTH),
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, roll>(), Value<float>(-0.5))),
            DebouncerCommand(DEBOUNCER_ID_11, 10, LessThan(Get<float, roll>(), Value<float>(0.5))),
        },
        {
            DEBOUNCER_ID_2, DEBOUNCER_ID_3, DEBOUNCER_ID_4,
            DEBOUNCER_ID_5, DEBOUNCER_ID_6, DEBOUNCER_ID_7,
            DEBOUNCER_ID_8, DEBOUNCER_ID_9, DEBOUNCER_ID_10,
            TIMER_ID_2
        }, 
        &Left_Side_Push
    ),
    Transition( //For on Top SIDE PUSH
        And(Get<DEBOUNCER_ID_8>(), Get<DEBOUNCER_ID_9>()), 
        {
            ActuatorCommand(TOP_SERVO_ID, SERVO_PUSH_SPEED) //this is not actually implemented in the state machine code yet but its a good place holder
        },
        {},
        {
            TimerCommand(TIMER_ID_1, SERVO_PUSH_TIMER_LENGTH),
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, roll>(), Value<float>(-0.5))),
            DebouncerCommand(DEBOUNCER_ID_11, 10, LessThan(Get<float, roll>(), Value<float>(0.5))),
        },
        {
            DEBOUNCER_ID_2, DEBOUNCER_ID_3, DEBOUNCER_ID_4,
            DEBOUNCER_ID_5, DEBOUNCER_ID_6, DEBOUNCER_ID_7,
            DEBOUNCER_ID_8, DEBOUNCER_ID_9, DEBOUNCER_ID_10,
            TIMER_ID_2
        }, 
        &Top_Side_Push
    ),
    Transition(
        And(And(Get<DEBOUNCER_ID_2>(), Get<DEBOUNCER_ID_3>()), Get<DEBOUNCER_ID_10>()),
        {
            ActuatorCommand(EXTEND_SERVO_ID, EXTEND_SERVO_SPEED)
        },
        {},
        {
            TimerCommand(TIMER_ID_1, SPIN_TIMER_LENGTH),
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, extend_current_sensor>(), Value<float>(500))), //needs changing currently limit is 500 mA
            DebouncerCommand(DEBOUNCER_ID_11, 10, And(Get<bool, ))
        }
    )
});
