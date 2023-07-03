import serial
import time

ser = serial.Serial('COM3', baudrate=115200)
time.sleep(0.5) #wait 0.5s for serial connectionp

cw = [0xff,0x00,0x80,0x00,0x00,0x00]

ser.write(serial.to_bytes(cw))
print(serial.to_bytes(cw))