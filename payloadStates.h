#include <Context.h>
#include <State.h>
//This is a raw dogged state file bc the platformio project was not building on the plane w/o internet sorry


// NOTE: I literally directly copied all of the different variables in the logs bc i didnt know which telemtry data we want to pick and place
// additionally i believe some of these variables shouldnt be sensor data anymore, like state, timestep, loop count but also i dont know and would like feedback
#define SENSOR_DATA(X)                                                         \ 
  X(float, accelX) /*telemetry! */                                                           \
  X(float, accelY)                                                             \
  X(float, accelZ)                                                             \
  X(float, altitude)                                                             \
  X(float, epochTime)                                                             \
  X(float, gpsAltAGL)                                                             \
  X(float, gpsLat)                                                             \
  X(float, gpsLock)                                                             \
  X(float, gpsLong)                                                         \
  X(float, gpsVelocityX)                                                             \
  X(float, gpsVelocityY)                                                             \
  X(float, gpsVelocityZ)                                                             \
  X(float, gyroX)                                                             \
  X(float, gyroY)                                                             \
  X(float, gyroZ)                                                             \
  X(float, i)                                                             \
  X(float, j)                                                             \
  X(float, k)                                                             \
  X(float, w)                                                             \
  X(float, launchAltitude)                                                             \
  X(long, loopCount)                                                             \
  X(float, magX)                                                             \
  X(float, magY)                                                             \
  X(float, magZ)                                                             \
  X(float, posX)                                                             \
  X(float, posY)                                                             \
  X(float, posZ)                                                             \
  X(float, pressure)                                                             \
  X(float, rawMagX)                                                             \
  X(float, rawMagY)                                                             \
  X(float, rawMagZ)                                                             \
  X(long, satellites)                                                             \
  X(long, state)                                                             \
  X(float, temperature)                                                             \
  X(long, timestamp)                                                             \
  X(float, velX)                                                             \
  X(float, velY)                                                             \
  X(float, velZ)                                                             \
  X(float, top_sr_axon_pos)                                                  \
  X(float, left_sr_axon_pos)                                                  \
  X(float, right_sr_axon_pos)                                                  \
  X(bool, doorMechServo_extended) /*theres no encoder on this but maybe we want to store their likely pos?*/ \
  X(bool, deliveryServo_extended) /*same as above*/ \
  X(bool, valve_open) /*same as above*/ \
  X(bool, drillPinion_extended) /*same as above*/ \
  X(bool, DRILL_BABY_DRILL_ON) /*same as above*/ /*im sorry for the variable name i need to keep myself entertained on this plane ride somehow and i wish there was a speaker that would just announce a DRILL BABY DRILLL from a cliip of trump whenever the auger servo turned on*/\
  X(bool, inBush) \ //software boolean for state machine
  X(float, ir_reading) \
  X(bool, limit_switch1) /*idk what these exactly do yet bc i thought there was supposed to be 3 limit switches for delivery*/\
  X(bool, limit_switch2) \
  X(float, extend_current_sensor) \
  X(float, auger_current_sensor)  \

CREATE_CONTEXT(SENSOR_DATA);

extern State<Context> PreLaunch_Init_Sensors;
extern State<Context> PreLaunch_Start_Telem;
extern State<Context> PreLaunch_Stable;
extern State<Context> Launch_Burn;
extern State<Context> Coast;
extern State<Context> Drogue_Descent;
extern State<Context> Main_Descent;
extern State<Context> Judge_Righting;
extern State<Context> Right_Side_Push;
extern State<Context> Left_Side_Push;
extern State<Context> Top_Side_Push;
extern State<Context> Tumbling;
extern State<Context> Its_So_Over;
extern State<Context> Drill_Extend;
extern State<Context> DRILL_BABY_DRILL;
extern State<Context> Drilling_Fully_Extended;
extern State<Context> Delivery_Door_Open;
extern State<Context> Planting;
extern State<Context> Valves_Open;
extern State<Context> Water_Push;
extern State<Context> Recovery;




