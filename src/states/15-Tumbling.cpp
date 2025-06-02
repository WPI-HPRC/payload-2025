#include "States.h"

void Tumbling::initialize_impl() {
    Serial.println("Tumbling initialized!");
}

State *Tumbling::loop_impl() {
    Serial.println("Tumbling looped");
    return nullptr;
} 