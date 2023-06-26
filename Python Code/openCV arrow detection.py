import cv2
import numpy as np

def f(x):
    # any operation
    pass

# Define the signal values for each movement
left_signal = [0xFF, 0x00, 0x00, 0x00, 0x00, 0x00] # Replace with actual values for left movement
right_signal = [0xFF, 0x00, 0x00, 0x00, 0x00, 0x00] # Replace with actual values for right movement
forward_signal = [0xFF, 0x00, 0x64, 0x00, 0x00, 0x00] # Replace with actual values for forward movement

cap = cv2.VideoCapture(0)

cv2.namedWindow("Trackbars")
cv2.createTrackbar("L-H", "Trackbars", 0, 180, f)
cv2.createTrackbar("L-S", "Trackbars", 0, 255, f)
cv2.createTrackbar("L-V", "Trackbars", 0, 255, f)
cv2.createTrackbar("U-H", "Trackbars", 180, 180, f)
cv2.createTrackbar("U-S", "Trackbars", 255, 255, f)
cv2.createTrackbar("U-V", "Trackbars", 30, 255, f)

font = cv2.FONT_HERSHEY_COMPLEX

while True:
    _, frame = cap.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    l_h = cv2.getTrackbarPos("L-H", "Trackbars")
    l_s = cv2.getTrackbarPos("L-S", "Trackbars")
    l_v = cv2.getTrackbarPos("L-V", "Trackbars")
    u_h = cv2.getTrackbarPos("U-H", "Trackbars")
    u_s = cv2.getTrackbarPos("U-S", "Trackbars")
    u_v = cv2.getTrackbarPos("U-V", "Trackbars")

    lower_black = np.array([l_h, l_s, l_v])
    upper_black = np.array([u_h, u_s, u_v])

    mask = cv2.inRange(hsv, lower_black, upper_black)
    kernel = np.ones((4, 4), np.uint8)
    mask = cv2.erode(mask, kernel)

    # Contours detection
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    arrow_detected = False # Flag to check if an arrow is detected
    for cnt in contours:
        area = cv2.contourArea(cnt)
        approx = cv2.approxPolyDP(cnt, 0.02 * cv2.arcLength(cnt, True), True)
        x = approx.ravel()[0]
        y = approx.ravel()[1]

        if area > 10000:

            if len(approx) == 7:
                cv2.putText(frame, "tip", (x, y), font, 1, (0, 0, 0))
                cv2.drawContours(frame, [approx], 0, (0, 0, 0), 5)

            n = approx.ravel()

            i = 0
            if area > 400:
                print(approx)
                if len(approx) == 7:
                    acord = []
                    cord = []
                    sumx = 0
                    sumy = 0
                    for j in n:

                        if (i % 2 == 0):
                            x = n[i]
                            y = n[i + 1]
                            # String containing the co-ordinates.
                            string = str(x) + " " + str(y)

                            if (i == 0):
                                # text on pointed co-ordinate.
                                cv2.putText(frame, string, (x, y), font, 0.5, (255, 255, 0))
                                cord = [(x, y)]
                                # print(cord)
                                x1 = x
                                y1 = y
                            else:
                                # text on remaining co-ordinates.
                                cv2.putText(frame, string, (x, y), font, 0.5, (0, 255, 0))
                                acord = acord + [(x, y)]

                            if len(acord) == 6:
                                # print(acord)
                                sumx = sumx + acord[0][0] + acord[1][0] + acord[2][0] + acord[3][0] + acord[4][0] + acord[5][0]
                                sumy = sumy + acord[0][1] + acord[1][1] + acord[2][1] + acord[3][1] + acord[4][1] + acord[5][1]

                        i = i + 1

                    center_x = sumx // 6
                    center_y = sumy // 6

                    # Calculate the angle between the first and last labeled vertices
                    angle = np.arctan2(acord[5][1] - acord[0][1], acord[5][0] - acord[0][0]) * 180 / np.pi

                    # Check if the angle is within a certain range to determine the direction
                    if 165 < angle < 195:
                        arrow_detected = True # Set flag to indicate that an arrow was detected
                        cv2.putText(frame, "Forward", (center_x, center_y), font, 1, (0, 0, 0))
                        print("Forward")
                        # Send forward movement signal

                    elif angle < 165:
                        arrow_detected = True # Set flag to indicate that an arrow was detected
                        cv2.putText(frame, "Left", (center_x, center_y), font, 1, (0, 0, 0))
                        print("Left")
                        # Send left movement signal

                    elif angle > 195:
                        arrow_detected = True # Set flag to indicate that an arrow was detected
                        cv2.putText(frame, "Right", (center_x, center_y), font, 1, (0, 0, 0))
                        print("Right")
                        # Send right movement signal

    if not arrow_detected:
        cv2.putText(frame, "No arrow detected", (10, 50), font, 1, (0, 0, 0))
        print("No arrow detected")
        # Send forward movement signal

    cv2.imshow("Frame", frame)
    cv2.imshow("Mask", mask)

    key = cv2.waitKey(1)
    if key == 27:
        break

cap.release()
cv2.destroyAllWindows()