import cv2
import numpy as np
import math
import serial

# Serial port setup
com_port = 'COM22'  # Adjust with your actual COM port
baud_rate = 9600  # Adjust baud rate to match your device's configuration

# Initialize cap to None
cap = None

# Counter for frames where the centroid is outside the "close enough" region
outside_center_counter = 0
# Threshold for consecutive frames outside the "close enough" region
outside_center_threshold = 10  # Adjust as needed

try:
    # Initialize serial connection
    ser = serial.Serial(com_port, baud_rate, timeout=1)
    print(f"Connected to {com_port}. Ready to send data.")

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

                font = cv2.FONT_HERSHEY_SIMPLEX
                text = f"({cX}, {cY})"
                cv2.putText(masked, text, (cX + 10, cY), font, 0.5, (255, 255, 255), 1, cv2.LINE_AA)

                # Calculate the distance to the center of the screen
                distance_to_center = math.sqrt((center_screen[0] - cX) ** 2 + (center_screen[1] - cY) ** 2)

                # Check if the centroid is close enough to the center of the screen
                if distance_to_center <= close_enough_threshold:
                    if outside_center_counter >= outside_center_threshold:
                        # Send data through the serial port
                        data_to_send = "MOVE"  # Define your data/command
                        ser.write((data_to_send + '\n').encode())
                        print(f"Sent: {data_to_send}")
                        outside_center_counter = 0  # Reset the counter
                else:
                    outside_center_counter += 1  # Increment the counter if the centroid is outside the "close enough" region

                # Display the original frame and the masked frame with the centroid
                cv2.imshow('Original', frame)
                cv2.imshow('Masked with Centroid', masked)

        # Press 'q' to exit the loop
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except serial.SerialException as e:
    print(f"Could not open serial port {com_port}: {e}")

finally:
    # Check if 'cap' was defined and release it if so
    if cap is not None:
        cap.release()
    # Destroy all cv2 windows
    cv2.destroyAllWindows()
    # Check if serial connection 'ser' was defined and is open, then close it
    if 'ser' in locals() and ser.is_open:
        ser.close()
    print("Resources released. Program terminated.")
