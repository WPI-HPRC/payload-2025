#include "States.h"

void Flail::initialize_impl() {
  Serial.println("Flail initialized!");
}

State *Flail::loop_impl() {
  Serial.println("Flail looped");
  // Access 

  return nullptr;
}
