#include "States.h"

void Recovery::initialize_impl() {
  //TODO: Assert all actuators off here to reduce overheating and unneccessary power draw. - Amber

  Serial.println("Recovery initialized!");
}

State *Recovery::loop_impl() {
    Serial.println("Recovery looped");

    return nullptr;
}
