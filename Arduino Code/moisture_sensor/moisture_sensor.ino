// Define the analog pin used for the sensor
const int moisturePin = A0;

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read the raw sensor value
  int sensorValue = analogRead(moisturePin);

  // Convert the sensor value to a percentage
  // Assuming the sensor has a range from 0 to 1023
  int moisturePercentage = map(sensorValue, 0, 1023, 0, 100);

  // Print the moisture percentage to the serial monitor
  Serial.print("Moisture Level: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  // Add a delay before taking the next reading
  delay(1000); // Adjust delay as needed
}
