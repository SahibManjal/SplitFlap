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

extern const int WEEKDAY_TIMETABLE_LENGTH;
extern const int WEEKEND_TIMETABLE_LENGTH;
extern TimetableEntry weekdayTimetable[];
extern TimetableEntry weekendTimetable[];

#endif