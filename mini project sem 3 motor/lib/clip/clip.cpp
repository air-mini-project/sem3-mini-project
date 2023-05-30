#include "clip.h"

clip::clip(motor* lifter, Adafruit_PWMServoDriver* servo_driver){
    this -> lifter = lifter;
    this -> servo_driver = servo_driver;

    this -> servo_driver -> begin();
    this -> servo_driver -> setOscillatorFrequency(27000000);
    this -> servo_driver -> setPWMFreq(50);
}

void clip::set_num_channel(int num){
    num_channel = num;
}

void clip::openClip(){
    servo_driver -> setPWM(num_channel, 0, 130); ///??? = angle
}

void clip::closeClip(){
    servo_driver -> setPWM(num_channel, 0, 0);
}

void clip::liftUp(){
    lifter->setDirection(true);
    lifter->setSpeed(100);//speed
}

void clip::liftDown(){
    lifter->setDirection(false);
    lifter->setSpeed(100);//speed
}

void clip::liftStop(){
    lifter->setSpeed(0);
}