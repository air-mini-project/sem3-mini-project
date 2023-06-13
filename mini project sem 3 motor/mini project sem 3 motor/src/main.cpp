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

Adafruit_PWMServoDriver servo_driver(0x40);
motor motor5(motor5_pwm, motor5_CWCCW);
//clip clipper(&motor5, &servo_driver);
clip clipper(&motor5);

chassis superCar(&motor4, &motor2, &motor1, &motor3, true); //motor* fl, motor* fr, motor* bl, motor* br, bool isTypeX

msg nanoMsg;

int num_channel = 1;

void liftStop(){
  motor5.setSpeed(0);
}

void setup() {
  
  pinMode(upper_switch, INPUT_PULLDOWN);
  pinMode(bottom_switch, INPUT_PULLDOWN);

  //setup the interrupt
  attachInterrupt(upper_switch, liftStop, RISING);
  attachInterrupt(bottom_switch, liftStop, RISING);

  servo_driver.begin();
  servo_driver.setOscillatorFrequency(27000000);
  servo_driver.setPWMFreq(50);

  //Debug / Simulate
  nanoMsg.init(&Serial);

  //JetsonNano
  //nanoMsg.init(&Serial2);

  motor4.setReversed(true);
  motor1.setReversed(true);

  clipper.liftStop();
}

void loop() {
  if (!nanoMsg.read()){
    return; 
  }

  //chassis move
  superCar.move(nanoMsg.getx_speed(), nanoMsg.gety_speed(), nanoMsg.getw_speed());

  //clip
  // if(nanoMsg.get_iscloseClip()){
  //   clipper.closeClip();
  // }
  // else{
  //   clipper.openClip();
  // }

  if(nanoMsg.get_iscloseClip()){
    servo_driver.setPWM(num_channel, 0, 10);
  }
  else{
    servo_driver.setPWM(num_channel, 0, 10);
  }

  //lifter
  switch(nanoMsg.getLifting_status()){
    case 0x02:
      //up
      if(!digitalRead(upper_switch)){
        clipper.liftUp();
      }
      else{
        clipper.liftStop();
      }
      break;

    case 0x01:
      //down
      if(!digitalRead(bottom_switch)){
        clipper.liftDown();
      }
      else{
        clipper.liftStop();
      }
      break;

    case 0x00:
      //stop
      clipper.liftStop();
      break;

    default:
      //for safety, it will stop if the limit switches are triggered.
      if(digitalRead(upper_switch) | digitalRead(bottom_switch)){
        clipper.liftStop();
      }
    break;
  }

}