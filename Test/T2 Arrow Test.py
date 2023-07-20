import numpy as np
import cv2
import serial
import time

ser = serial.Serial('COM3', baudrate=115200)
time.sleep(0.5)  # wait 0.5s for serial connection

video_capture = cv2.VideoCapture(1, cv2.CAP_DSHOW)
video_capture.set(3, 180)
video_capture.set(4, 140)

cw = [0xFF, 0x00, 0x00, 0x00, 0x00, 0x00]

countdown = 0
counter = 0
blue_count = 0
countdown_a = 20

turn_val = 4

# Function to detect blue color in the frame
def detect_blue(frame):
    global countdown
    global blue_count
    global turn_val
    # Convert to HSV color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Define range of blue color in HSV
    lower_blue = np.array([100, 50, 50])
    upper_blue = np.array([130, 150, 150])

    # Threshold the HSV image to get only blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    # Find the contours of the frame
    contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Check if blue color is detected
    if blue_count <= turn_val:
        if countdown <= 0:
            if len(contours) > 0:
                print("I can see blue")  # To be tested
                cw = [0xFF, 0x00, 0x00, 0x00, 0x00, 0x00]
                ser.write(serial.to_bytes(cw))
                time.sleep(5)
                countdown = 320
                blue_count +=1
                print(blue_count)

    elif blue_count >= turn_val:
        if countdown <= 0:
            if len(contours) > 0:
                print("I can see blue")  # To be tested
                cw = [0xFF, 0x00, 0x00, 0x00, 0x00, 0x00]
                ser.write(serial.to_bytes(cw))
                time.sleep(5)
                countdown = 50


    # Apply cooldown after detection
    if countdown > 0:
        countdown -= 1
        # print("Cooldown:", countdown)
        return frame, countdown

    return frame


def detect_arrow(frame):
    global counter

    # global countdown_a
    # if countdown_a >= 0:
    #     countdown_a -=1
    # else:
        # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Apply a threshold to create a binary image
    ret, thresh = cv2.threshold(gray, 127, 255, 0)

    # Find contours in the binary image
    contours, _ = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # Iterate over the contours
    for contour in contours:
        # Compute the bounding box of the contour
        (x, y, w, h) = cv2.boundingRect(contour)

        # Filter the contour based on its size
        if w > 50 and h > 50:
            # Crop the binary image to the bounding box
            cropped = thresh[y:y + h, x:x + w]

            # Split the cropped image into left and right halves
            left = cropped[:, :w // 2]
            right = cropped[:, w // 2:]

            # Count the number of black pixels in each half
            left_count = np.count_nonzero(left == 0)
            right_count = np.count_nonzero(right == 0)

            # Check if there are more black pixels on one side than the other
            if left_count > right_count:
                print("right arrow")
                cw = [0xFF, 0x00, 0x00, 0x50, 0x00, 0x00]
                time.sleep(3)

            elif right_count > left_count:
                print("left arrow")
                cw = [0xFF, 0x00, 0x00, 0xAC, 0x00, 0x00]
                time.sleep(3)
            counter +=1


while (True):
    # Capture the frames
    ret, frame = video_capture.read()

    if blue_count == turn_val:
        if counter < 1:
            detect_arrow(frame)
    #cv2.imshow('frame2', frame)

    # Crop the image
    crop_img = frame[20:140, 30:170]

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
            cx = int(M['m10'] / M['m00'])
            cy = int(M['m01'] / M['m00'])
            cv2.line(crop_img, (cx, 0), (cx, 720), (255, 0, 0), 1)
            cv2.line(crop_img, (0, cy), (1280, cy), (255, 0, 0), 1)

        cv2.drawContours(crop_img, contours, -1, (0, 255, 0), 1)

        # CE=-50 / 9C=-100 // 32=50 / 64 = 100

        if cx > 0:
            # print(cx)
            if cx >= 95:  # 120
                # print("Turn Right!")
                cw = [0xFF, 0x00, 0x00, 0x45, 0x00, 0x00]

            if 65 < cx < 95:  # 50
                # print("On Track!")
                cw = [0xFF, 0x00, 0x32, 0x00, 0x00, 0x00]

            if cx <= 65:
                # print("Turn Left!")
                cw = [0xFF, 0x00, 0x00, 0xBC, 0x00, 0x00]

        else:
            #print("I don't see the line")
            cw = [0xFF, 0x00, 0x00, 0x45, 0x00, 0x00]  # 0x64

    ser.write(serial.to_bytes(cw))
    # print(serial.to_bytes(cw))

    # Detect blue color in the frame
    detect_blue(crop_img)

    # Display the resulting frame
    cv2.imshow('frame', crop_img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close all windows
video_capture.release()
cv2.destroyAllWindows()
