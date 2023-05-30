#include "motor.h"

motor::motor(uint8_t pwm_pin, uint8_t direction_pin){ 
    this->pwm_pin = pwm_pin;
    this->direction_pin = direction_pin;

    pinMode(pwm_pin, OUTPUT); 
    pinMode(direction_pin, OUTPUT);
    
    setSpeed(0);
    setDirection(true);
}

void motor::setSpeed(int pwm){
    this->pwm = pwm;

    int value = map(pwm, 0, 100, 0, 255);
    analogWrite(this->pwm_pin,value);
}

int motor::getSpeed(){
return pwm;
}

void motor::setDirection(bool isCW){ 
    this->isCW = isCW;

    if(isCW){
        digitalWrite(this->direction_pin, HIGH);
    }
    else{
        digitalWrite(this->direction_pin, LOW);
    }

}

bool motor::getDirection(){
return isCW;
}