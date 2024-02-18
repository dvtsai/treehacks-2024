#include "Mx2125.h"

Mx2125::Mx2125(uint8_t x, uint8_t y, uint8_t t){
  xPin = x ;  pinMode(xPin, INPUT) ;
  yPin = y ;  pinMode(yPin, INPUT) ;
  tPin = t ;  pinMode(tPin, INPUT) ;
}

/* Public Methods */

/*
 * Measure acceleration in terms of g (acceleration due to earth's gravity).
 *
 * Returns value that represents either milli G's or 1250ths of a g acting on
 * the axis, depending upon whether or not USE_MILLI_G is defined in Mx2125.h --
 * see README.md for details.
 */
int16_t   Mx2125::mx_acceleration_x() { return mx_acceleration(xPin); }
int16_t   Mx2125::mx_acceleration_y() { return mx_acceleration(yPin); }

/*
 * Measure the level of tilt in terms of +/- 90 degrees. 0 degrees is when
 * the top surface of the accelerometer chip is parallel to the ground.
 */
int8_t    Mx2125::mx_tilt_x() { return mx_tilt(xPin) ; }
int8_t    Mx2125::mx_tilt_y() { return mx_tilt(yPin) ; }

/*
 * Measure clockwise rotation assuming accelerometer is held vertical. Zero
 * degree rotation is when the triangle on the MX2125 chip is pointing up.
 *
 * Returns integer degree value (0 to 359) that represents the clockwise angle
 * of rotation.
 */
uint16_t Mx2125::mx_rotation() {
    return (uint16_t)
      ((atan2(mx_acceleration(yPin), mx_acceleration(xPin)) * 180.0 / PI) + 360) % 360 ;
}

/*
 * Temperature in Celsius as reported by MX2125 chip
 */
float  Mx2125::mx_temperature() {
// This calculation is based upon an Arduino UNO's ADC bit width... adjust if necessary
// for your model.
//     Analog Pin range 0-5v, 0-1023 value, 4.8828125 mV/unit
//     Memsic mx2125 calibrated to 1.25v @ 25C, 5 mV/degree C per [3]

  return 25.0 + ((analogRead(tPin) * 4.8828125) - 1250.0) / 5.0;
}

/* Private methods */

int16_t Mx2125::mx_acceleration(uint8_t axisPin) {
  pulseIn(axisPin, HIGH) ;  // Throw away first result as some people report inconsistent
                            // results if polled too fast on changing pins. And since we
                            // don't know how or when we were last called...
#ifndef USE_MILLI_G
  return pulseIn(axisPin, HIGH) - 5000 ;              // 1250th G per [1]
#else
  return ((pulseIn(axisPin, HIGH) / 10) - 500) * 8 ; // milli G's per [2]
#endif
}

int8_t Mx2125::mx_tilt(uint8_t axisPin) {
  float fa = (float) mx_acceleration(axisPin) ;
#ifndef USE_MILLI_G
  if(fa > 1250.0) fa = 1250.0 ; if(fa < -1250.0) fa = -1250.0 ; // 1250th G per [1]
  return (int8_t) (asin(fa/1250.0) * 180.0 / PI) ;
#else
  if(fa > 1000.0) fa = 1000.0 ; if(fa < -1000.0) fa = -1000.0 ; // milli G's per [2]
  return (int8_t) (asin(fa/1000.0) * 180.0 / PI) ;
#endif
}
