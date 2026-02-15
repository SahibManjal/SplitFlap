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

#define LATCH_TIME 120
#define FLIPPER_AMOUNT 2

extern Flipper flippers[FLIPPER_AMOUNT];

#endif