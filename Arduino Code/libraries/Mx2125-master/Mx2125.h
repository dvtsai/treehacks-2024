/*
 *  Mx2125.h
 *  Memsic MX2125 Dual-axis Accelerometer library
 *
 *  Created by Aaron Magill on 4/20/15.
 *  Copyright 2015 Aaron Magill, MIT License.
 *
 *  Inspired by code from the following:
 *  [1] http://learn.parallax.com/propeller-c-simple-devices/tilt-and-acceleration-mx2125
 *      specifically, the contentst of mx2125.h and mx2125.c in specified support library.
 *  [2] http://www.arduino.cc/en/Tutorial/Memsic2125?from=Tutorial.AccelerometerMemsic2125
 *  [3] https://www.jameco.com/Jameco/Products/ProdDS/282870.pdf
 *
 *  Milli G's seems more intuitive, so calculations based upon [2] for acceleration are
 * the default.  Comment out #define USE_MILLI_G if you wish to use Parallax's formula.
 *
 */

#ifndef _Mx2125_h
#define _Mx2125_h

// Comment this out if you want to use Parallax's 1250th of a G formula's instead (see
// link [1] above.)
#define USE_MILLI_G

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Mx2125{
  public:
    Mx2125(uint8_t, uint8_t, uint8_t);
    int16_t   mx_acceleration_x();
    int16_t   mx_acceleration_y();
    uint16_t  mx_rotation();
    int8_t    mx_tilt_x();
    int8_t    mx_tilt_y();
    float     mx_temperature() ;
  private:
    uint8_t   xPin, yPin, tPin ;
    int16_t   mx_acceleration(uint8_t);
    int8_t    mx_tilt(uint8_t);
};

#endif /* _Mx2125_h */
