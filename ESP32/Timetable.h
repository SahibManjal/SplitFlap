#ifndef TIMETABLE_H

#define TIMETABLE_H

#include <Arduino.h>

struct TimetableEntry
{
  String location;
  int hour;
  int minutes;
  int destinationFlap;
  int stopFlap;
};

#endif