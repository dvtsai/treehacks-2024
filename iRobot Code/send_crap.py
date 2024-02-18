import serial

# Set your COM port here
com_port = 'COM22'  # Replace 'COMX' with your actual COM port, e.g., 'COM3'
baud_rate = 9600  # Adjust baud rate as needed to match your Arduino configuration

ser = None  # Initialize ser to None outside try block

try:
    # Initialize serial connection
    ser = serial.Serial(com_port, baud_rate, timeout=1)
    print(f"Connected to {com_port}. You can start sending data.")

    while True:
        # Get input from user
        data_to_send = input("Enter data to send (type 'exit' to quit): ")
        if data_to_send.lower() == 'exit':
            ser.close()  # Close serial connection
            break  # Exit loop

        # Send data to Arduino with newline ending and proper encoding
        ser.write((data_to_send + '\n').encode())
        ser.flushInput()  # Flush input buffer
        print(f"Sent: {data_to_send}")

except serial.SerialException as e:
    print(f"Could not open serial port {com_port}: {e}")

finally:
    if ser is not None and ser.is_open:
        ser.close()  # Ensure serial connection is closed if it was opened
    print("Connection closed.")
