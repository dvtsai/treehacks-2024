#include <Mx2125.h>

Mx2125  Accelerometer(2, 3, A0) ;
uint8_t HB=0 ;  // Heartbeat counter

void setup() {
  DDRB |= B00100000 ; // Blink pin 13 for heartbeat
  Serial.begin(115200) ;
}

void loop() {
  float F, C ;

  Serial.print("Acceleration: ") ;
  Serial.print(Accelerometer.mx_acceleration_x()) ;
  Serial.print(",") ;
  Serial.print(Accelerometer.mx_acceleration_y()) ;
  Serial.print(" -- Tilt: ") ;
  Serial.print(Accelerometer.mx_tilt_x()) ;
  Serial.print(",") ;
  Serial.print(Accelerometer.mx_tilt_y()) ;
  Serial.print("  -- Rotation: ") ;
  Serial.print(Accelerometer.mx_rotation()) ;
  Serial.print("  -- Temp: ") ;
  C = Accelerometer.mx_temperature() ;
  F = C * 9/5 + 32 ;
  Serial.print(C) ; Serial.print("C, ") ;
  Serial.print(F) ; Serial.println("F") ;
  delay(200) ; if (HB++ % 5 == 0) PORTB ^= B00100000 ; // Blink pin 13 for heartbeat
}
