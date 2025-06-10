#include "States.h"

void Recovery::initialize_impl() {
  Serial.println("Recovery initialized!");
}

State *Recovery::loop_impl() {
    Serial.println("Recovery looped");

    return nullptr;
}
