#include <WiFi.h>
#include "time.h"
#include "WiFi_Info.h"
#include "Flipper_Config.h"
#include "Timetable.h"

// Time holders
int hour;
int minutes;
int wday;
int month;
int day;
enum stateType {NORMAL, TIMEDROP, STALLTIME};
enum stateType state;

// Going home phase
int initialHomeBool = 1;

// TimeTable Index
int currentTrain;

// Displayed TimeTable Index
int displayedTrain = 0;

// Empty Times
char* emptyTimes[] = {"Pass", "Terminates Here", "Blank", "Not In Service", ""};

TimetableEntry *timetable;
int timetableLength;

int mod(int x, int n) {
  int rem = x % n;
  if (rem < 0) {
    rem += n;
  }
  return rem;
}

int useWeekend() {
  return (wday == 0) || (wday == 6) || (month == 1 && day == 1) || (month == 1 && day >= 8 && day <= 14 && wday == 1) || 
  (month == 2 && day == 11) || (month == 2 && day == 23) || (month == 3 && day == 20) || (month == 4 && day == 29) ||
  (month == 5 && day == 3) || (month == 5 && day == 4) || (month == 5 && day == 5) || (month == 7 && day >= 15 && day <= 21 && wday == 1) ||
  (month == 8 && day == 11) || (month == 9 && day >= 15 && day <= 21 && wday == 1) || (month == 9 && day == 23) ||
  (month == 10 && day >= 8 && day <= 14 && wday == 1) || (month == 11 && day == 3) || (month == 11 && day == 23);
}


void updateTime(int dayUpdate) {
  struct tm currentTime;
  getLocalTime(&currentTime);
  hour = currentTime.tm_hour;
  minutes = currentTime.tm_min;
  if (dayUpdate) {
    wday = currentTime.tm_wday;
    month = currentTime.tm_mon + 1;
    day = currentTime.tm_mday;
  }
}


void setup() {
  for (int i = 0; i < FLIPPER_AMOUNT; i++) {
    pinMode(flippers[i].in1, OUTPUT);
    pinMode(flippers[i].in2, OUTPUT);
    pinMode(flippers[i].enable, OUTPUT);
    pinMode(flippers[i].home, INPUT);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
  }

  configTime(0, 0, "pool.ntp.org");
  setenv("TZ","JST-9",1);
  tzset();

  updateTime(1);

  // Determine what Timetable to use
  if (useWeekend()) {
    timetable = weekendTimetable;
    timetableLength = WEEKEND_TIMETABLE_LENGTH;
  } 
  else {
      timetable = weekdayTimetable;
      timetableLength = WEEKDAY_TIMETABLE_LENGTH;
  }

  state = NORMAL;

  // Get what current Train
  for (int i = 0; i < timetableLength; i++) {
    if (timetable[i].hour * 60 + timetable[i].minutes > hour * 60 + minutes) {
      currentTrain = i;
      break;
    }
  }
}

void loop() {
  updateTime(0);

  // Determine what Timetable to use
  if (useWeekend()) {
    timetable = weekendTimetable;
    timetableLength = WEEKEND_TIMETABLE_LENGTH;
  } 
  else {
      timetable = weekdayTimetable;
      timetableLength = WEEKDAY_TIMETABLE_LENGTH;
  }
  // All Flippers go Home when we start
  if (initialHomeBool) {
    initialHomeBool = !goHomeTick();
  }
  // Move All Flippers to new Positions
  else if (displayedTrain != currentTrain) {
    int noTime = 0;
    for (int i = 0; emptyTimes[i] != ""; i++) {
      if (timetable[currentTrain].location == emptyTimes[i]) {
        noTime = 1;
      }
    }
    flippers[0].flipAmount = mod(timetable[currentTrain].destinationFlap - flippers[0].flapPosition, 60);
    flippers[1].flipAmount = mod(timetable[currentTrain].stopFlap - flippers[1].flapPosition, 60);
    // if (noTime) {
    //   flippers[2].flipAmount = 60 - flippers[2].flapPosition;
    //   flippers[3].flipAmount = 60 - flippers[3].flapPosition;
    //   flippers[4].flipAmount = 60 - flippers[4].flapPosition;
    // }
    // else {
    //   flippers[2].flipAmount = mod(timetable[currentTrain].hour - flippers[2].flapPosition + 1, 60);
    //   flippers[3].flipAmount = mod(timetable[currentTrain].minutes / 10 - flippers[3].flapPosition + 1, 60);
    //   flippers[4].flipAmount = mod(timetable[currentTrain].minutes % 10 - flippers[4].flapPosition + 1, 60);
    // }
    int haveFlips = 1;
    while (haveFlips) {
      haveFlips = goNewPositionTick();
    }
    displayedTrain = currentTrain;
  }
  // Updates to New Timetable Position
  else {
    int trainStopTime = timetable[displayedTrain].hour * 60 + timetable[displayedTrain].minutes;
    int nextTrain = mod(currentTrain + 1, timetableLength);
    int nextTrainStopTime = timetable[nextTrain].hour * 60 + timetable[nextTrain].minutes;
    // Handles moving from hour 23 to hour 0
    switch (state) {
      case NORMAL:
        if (nextTrainStopTime < trainStopTime) {
          state = TIMEDROP;
        }
        if (trainStopTime < hour * 60 + minutes) {
          currentTrain = nextTrain;
          // Starts new timetable if necessary
          if (currentTrain == 0) {
            updateTime(1);
          }
        }
        break;
      case TIMEDROP:
        if ((trainStopTime < hour * 60 + minutes) || hour == 0) {
          currentTrain = nextTrain;
          // Starts new timetable if necessary
          if (currentTrain == 0) {
            updateTime(1);
          }
          state = STALLTIME;
        }
        break;
      case STALLTIME:
        if (trainStopTime == hour * 60 + minutes) {
          state = NORMAL;
        }
        break;
    }
  }
}

int goNewPositionTick() {
  // Moves all Flipers towards the New Position
  int noFlips = FLIPPER_AMOUNT;
  enableAllFlippers();
  for (int i = 0; i < FLIPPER_AMOUNT; i++) {
    if (flippers[i].flipAmount) {
      singleFlip(i);
      flippers[i].flipAmount--;
      flippers[i].flapPosition = mod(flippers[i].flapPosition + 1, 60);
    }
    else {
      noFlips--;
    }
  }
  disableAllFlippers();
  return noFlips;
}


int goHomeTick() {
  // Moves all Flipers towards the Home Position
  int allHome = 0;
  enableAllFlippers();
  for (int i = 0; i < FLIPPER_AMOUNT; i++) {
    if (digitalRead(flippers[i].home)) {
      singleFlip(i);
    }
    else {
      allHome++;
    }
  }
  disableAllFlippers();
  return allHome == FLIPPER_AMOUNT;
}

void enableAllFlippers() {
  // Sets the enable pin HIGH for all flippers
  for (int i = 0; i < FLIPPER_AMOUNT; i++) {
    digitalWrite(flippers[i].enable, HIGH);
  }
}

void disableAllFlippers() {
  // Delays for `latchTime` ms and then sets the enable pin LOW for all flippers
  delay(LATCH_TIME);
  for (int i = 0; i < FLIPPER_AMOUNT; i++) {
    digitalWrite(flippers[i].enable, LOW);
  }
}

void singleFlip(int index) {
  // Advances a Flipper by a Single Flip
  digitalWrite(flippers[index].in1, flippers[index].flipState);
  digitalWrite(flippers[index].in2, !flippers[index].flipState);

  flippers[index].flipState = flippers[index].flipState == HIGH ? LOW : HIGH;
}
