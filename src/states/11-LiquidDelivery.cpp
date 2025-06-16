#include "States.h"

void LiquidDelivery::initialize_impl() {
  Serial.println("Liquid Delivery initialized!");
}

State *LiquidDelivery::loop_impl() {
    Serial.println("Liquid Delivery looped");

    auto const magData = ctx->mag.getData();
    
    if(currentTime < 10000){

      GroundSide currSide = getGroundSide(magData->accelX, magData->accelY);
        
#ifdef DEBUG
        Serial.print(">ground_side_raw:"); Serial.println((int)currSide);
        Serial.println(groundSideToString(currSide));
#endif

        //TODO: Check multistate debouncer works✅
      GroundSide debouncedSide = liquidDelivDebouncer.update(currSide, ::millis());

      switch(currSide) {
        case GroundSide::BOTTOM:
            // I WOULD LOVE TO HAVE AN IR SENSOR RN  -_-
                digitalWrite(SOL_BOT_OUT_PIN, HIGH);
                oneValveOpen = true;
            case GroundSide::LEFT:
                digitalWrite(SOL_BOT_OUT_PIN, HIGH);
                oneValveOpen = true;
            case GroundSide::RIGHT:
                digitalWrite(SOL_RIGHT_OUT_PIN, HIGH);
                oneValveOpen = true;
            case GroundSide::TOP:
                digitalWrite(SOL_RIGHT_OUT_PIN, HIGH);
                oneValveOpen = true;
      }
    }
    else if(oneValveOpen){
      ctx->LiquidDeliveryServo.write(LIQUID_DELIVERY_POS);  
    }
    
    if (this->currentTime > MAX_LIQUID_DELIVERY_TIME) {
        return new Recovery(ctx);
    }
    return nullptr;
}

//TODO: Make sure each solenoid side works poroperly

LiquidDelivery::GroundSide LiquidDelivery::getGroundSide(const float& accelX, const float& accelY) { //FIXME: check axis of accelX and accelY ✅
    if (std::abs(accelX) >= std::abs(accelY)){
        if (accelX < 0) { //gravity is pointed in the negative X axis (since accelerometer measures normal force)
            return GroundSide::RIGHT;
        }
        else {
            return GroundSide::LEFT;
        }
    }
    else {
        if (accelY > 0){ //gravity is pointed in the negative y axis (bottom side where drill points)
            return GroundSide::BOTTOM;
        }
        else {
            return GroundSide::TOP;
        }
    }
    return GroundSide::UNKNOWN;
}