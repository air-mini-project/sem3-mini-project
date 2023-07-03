#include "chassis.h"

chassis::chassis(motor* fl,motor*bl , motor* fr, motor* br, bool isTypeX){
    this->fl = fl;
    this->fr = fr;
    this->bl = bl;
    this->br = br;
    this->isTypeX = isTypeX;
}

void chassis::setType(bool isTypeX){
    this->isTypeX = isTypeX;
}

void chassis::move(int x, int y, int w){
    int8_t frontLeftMotor;
    int8_t backLeftMotor;
    int8_t frontRightMotor;
    int8_t backRightMotor;  

    if(isTypeX){
        //type x algorithm
        frontLeftMotor = y + x + w;
        backLeftMotor = y - x + w;
        frontRightMotor = y - x - w;
        backRightMotor = y + x - w;
    }
    else{
        //type o algorithm
        //do this by Nick
    }

    //set direction
    if(frontLeftMotor > 0){
        fl->setDirection(false);
    }
    else{
        fl->setDirection(true);
    }
    //set the speed
    fl->setSpeed(abs(frontLeftMotor));

    //set direction
    if(frontRightMotor > 0){
        fr->setDirection(true);
    }
    else{
        fr->setDirection(false);
    }
    //set the speed
    fr->setSpeed(abs(frontRightMotor));

    //set direction
    if (backLeftMotor > 0){
         bl->setDirection(false);
    }
    else{
        bl->setDirection(true);
    }
    //set the speed
    bl->setSpeed(abs(backLeftMotor));

    //set direction
    if (backRightMotor > 0) {
        br->setDirection(true);
    }
    else{
        br->setDirection(false);
    }
    //set the speed
    br->setSpeed(abs(backRightMotor));
}