#include <Arduino.h>

#ifndef MSG_H
#define MSG_H

    class msg{
        private:
            uint8_t start_byte = 0xff; 
            HardwareSerial* nano;

            int x_speed = 0; 
            int y_speed = 0; 
            int w_speed = 0;

            bool isCloseClip = false; 
            int lifting_status = 0;

        public:
            void init(HardwareSerial* serial);

            bool read();
            int getx_speed(); 
            int gety_speed(); 
            int getw_speed();

            bool get_iscloseClip(); 
            int getLifting_status();
    };

#endif