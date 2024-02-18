#include <AltSoftSerial.h>

String a;
AltSoftSerial ext;  // AltSoftSerial uses fixed pins, no need to define them here

void setup() {
  // opens serial port
  Serial.begin(4800);
  Serial.println("test");

  // Setup external serial connection
  ext.begin(4800);  // Begin communication on the fixed pins for AltSoftSerial
}

void loop() {
  while (ext.available() > 0) {
    a = ext.readStringUntil('\n');  // read the incoming data as string
    // Print message on IDE console
    Serial.println(a);
    // Answer back
    ext.print("MESSAGE_OK");
    ext.print("\n");
  }

  delay(1000);  // Delay to reduce workload on the Arduino
}
