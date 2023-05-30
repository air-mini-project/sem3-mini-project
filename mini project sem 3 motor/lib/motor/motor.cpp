#include "motor.h"

motor::motor(uint8_t pwm_pin, uint8_t direction_pin){ 
    this->pwm_pin = pwm_pin;
    this->direction_pin = direction_pin;

    pinMode(pwm_pin, OUTPUT); 
    pinMode(direction_pin, OUTPUT);
    
    setSpeed(0);
    setDirection(true);
}

void motor::setReversed(bool isReversed){
    this -> isReversed = isReversed;
}

int motor::limited(int value, int minimum_value, int maximum_value){
    if (value >= maximum_value){
        value = maximum_value;
    }
    else if (value <= minimum_value){
        value = minimum_value;
    }
    return value;
}

void motor::setSpeed(int pwm){
    this->pwm = pwm;

    this->pwm = limited(this->pwm, 0, 100); //pwm, max value, min value

    int value = map(this->pwm, 0, 100, 0, 255);
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