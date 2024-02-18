import cv2
import numpy as np
import math

# Start capturing video from the webcam
cap = cv2.VideoCapture(1)

# Get the size of the video frame
_, frame = cap.read()
height, width = frame.shape[:2]

# Calculate the center of the screen/frame
center_screen = (width // 2, height // 2)

# Define a threshold for "close enough" (in pixels)
close_enough_threshold = 50  # Adjust this value as needed

while True:
    # Read a frame from the webcam
    ret, frame = cap.read()
    if not ret:
        break  # If no frame is captured, break out of the loop

    # Convert the frame to LAB color space
    lab = cv2.cvtColor(frame, cv2.COLOR_BGR2LAB)

    # Extract the A-channel
    a_channel = lab[:, :, 1]

    # Apply threshold
    _, th = cv2.threshold(a_channel, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

    # Find contours in the thresholded image
    contours, _ = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Filter out small contours
    min_area = 50  # Adjust as needed
    large_contours = [cnt for cnt in contours if cv2.contourArea(cnt) > min_area]

    if large_contours:
        # Combine all large contours into a single contour
        all_contours = np.vstack(large_contours)

        # Calculate the moments for the combined contour to find the centroid
        M = cv2.moments(all_contours)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])

            # Create a mask and apply it to the original frame
            masked = cv2.bitwise_and(frame, frame, mask=th)

            # Draw the centroid on the masked image
            cv2.circle(masked, (cX, cY), 5, (255, 255, 255), -1)
            # Put the coordinates of the centroid on the masked image
            cv2.putText(masked, f"({cX}, {cY})", (cX + 10, cY), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

            # Calculate the distance to the center of the screen
            distance_to_center = math.sqrt((center_screen[0] - cX) ** 2 + (center_screen[1] - cY) ** 2)

            # Check if the centroid is close enough to the center of the screen
            if distance_to_center <= close_enough_threshold:
                # Execute desired actions here
                print("Centroid is close to the center!")

            # Display the original frame and the masked frame with the centroid and coordinates
            cv2.imshow('Original', frame)
            cv2.imshow('Masked with Centroid', masked)

    # Press 'q' to exit the loop
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the webcam and destroy all cv2 windows
cap.release()
cv2.destroyAllWindows()
