import cv2
import numpy as np
import math
import os
import time

from  pycreate2 import Create2
import time

port  = "COM23"
bot = Create2(port)

bot.start()

bot.safe()

bot.full()

# Start capturing video from the webcam
cap = cv2.VideoCapture(1)

# Get the size of the video frame
_, frame = cap.read()
height, width = frame.shape[:2]

# Calculate the center of the screen/frame
center_screen = (width // 2, height // 2)

# Define a threshold for "close enough" (in pixels)
close_enough_threshold = 50  # Adjust this value as needed
# Define thresholds
close_enough_threshold = 50  # Adjust as needed
turn_right_threshold = 100  # Adjust as needed, should be larger than close_enough_threshold

while True:
    # Read a frame from the webcam
    ret, frame = cap.read()
    if not ret:
        break  # If no frame is captured, break out of the loop

    # Convert the frame to LAB color space and extract the A-channel
    lab = cv2.cvtColor(frame, cv2.COLOR_BGR2LAB)
    a_channel = lab[:, :, 1]

    # Apply threshold and find contours
    _, th = cv2.threshold(a_channel, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    contours, _ = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Filter out small contours
    min_area = 50  # Adjust as needed
    large_contours = [cnt for cnt in contours if cv2.contourArea(cnt) > min_area]

    if large_contours:
        # Combine all large contours into a single one and find its centroid
        all_contours = np.vstack(large_contours)
        M = cv2.moments(all_contours)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])

            # Mask the original frame and draw the centroid
            masked = cv2.bitwise_and(frame, frame, mask=th)
            cv2.circle(masked, (cX, cY), 5, (255, 255, 255), -1)
            cv2.putText(masked, f"({cX}, {cY})", (cX + 10, cY), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

            # Calculate the horizontal distance to the center of the screen
            distance_x = cX - center_screen[0]
            turn_left_threshold = 60 
            turn_right_threshold = 60

            # Turn right if the centroid is to the left and beyond the left threshold
            if distance_x < -turn_left_threshold:
                print("Turning right!")
                bot.drive_direct(30, -30)  # Adjust speeds as needed
                time.sleep(0.8)
                bot.drive_direct(0, 0)  # Adjust speeds as needed
                time.sleep(0.8)

            # Turn left if the centroid is to the right and beyond the right threshold
            elif distance_x > turn_right_threshold:
                print("Turning left!")
                bot.drive_direct(-30, 30)  # Adjust speeds as needed
                time.sleep(0.8)
                bot.drive_direct(0, 0)  # Adjust speeds as needed
                time.sleep(0.8)


            # Move forward if within the close enough threshold
            else:
                print("Moving forward!")
                bot.drive_direct(30, 30)  # Adjust speeds as needed
                time.sleep(0.8)
                bot.drive_direct(0, 0)  # Adjust speeds as needed
                time.sleep(0.8)


            # else:
            #     print("Object not within specific thresholds, adjust behavior as needed.")
                # Define other behaviors here (e.g., stop)
                # bot.drive_direct(0, 0)  # Stop or adjust as needed

            cv2.imshow('Original', frame)
            cv2.imshow('Masked with Centroid', masked)


    # Exit loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        bot.drive_stop()
        bot.close()
        break

# Release the webcam and destroy all OpenCV windows
cap.release()
cv2.destroyAllWindows()


# Release the webcam and destroy all cv2 windows
cap.release()
cv2.destroyAllWindows()
