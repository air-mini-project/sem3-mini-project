#include <Arduino.h>

#ifndef MOTOR_H
#define MOTOR_H

class motor{
    private:
        uint8_t pwm_pin;
        uint8_t direction_pin;

        int pwm = 0;
        bool isCW;

        bool isReversed = false;

        int limited(int value, int maximum_value, int minimum_value);

    public:
        motor(uint8_t pwm_pin, uint8_t direction_pin);

        void setReversed(bool isReversed);

        void setSpeed(int pwm);
        int getSpeed();

        void setDirection(bool isCW);
        bool getDirection();            
};

#endif