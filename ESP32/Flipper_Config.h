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
  int enable;
  int home;
  byte flipState;
  int flipAmount;
  int flapPosition;
};

// Milliseconds between consecutive flips for a single split-flap display.
// REQUIRED: must be >= 75 to function properly
#define LATCH_TIME -1
// the number of entries in the flippers array
#define FLIPPER_AMOUNT -1

extern Flipper flippers[FLIPPER_AMOUNT];

#endif