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
        ? Idk what would make it transition,
        {},
        {},
        {
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, accelX>(), Value<float>(4))) //is this right? I'm assuming its in g's
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
        {}, //Note for Dashiell; We need an absolute value operator for the above issue^
        {},
        {
            TimerCommand(TIMER_ID_1, BURN_TIMER_LENGTH), //I'm going to define these all as macros to be tweaked at a later time
            DebouncerCommand(DEBOUNCER_ID_1, 10, LessThan(Get<float, accelX>(), Value<float>(0.3))) //Is this in g's? I'm assuming so
        },
        {}, //I'm 99% sure I'm supposed to be pulling utility but I don't have the flow chart and I forgot I'm sorry everyone this is all going to be wrong :(
        //Is the pop supposed to be used to pull/reset util? I forgotttttttttt
        &Launch_Burn
    )
});

State Launch_Burn({
    Transition(
        And(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>()),
        {},
        {},
        {
            TimerCommand(TIMER_ID_1, COAST_TIMER_LENGTH), //I'm going to define these all as macros to be tweaked at a later time
            DebouncerCommand(DEBOUNCER_ID_1, 10, LessThan(Get<float, velX>(), Value<float>(0.f))) //Is this in m/s? I'm assuming so
        }
        {}, //I'm 99% sure I'm supposed to be pulling utility but I don't have the flow chart and I forgot I'm sorry everyone this is all going to be wrong :(
        //Is the pop supposed to be used to pull/reset util? I forgotttttttttt
        &Coast
    )
});

State Coast({
    Transition(
        Or(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>()), //Do we still need timers? Idk NEEDS FEEDBACK 
        {},
        {},
        {
            TimerCommand(TIMER_ID_1, DROGUE_DESCENT_TIMER_LENGTH), 
            DebouncerCommand(DEBOUNCER_ID_1, 10, LessThan(Get<float, velX>(), Value<float>(-23.f))) //Is this in m/s? I'm assuming so
        }
        {}, //I'm 99% sure I'm supposed to be pulling utility but I don't have the flow chart and I forgot I'm sorry everyone this is all going to be wrong :(
        //Is the pop supposed to be used to pull/reset util? I forgotttttttttt
        &Drogue_Descent
    )
});

State Drogue_Descent({
    Transition(
        Or(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>()), //Do we still need timers? Idk NEEDS FEEDBACK
        {},
        {},
        {
            TimerCommand(TIMER_ID_1, MAIN_DESCENT_TIMER_LENGTH), 
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, velX>(), Value<float>(-5.f))) //Is this in m/s? I'm assuming so
        }
        {}, //I'm 99% sure I'm supposed to be pulling utility but I don't have the flow chart and I forgot I'm sorry everyone this is all going to be wrong :(
        //Is the pop supposed to be used to pull/reset util? I forgotttttttttt
        &Main_Descent
    )
});


// NOTE!!! idk what the sensor values are for angular rotation around the x-axis. I'm going to assume its the quaternion reading, w, but this needs to be fixed for every righting state
State Main_Descent({
    Transition(
        And(Get<DEBOUNCER_ID_1>(), Get<TIMER_ID_1>()), //Do we still need timers? Idk NEEDS FEEDBACK
        {},
        {},
        {
            TimerCommand(TIMER_ID_1, RIGHTING_TIMER_LENGTH), 
            //For Belly Down
            DebouncerCommand(DEBOUNCER_ID_1, 10, GreaterThan(Get<float, roll>(), Value<float>(-45.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_2, 10, LessThan(Get<float, roll>(), Value<float>(45.f))),
            //For Right Side Down
            DebouncerCommand(DEBOUNCER_ID_3, 10, GreaterThan(Get<float, roll>(), Value<float>(-135.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_4, 10, LessThan(Get<float, roll>(), Value<float>(-45.f))),
            //For Left Side Down
            DebouncerCommand(DEBOUNCER_ID_5, 10, GreaterThan(Get<float, roll>(), Value<float>(45.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_6, 10, LessThan(Get<float, roll>(), Value<float>(135.f))),
            //For Top Side Down
            DebouncerCommand(DEBOUNCER_ID_7, 10, GreaterThan(Get<float, roll>(), Value<float>(135.f))), //I don't have access to the direction of which way is up but I'm going to use the euler angles? this needs discussion with dan I'm too unfamiliar with the quaternion at this current moment to do this correctly
            DebouncerCommand(DEBOUNCER_ID_8, 10, LessThan(Get<float, roll>(), Value<float>(-135.f))),
            //IR Debouncer
            DebouncerCommand(DEBOUNCER_ID_9, 10, LessThan(Get<float, ir_sensor>(), Value<float>(IR_BLACK_THRESHOLD)))
        }
        {}, //I'm 99% sure I'm supposed to be pulling utility but I don't have the flow chart and I forgot I'm sorry everyone this is all going to be wrong :(
        //Is the pop supposed to be used to pull and reset util? I forgotttttttttt
        &Judge_Righting
    )
});

State Judge_Righting({
    Transition(
        And(Get<DEBOUNCER_ID_3>(), Get<DEBOUNCER_ID_4>()),
        {
            ActuatorCommand(RIGHT_SERVO_ID, SERVO_PUSH_SPEED) //this is not actually implemented in the state machine code yet but its a good place holder
        },
        {},
        {
            TimerCommand(TIMER_ID_2, SERVO_PUSH_TIMER_LENGTH),
        },
        {}, //I'm 99% sure I'm supposed to be pulling utility but I don't have the flow chart and I forgot I'm sorry everyone this is all going to be wrong :(
        //Is the pop supposed to be used to pull and reset util? I forgotttttttttt
        &Right_Side_Push
    )
});
