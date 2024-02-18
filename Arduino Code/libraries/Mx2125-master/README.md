Memsic MX2125 Dual-axis Accelerometer library for Arduino
=========================================================
Copyright 2015 Aaron Magill, MIT License.

  Sourced primarily from the following:

  [1] http://learn.parallax.com/propeller-c-simple-devices/tilt-and-acceleration-mx2125
      specifically, the contents of mx2125.h and mx2125.c in specified support library.

  [2] http://www.arduino.cc/en/Tutorial/Memsic2125?from=Tutorial.AccelerometerMemsic2125

  [3] https://www.jameco.com/Jameco/Products/ProdDS/282870.pdf

Note that this is all working on an Arduino UNO... there may be changes necessary for other boards, but I don't know them as I don't have them to test with... As I think I recall reading that some models have a different ADC bit width on their analog pins, especially check `Mx2125::mx_temperature()` in `Mx2125.cpp` for your model.

### Methods

~~~
Mx2125::Mx2125(xPin, yPin, tPin)
~~~
Constructor which sets the pins being used for the X-Axis reading, the Y-Axis reading, and the T-Out reading for input.  xPin and yPin should be digital pins, while tPin should be an analog pin.  Undefined results if you choose poorly.

~~~
Mx2125::mx_acceleration_x()
Mx2125::mx_acceleration_y()
~~~
Measure acceleration in terms of g (acceleration due to earth's gravity).

Returns value that represents the milli G's acting on the axis.

Milli G's seems more intuitive to me, so calculations based upon [2] for acceleration are the default.  Comment out `#define USE_MILLI_G` in `Mx2125.h` if you wish to use Parallax's formula with a nominal division of 1/1250 G instead.

~~~
Mx2125::mx_tilt_x()
Mx2125::mx_tilt_y()
~~~
Measure the level of tilt in terms of +/- 90 degrees. 0 degrees is when the top surface of the accelerometer chip is parallel to the ground.

~~~
Mx2125::mx_rotation()
~~~
Measure clockwise rotation assuming accelerometer is held vertical. Zero degree rotation is when the triangle on the MX2125 chip is pointing up.

Returns integer degree value (0 to 359) that represents the clockwise angle of rotation.

~~~
Mx2125::mx_temperature()
~~~
Returns floating point number of the temperature in Celsius as reported by the MX2125 chip.  The formula used is calculated for use with the ADC bit width of an Arduino UNO and will need to be changed if your model is different.  See [3] for details on how T-Out is calibrated.  Note that this seems to fluctuate quite a bit, but should give an idea as to the approximate operating temperature... I found very sparse documentation for its use and practically no examples, but for my purposes it is close enough, though I may return to see if I can tweak it later.

### Example

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
