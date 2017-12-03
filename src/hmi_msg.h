#include <avr/pgmspace.h>

#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define NAME_BEGIN "H"
#define NAME_END "li Ann Reintam"

#define VER_LIBC "Version: " FW_VERSION "built on: " __DATE__" "__TIME__"\n"
#define VER_FW "avr-libc version: "__AVR_LIBC_VERSION_STRING__ "\n"

#define ENTER_NUMBER "\nEnter number >"
#define YOUR_NUMBER "\nYou have entered number "
#define NOT_IN_RANGE "\nPlease enter number between 0 and 9!"
#define EMPTY_LINE "        "
const char no_0[] PROGMEM = "zero";
const char no_1[] PROGMEM = "one";
const char no_2[] PROGMEM = "two";
const char no_3[] PROGMEM = "three";
const char no_4[] PROGMEM = "four";
const char no_5[] PROGMEM = "five";
const char no_6[] PROGMEM = "six";
const char no_7[] PROGMEM = "seven";
const char no_8[] PROGMEM = "eight";
const char no_9[] PROGMEM = "nine";
PGM_P const numbers[] PROGMEM={no_0, no_1, no_2, no_3, no_4, no_5, no_6, no_7, no_8, no_9};

#endif /* _HMI_MSG_H_ */
