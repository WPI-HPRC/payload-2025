#include "States.h"

void Abort::initialize_impl() {
  Serial.println("Abort initialized!");
}

State *Abort::loop_impl() {
    Serial.println("Abort looped");
    
    return nullptr;
}
