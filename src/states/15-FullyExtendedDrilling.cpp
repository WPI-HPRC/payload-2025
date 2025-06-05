#include "States.h"

void FullyExtendedDrilling::initialize_impl() {
    Serial.println("FullyExtendedDrilling initialized!");
}
 
State *FullyExtendedDrilling::loop_impl() {
    Serial.println("FullyExtendedDrilling looped");
    return nullptr;
} 