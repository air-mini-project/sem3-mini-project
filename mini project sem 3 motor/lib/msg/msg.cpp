#include "msg.h"

void msg::init(HardwareSerial* serial){ 
    nano = serial;

    nano->begin(115200);
}

bool msg::read(){
    if(!nano->available()){ 
        return false;
    }
    if (nano->read() != start_byte){ 
        return false;
    }

    //decoding 
    x_speed = (int) nano->read();
    y_speed = (int) nano->read();
    w_speed = (int) nano->read();
    isCloseClip = (int) nano->read() != 0x00 ? true : false;
    lifting_status = (int) nano->read();

    return true;
}

int msg::getx_speed(){
    return x_speed;
}

int msg::gety_speed(){
    return y_speed;
}

int msg::getw_speed(){
    return w_speed;
}

bool msg::get_iscloseClip(){ 
    return isCloseClip;
}

int msg::getLifting_status(){ 
    return lifting_status;
}
