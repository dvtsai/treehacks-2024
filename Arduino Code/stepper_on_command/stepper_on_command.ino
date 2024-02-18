// defines pins numbers
const int stepPin = 3;
const int dirPin = 2;
const int dirPin2 = 8;
const int enablePin = 9; // Using pin 9 as the enable pin

// Variables to store received commands
String command = "";

// Sets the pins as Outputs and initializes them
void setup() {
  // Sets the pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(enablePin, OUTPUT); // Set the enable pin as an output

  // Initialize pins to known states
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(dirPin2, LOW);
  digitalWrite(enablePin, LOW); // Enable the stepper driver

  // Start serial communication
  Serial.begin(9600);
}

// Function to move the steppers
void moveSteppers() {
  // Define the number of steps and delay
  int del = 10000;
  int step_num = 15;

  // Move in one direction
  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
  digitalWrite(dirPin2, LOW);

  // Makes pulses for one full cycle rotation
  for (int x = 0; x < step_num; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
  }
  delay(1000); // One second delay

  // Change direction
  digitalWrite(dirPin, LOW);
  digitalWrite(dirPin2, HIGH);

  // Makes pulses for one full cycle rotation
  for (int x = 0; x < step_num; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
  }
}

// Main program loop
void loop() {
  // Check if serial data is available
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();
    // Append the incoming character to the command string
    command += incomingByte;

    // Check if the command string contains "MOVE"
    if (command.indexOf("MOVE") >= 0) {
      // Move the steppers
      moveSteppers();
      // Clear the command string
      command = "";
    }
  }
}
