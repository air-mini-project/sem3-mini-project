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

int num_channel = 3;  //according to your setting to adjust it
int close_pulse_value = 160;  //according to your setting to adjust it
int open_pulse_value = 90;   //according to your setting to adjust it
// Min:180, Max:90 (SMALL VAL = LARGER OPEN PULSE)
// Adafruit_PWMServoDriver servo_driver(0x40);

void liftStop(){
  motor5.setSpeed(0);
}

void openClip(){
    servo_driver.setPWM(num_channel, 0, open_pulse_value); //done
  }
void closeClip(){
    servo_driver.setPWM(num_channel, 0, close_pulse_value); //done

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

  //servo_driver.setPWM(num_channel, 0, angle);//160

}

void loop() {
  if(!nanoMsg.read()){
    return;
  }

  //chassis move
  superCar.move(nanoMsg.getx_speed(), nanoMsg.gety_speed(), nanoMsg.getw_speed());

  //clip
  // if(nanoMsg.get_iscloseClip()){
  //   closeClip();
  // }
  // else{
  //   openClip();
  // }

  // if(nanoMsg.get_iscloseClip()){
  //   servo_driver.setPWM(num_channel, 0, open_pulse_value); 
  // }
  // else{
  //   servo_driver.setPWM(num_channel, 0, close_pulse_value);    
  // }

// for (int pos = 0; pos <= 180; pos += 1) // change here in the place 0 and 180 , into any two degrees you wish the servo to sweep.
//     { 
    
//      servo_driver.setPWM(num_channel, 0,pos);              
//      delay(15);                      
//     }
    
//     for (int pos = 180; pos >= 0; pos -= 1)  // change here also  in the place 0 and 180 , into any two degrees you wish the servo to sweep.
//     { 
//      servo_driver.setPWM(num_channel, 0,pos);              
//      delay(15);                       
//     }


  //lifter
  switch(nanoMsg.getLifting_status()){
    case 0x02:
      //up
      if(!digitalRead(upper_switch)){
        clipper.liftUp();
        delay(500);
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
        delay(500);
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

    case 0x03:
      closeClip();
      break;

    case 0x04:
      openClip();
      break;

    default:
      //for safety, it will stop if the limit switches are triggered.
      // if(digitalRead(upper_switch) | digitalRead(bottom_switch)){
      //   clipper.liftStop();
      // }
    break;
  }

}
