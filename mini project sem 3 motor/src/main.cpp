#include <Arduino.h>
#include "pinmap.h"
#include "motor.h"
#include "chassis.h"
#include "msg.h"
#include "Wire.h" 
// #include "pca9685.h"
// #include "clip.h"

motor motor1(motor1_pwm, motor1_CWCCW); 
motor motor2(motor2_pwm, motor2_CWCCW); 
motor motor3(motor3_pwm, motor3_CWCCW); 
motor motor4(motor4_pwm, motor4_CWCCW);

//PCA9685 servo_driver(12, &wire);
motor motor5(motor5_pwm, motor5_CWCCW);
//clip clipper(&motor5, upper_switch, bottom_switch, &servo_driver);

chassis superCar(&motor1, &motor2, &motor3, &motor4);

msg nanoMsg;

void setup() {
  nanoMsg.init(&Serial2);
}

void loop() {
  if (nanoMsg.read()){
    return; 
  }

  superCar.move(nanoMsg.getx_speed(), nanoMsg.gety_speed(), nanoMsg.getw_speed());
}