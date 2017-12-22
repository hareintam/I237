#include <avr/pgmspace.h>
#include "hmi_msg.h"

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

PGM_P const numbers[] PROGMEM = {no_0, no_1, no_2, no_3, no_4, no_5, no_6, no_7, no_8, no_9};

const char name_begin[] PROGMEM = "H";
const char name_end[] PROGMEM = "li Ann Reintam";
PGM_P const stud_name[] PROGMEM = {name_begin, name_end};
