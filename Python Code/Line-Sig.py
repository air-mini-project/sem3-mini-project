import numpy as np
import cv2
import serial
import time

ser = serial.Serial('COM3', baudrate=115200)
time.sleep(0.5) #wait 0.5s for serial connection

video_capture = cv2.VideoCapture(1, cv2.CAP_DSHOW)
video_capture.set(3, 180)
video_capture.set(4, 140)

cw = [0xFF, 0x00, 0x00, 0x32, 0x00, 0x00]

while (True):
    # Capture the frames
    ret, frame = video_capture.read()

    # Crop the image
    crop_img = frame[20:140, 40:180]

    # Convert to HSV color space
    hsv = cv2.cvtColor(crop_img, cv2.COLOR_BGR2HSV)

    # Define range of yellow color in HSV
    lower_yellow = np.array([20, 100, 100])
    upper_yellow = np.array([30, 255, 255])

    # Threshold the HSV image to get only yellow colors
    mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

    # Find the contours of the frame
    contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Find the biggest contour (if detected)
    cx = -1
    cy = -1
    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)

        if M['m00'] > 0:
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            cv2.line(crop_img, (cx, 0), (cx, 720), (255, 0, 0), 1)
            cv2.line(crop_img, (0, cy), (1280, cy), (255, 0, 0), 1)

        cv2.drawContours(crop_img, contours, -1, (0, 255, 0), 1)

        #CE=-50 / 9C=-100 // 32=50 / 64 = 100
        if cx > 0:
            print(cx)
            if cx >= 95: #120
                #print("Turn Right!")
                cw = [0xFF, 0x00, 0x00, 0x45, 0x00, 0x00]

            if 65 < cx < 95: #50
                #print("On Track!")
                cw = [0xFF, 0x00, 0x32, 0x00, 0x00, 0x00]

            if cx <= 65:
                #print("Turn Left!")
                cw = [0xFF, 0x00, 0x00, 0xBC, 0x00, 0x00]

        else:
            print("I don't see the line")
            cw = [0xFF, 0x00, 0x00, 0x32, 0x00, 0x00] # 0x64


    ser.write(serial.to_bytes(cw))
    #print(serial.to_bytes(cw))

    # Display the resulting frame
    cv2.imshow('frame', crop_img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


# Release the camera and close all windows
video_capture.release()
cv2.destroyAllWindows()
