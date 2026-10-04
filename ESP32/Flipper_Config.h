#ifndef FLIPPER_CONFIG_H

#define FLIPPER_CONFIG_H

#include <Arduino.h>

enum FlipperType {
  DESTINATION,
  STOP_PATTERN,
  HOUR,
  TENS_MINUTE,
  ONES_MINUTE,
};

struct Flipper {
  FlipperType type;
  int in1;
  int in2;
  int home;
};

// Milliseconds between consecutive flips for a single split-flap display.
// REQUIRED: must be >= 75 to function properly
#define LATCH_TIME 120
// the number of entries in the flippers array
#define FLIPPER_AMOUNT 4
// the gpio pin connected to both hall sensors. When set to HIGH it senses when
// the flipper reaches home, when set to LOW it senses if a flip actually 
// occured (i.e. there was some mechanical failire).
#define HOME_OR_ERROR_PIN 2

// Pin data
Flipper flippers[FLIPPER_AMOUNT] = {
    {DESTINATION, 15, 4, 25},
    {STOP_PATTERN, 22, 23, 34},
    {TENS_MINUTE, 16, 17, 33},
    {ONES_MINUTE, 5, 18, 32},
};

#endif