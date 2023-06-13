#include <Arduino.h>
#include "motor.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#ifndef CLIP_H
#define CLIP_H

class clip{
    private:
        motor* lifter;
        uint8_t upper_limited_switch;
        uint8_t bottom_limited_switch;

        Adafruit_PWMServoDriver* servo_driver;
        int num_channel;

    public:
        clip(motor* lifter);
        clip(motor* lifter, Adafruit_PWMServoDriver* servo_driver);

        void set_num_channel(int num);

        void openClip();
        void closeClip();

        void liftUp();
        void liftDown();
        void liftStop();
};

#endif