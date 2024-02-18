#include <AltSoftSerial.h>

// Stepper motor control pins
const int stepPin = 3;
const int dirPin = 2;

// Initialize AltSoftSerial
AltSoftSerial extSerial;

String incomingCommand;  // To store the incoming serial command

void setup() {
  // Initialize built-in serial port
  Serial.begin(4800);
  Serial.println("Ready");

  // Initialize external serial communication
  extSerial.begin(4800);
  extSerial.print("Init");
  extSerial.print("\n");

  // Setup stepper motor pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  // Check if data is available to read from the built-in serial
  if (Serial.available() > 0) {
    // Read the incoming data as a string
    incomingCommand = Serial.readStringUntil('\n');

    // Forward the incoming command to the external serial
    extSerial.println(incomingCommand);  // Use println to preserve line endings

    // Optional: Echo back to built-in serial for confirmation
    Serial.println("Forwarded: " + incomingCommand);
  }

  // Check if data is available to read from the external serial
  if (extSerial.available() > 0) {
    // Read the incoming data as a string
    incomingCommand = extSerial.readStringUntil('\n');

    // Echo every single thing received from the external serial to the built-in serial
    Serial.println(incomingCommand);

    // Process the incoming command
    if (incomingCommand == "SPIN") {
      spinStepper();  // Function to spin the stepper motor
      delayMicroseconds(500);
      extSerial.println("Stepper Spinning");

    } else if (incomingCommand == "STOP") {
      delayMicroseconds(500);
      extSerial.println("Stepper Stopped");

      // If STOP is received, no action is taken on the stepper motor
    } else {
      extSerial.println("Unknown Command");
    }

    // Clear the command after execution
    incomingCommand = "";
  }
}

void spinStepper() {
  digitalWrite(dirPin, HIGH);  // Set direction

  // Spin the stepper motor with 200 steps
  for (int x = 0; x < 200; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    // digitalWrite(stepPin, LOW);
    // delayMicroseconds(500);
  }
}
