#include "clip.h"


// clip::clip(motor* lifter, Adafruit_PWMServoDriver* servo_driver){
//     this -> lifter = lifter;
//     this -> servo_driver = servo_driver;

//     this -> servo_driver -> begin();
//     this -> servo_driver -> setOscillatorFrequency(27000000);
//     this -> servo_driver -> setPWMFreq(50);
// }

clip::clip(motor* lifter){
    this -> lifter = lifter;
}

//debug
void clip::set_num_channel(int num){
    num_channel = num;
}

// void clip::openClip(){
//     servo_driver -> setPWM(num_channel, 0, 120); ///??? = angle
// }

// void clip::closeClip(){
//     servo_driver -> setPWM(num_channel, 0, 10);
// }

void clip::liftUp(){
    lifter->setDirection(true);
    lifter->setSpeed(80);//speed
}

void clip::liftDown(){
    lifter->setDirection(false);
    lifter->setSpeed(80);//speed
}

void clip::liftStop(){
    //stop the motor
    lifter->setSpeed(0);
}