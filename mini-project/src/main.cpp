#include <Arduino.h>
#include "pinmap.h"
#include "motor.h"
#include "chassis.h"
#include "msg.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "clip.h"

motor motor1(motor1_pwm, motor1_CWCCW);
motor motor2(motor2_pwm, motor2_CWCCW);
motor motor3(motor3_pwm, motor3_CWCCW);
motor motor4(motor4_pwm, motor4_CWCCW);

// Adafruit_PWMServoDriver servo_driver(0x40);
motor motor5(motor5_pwm, motor5_CWCCW);
// clip superClip(&motor5, &servo_driver);

//type x
// chassis superCar(&motor1, &motor2, &motor3, &motor4, true);
chassis superCar(&motor4, &motor2, &motor1, &motor3, true);
//type o
//chassis superCar(&motor1, &motor2, &motor3, &motor4, false);

msg nanoMsg;

void liftstop(){
   motor5.setSpeed(0);
}

// clip::clip(motor* lifter, Adafruit_PWMServoDriver* servo_driver){
//     this->servo_driver->begin();
//     this->servo_driver->setOscillatorFrequency(27000000);
//     this->servo_driver->setPWMFreq(50);
// }

void setup() {
  pinMode(upper_switch, INPUT_PULLDOWN);
  pinMode(bottom_switch, INPUT_PULLDOWN);

  //setup the interrupt
  attachInterrupt(upper_switch, liftstop,RISING);
  attachInterrupt(bottom_switch, liftstop, RISING);

  // //for debug / simulate
  nanoMsg.init(&Serial);

  // //for jetson nano
  //nanoMsg.init(&Serial2);
}

void loop() {
  if(!nanoMsg.read()){
    return;
  }
  
  superCar.move(nanoMsg.getX_speed(), nanoMsg.getY_speed(), nanoMsg.getW_speed());

// if(nanoMsg.get_isCloseClip()){
//   superClip.closeClip();
// }
// else{
//   superClip.openClip();
// }
// switch(nanoMsg.getLifting_status()){
//   case 0x00:
//   if(!digitalRead(upper_switch)){
//     superClip.liftUp();
//   }
//   else{
//     superClip.liftstop();
//   }
//   break;
//   case 0x01:
//    if(!digitalRead(bottom_switch)){
//     superClip.liftDown();
//    }
//    else{
//     superClip.liftstop();
//    }
//    break;
//    case 0x02:

//    superClip.liftstop();
//    break;
//    default:
//    break;
// }
 }