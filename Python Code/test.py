import cv2
import numpy as np


def detect_arrows(frame, params):
    """
    Detect arrows in a given frame using computer vision techniques.

    Parameters:
        frame (numpy.ndarray): The input frame as a NumPy array.
        params (dict): A dictionary containing the parameters for arrow detection.

    Returns:
        str: The direction of the detected arrow, or "no arrow found" if no arrow is detected.
    """
    # Convert the frame to the HSV color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Threshold the hue, saturation, and value channels to detect black pixels
    mask = cv2.inRange(hsv, params['lower_black'], params['upper_black'])

    # Apply morphological operations to remove noise and fill gaps in the mask
    kernel = np.ones((params['morph_kernel_size'], params['morph_kernel_size']), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

    # Detect edges in the masked image using Canny edge detection
    edges = cv2.Canny(mask, params['canny_threshold1'], params['canny_threshold2'], apertureSize=params['canny_aperture_size'])

    # Detect lines using the probabilistic Hough transform
    lines = cv2.HoughLinesP(edges, rho=params['hough_rho'], theta=params['hough_theta'], threshold=params['hough_threshold'], minLineLength=params['hough_min_line_length'], maxLineGap=params['hough_max_line_gap'])

    if lines is not None:
        # Initialize counters for each direction
        directions = {"left": 0, "right": 0, "up": 0, "down": 0}

        # Iterate through the detected lines and classify them as left, right, up, or down arrows
        for line in lines:
            x1, y1, x2, y2 = line[0]
            dx = x2 - x1
            dy = y2 - y1
            angle = np.arctan2(dy, dx) * 180 / np.pi
            length = np.sqrt(dx**2 + dy**2)

            if length < params['min_line_length']:
                continue

            if -45 < angle < 45:
                directions["right"] += 1
            elif 45 <= angle < 135:
                directions["down"] += 1
            elif -135 <= angle < -45:
                directions["up"] += 1
            else:
                directions["left"] += 1

        # Determine the direction of the arrow based on the counts
        direction = max(directions, key=directions.get) if max(directions.values()) >= 1 else "no arrow found"

        # Display the detected lines and the direction of the arrow on the frame
        for line in lines:
            x1, y1, x2, y2 = line[0]
            cv2.line(frame, (x1, y1), (x2, y2), (0, 0, 255), params['line_thickness'])
        cv2.putText(frame, direction, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, params['text_scale'], (0, 0, 255), params['text_thickness'])
    else:
        direction = "no arrow found"

    # Display the frame
    cv2.imshow("Arrow Detection", frame)

    # Return the direction of the arrow
    return direction


def main():
    # Define the parameters for arrow detection
    params = {
        'lower_black': np.array([0, 0, 0]),
        'upper_black': np.array([180, 255, 50]),
        'morph_kernel_size': 5,
        'canny_threshold1': 50,
        'canny_threshold2': 150,
        'canny_aperture_size': 3,
        'hough_rho': 1,
        'hough_theta': np.pi/180,
        'hough_threshold': 50,
        'hough_min_line_length': 100,
        'hough_max_line_gap': 20,
        'min_line_length': 50,
        'line_thickness': 3,
        'text_scale': 1,
        'text_thickness': 2
    }

    # Open the default camera
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Failed to open the camera!")
        return

    while True:
        # Capture a frame
        ret, frame = cap.read()

        if not ret:
            print("Failed to capture a frame!")
            break
        return frame