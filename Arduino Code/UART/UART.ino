#include <SoftwareSerial.h>

String a;
// Arduino uno Ext Serial pins
int ext_rx_pin = 8;
int ext_tx_pin = 9;
SoftwareSerial ext(ext_rx_pin, ext_tx_pin);  //RX, TX

void setup() {
  // opens serial port
  Serial.begin(4800);
  Serial.println("test");

  // Setup external serial connection to jetson

  ext.begin(4800 );
  ext.print("Hi!");

  Serial.flush();
  ext.flush();

}

void loop() {
  ext.flush();
  while (ext.available() > 0) {
    a = ext.readStringUntil('\n');  // read the incoming data as string
    // Print message on ide console
    Serial.println(a);
    // Answer to jetson
    ext.print("MESSAGE_OK");
    ext.print("\n");
  }
  

  //ext.print("Hi!");
  delay(1000);
}