import cv2
import numpy as np

def find_arrow_direction(contour):
    # Calculate the convex hull and the bounding rectangle
    hull = cv2.convexHull(contour)
    rect = cv2.minAreaRect(hull)
    box = cv2.boxPoints(rect)
    box = np.int0(box)

    # Calculate the length of box sides
    side_lengths = [np.linalg.norm(box[i] - box[(i+1)%4]) for i in range(4)]
    sorted_lengths_idx = np.argsort(side_lengths)

    # Find the two longest sides of the bounding box
    pt1, pt2 = (box[sorted_lengths_idx[-1]], box[sorted_lengths_idx[-2]])
    dx, dy = pt1 - pt2
    angle = np.arctan2(dy, dx)

    # Calculate the arrow centroid
    M = cv2.moments(contour)
    cx = int(M['m10'] / M['m00'])
    cy = int(M['m01'] / M['m00'])

    # Check the direction of the arrow
    if np.cos(angle) * (pt1[0] - cx) + np.sin(angle) * (pt1[1] - cy) <= 45:
        return "Left"
    else:
        return "Right"

def main():
    cap = cv2.VideoCapture(0)

    while True:
        ret, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        _, thresh = cv2.threshold(gray, 50, 255, cv2.THRESH_BINARY_INV)

        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            area = cv2.contourArea(cnt)
            if area > 1000:  # Filter out small contours
                hull = cv2.convexHull(cnt)
                hull_area = cv2.contourArea(hull)
                solidity = float(area) / hull_area

                if 0.5 < solidity < 0.9:  # Filter contours based on solidity
                    direction = find_arrow_direction(cnt)
                    cv2.drawContours(frame, [cnt], 0, (0, 255, 0), 2)
                    M = cv2.moments(cnt)
                    cx = int(M['m10'] / M['m00'])
                    cy = int(M['m01'] / M['m00'])
                    cv2.putText(frame, direction, (cx, cy), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

        cv2.imshow('Arrow Detection', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()