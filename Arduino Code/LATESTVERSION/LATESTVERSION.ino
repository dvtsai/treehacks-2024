#include <Servo.h>  // Include the Servo library
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6

#define LED_COUNT 50

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


const int moisturePin = A0;


// Define pin numbers
const int stepPin = 3;
const int dirPin = 2;
const int dirPin2 = 8;
const int enablePin = 9;
const int servoPin = 10;  // Define the pin connected to the servo signal line
const int servoPin2 = 11;  // Define the pin connected to the servo signal line
const int servoPin3 = 5;  // Define the pin connected to the servo signal line


// Variables to store received commands
String command = "";

Servo myThirdServo;  // New servo
Servo mySecondServo;  // New servo
Servo myServo;  // Create a servo object to control a servo

void setup() {
   pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);




  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(20);  // Set BRIGHTNESS to about 1/5 (max = 255)
  // rainbow(10);
  // colorWipe(strip.Color(0, 0, 255), 10);  // Blue




  // Sets the pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Initialize pins to known states
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(dirPin2, LOW);
  digitalWrite(enablePin, LOW);  // Enable the stepper driver


  // myServo.attach(servoPin);  // Attaches the servo on servoPin to the servo object

  // Start serial communication
  Serial.begin(9600);
}

void moveSteppers() {
  // Define the number of steps and delay
  int del = 10000;
  int step_num = 15;

  // Move in one direction
  digitalWrite(dirPin, HIGH);
  digitalWrite(dirPin2, LOW);

  for (int x = 0; x < step_num; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
  }
  delay(1000);  // One second delay

  // Change direction
  digitalWrite(dirPin, LOW);
  digitalWrite(dirPin2, HIGH);

  for (int x = 0; x < step_num; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(del);
  }
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}

int readMoisturePercentage(int moisturePin) {
  // Read the raw sensor value
  int sensorValue = analogRead(moisturePin);

  // Convert the sensor value to a percentage
  // Assuming the sensor has a range from 0 to 1023
  int moisturePercentage = map(sensorValue, 0, 1023, 0, 100);

  return moisturePercentage;
}

// Function to control the servo with direction
void spinServo(bool clockwise) {
  myServo.attach(servoPin);  // Re-attach the servo to control it

  if (clockwise) {
    myServo.writeMicroseconds(1500 + 500);  // Clockwise at low speed
  } else {
    myServo.writeMicroseconds(1500 - 500);  // Counterclockwise at low speed
  }
  delay(1000);                      // Spin for 3 seconds
  myServo.writeMicroseconds(1500);  // Stop the servo

  myServo.detach();  // Detach the servo to save power
}

// Function to control the servo with direction
void spinServo2(bool clockwise) {
  myServo.attach(servoPin2);  // Re-attach the servo to control it

  if (clockwise) {
    myServo.writeMicroseconds(1500 + 500);  // Clockwise at low speed
  } else {
    myServo.writeMicroseconds(1500 - 500);  // Counterclockwise at low speed
  }
  delay(500);                      // Spin for 3 seconds
  myServo.writeMicroseconds(1500);  // Stop the servo

  myServo.detach();  // Detach the servo to save power
}

// Function to control the servo with direction
void spinServo3(bool clockwise) {
  myServo.attach(servoPin3);  // Re-attach the servo to control it

  if (clockwise) {
    myServo.writeMicroseconds(1500 + 500);  // Clockwise at low speed
  } else {
    myServo.writeMicroseconds(1500 - 500);  // Counterclockwise at low speed
  }
  delay(1500);                      // Spin for 3 seconds
  myServo.writeMicroseconds(1500);  // Stop the servo

  myServo.detach();  // Detach the servo to save power
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show();  // Update strip with new contents
    delay(wait);   // Pause for a moment
  }
}

void loop() {
  static String command = ""; // Use a static String to accumulate command characters
  static unsigned long lastMoistureCheck = 0; // Timestamp of the last moisture check
  const unsigned long moistureCheckInterval = 1000; // Check moisture every 10 seconds (10000 milliseconds)

  // Process serial commands
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();

    // Check for a command terminator, such as a newline or carriage return
    if (incomingByte == '\n' || incomingByte == '\r') {
      // Check the accumulated command string and execute the corresponding function
      if (command == "MOVE") {
        moveSteppers();
      } else if (command == "SERVO1_CW") {
        spinServo(true); // Spin servo 1 clockwise
      } else if (command == "SERVO1_CCW") {
        spinServo(false); // Spin servo 1 counterclockwise
      } else if (command == "SERVO2_CW") {
        spinServo2(true); // Spin servo 2 clockwise
      } else if (command == "SERVO2_CCW") {
        spinServo2(false); // Spin servo 2 counterclockwise
      } else if (command == "SERVO3_CW") {
        spinServo3(true); // Spin servo 2 clockwise
      } else if (command == "SERVO3_CCW") {
        spinServo3(false); // Spin servo 2 counterclockwise
      }

      // Clear the command string after processing
      command = "";
    } else {
      // Append the incoming byte to the command string if it's not a terminator
      command += incomingByte;
    }
  }

  // Check moisture at intervals instead of every loop iteration
  unsigned long currentMillis = millis();
  if (currentMillis - lastMoistureCheck >= moistureCheckInterval) {
    lastMoistureCheck = currentMillis; // Update the last check timestamp
    int moisturePercentage = readMoisturePercentage(moisturePin);
    // Serial.print(moisturePercentage);

    // Update the LED strip based on the moisture level
    if (moisturePercentage > 15) {
      colorWipe(strip.Color(0, 0, 255), 10);  // Blue for high moisture
    } else {
      colorWipe(strip.Color(255, 0, 0), 10);  // Red for low moisture
    }
  }
}
