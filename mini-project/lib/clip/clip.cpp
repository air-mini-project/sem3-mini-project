#include "clip.h"


void clip::set_num_channel(int num){
    num_channel = num;
}

void clip::openClip(){
    servo_driver->setPWM(num_channel, 0, 30);
}

void clip::closeClip(){
    servo_driver->setPWM(num_channel, 0, 30);
}

void clip::liftUp(){
    lifter->setDirection(30);
    lifter->setSpeed(40);
}

void clip::liftDown(){
    lifter->setDirection(-30);
    lifter->setSpeed(40);
}

void clip::liftstop(){
    //stop the motor
    lifter->setSpeed(0);
}