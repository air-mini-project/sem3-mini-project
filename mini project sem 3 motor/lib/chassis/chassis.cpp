#include "chassis.h"

chassis::chassis(motor* fl, motor* fr, motor* bl, motor* br){
    this->fl = fl;
    this->fr = fr;
    this->bl = bl;
    this->br = br;
}

void chassis::move(int x, int y, int w){
    int frontLeftMotor = y + x + w;
    int backLeftMotor = y - x + w;
    int frontRightMotor = y - x - w;
    int backRightMotor = y + x - w;

    //set direction
    if(frontLeftMotor > 0){
        fl->setDirection(true);
    }
    else{
        fl->setDirection(false);
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
        bl->setDirection(true);
    }
    else{
        bl->setDirection(false);
    }
    //set the speed
    bl->setSpeed(abs(backLeftMotor));

    //set direction
    if (backRightMotor > 0){
        br->setDirection(true);
    }
    else{
        br->setDirection(false);
    }
    //set the speed
    br->setSpeed(abs(backRightMotor));
}