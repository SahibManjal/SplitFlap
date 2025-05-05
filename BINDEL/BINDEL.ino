#include <WiFi.h>
#include "time.h"

// WiFi info
const char* ssid = "SpectrumSetup-F27F";
const char* password = "neatsystem293";

// Time holders
int hour;
int minutes;
int wday;
enum stateType {NORMAL, TIMEDROP, STALLTIME};
enum stateType state;

struct Flipper {
  String type;
  int in1;
  int in2;
  int enable;
  int home;
  byte flipState;
  int flipAmount;
  int flapPosition;
};

// Pin data
Flipper flippers[] = {
  {"Destination", 4, 15, 2, 25, LOW, 0, 0},
  {"Stops", 23, 22, 21, 33, LOW, 0, 0}
};

// State and Flap data
byte latchTime = 75;
byte parallelTime = 50 / 2;

// Going home phase
int initialHomeBool = 1;

// TimeTable Index
int currentTrain;

// Displayed TimeTable Index
int displayedTrain = 0;

struct Timetable
{
  String location;
  int hour;
  int minutes;
  int destinationFlap;
  int stopFlap;
};

Timetable *timetable;

Timetable weekdayTimetable[] = {
  {"Blank", 5, 8, 0, 0},
  {"L-Haibara", 5, 38, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 5, 51, 20, 25}, // Actual end is Nakagawa
  {"L-Haibara", 6, 3, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 6, 11, 20, 25}, // Actual end is Nakagawa
  {"L-Nabari", 6, 24, 30, 39},
  {"LE-Kashikojima", 6, 37, 5, 47},
  {"L-Haibara", 6, 38, 31, 39},
  {"Terminates Here", 6, 49, 38, 16}, // Added 1 minute to clear platform
  {"L-Haibara", 6, 54, 31, 39},
  {"LE-Nagoya", 7, 0, 1, 2},
  {"E-Isuzugawa", 7, 4, 21, 33},
  {"LE-Ujiyamada", 7, 14, 7, 53},
  {"L-Haibara", 7, 15, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 7, 21, 24, 36},
  {"LE-Nagoya", 7, 29, 1, 44},
  {"SE-Nabari", 7, 31, 27, 39},
  {"LE-Kashikojima", 7, 39, 5, 47},
  {"SSE-Haibara", 7, 40, 48, 39},
  {"E-Isuzugawa", 7, 47, 21, 33},
  {"L-Haibara", 7, 52, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 8, 1, 1, 2},
  {"E-Aoyamacho", 8, 2, 24, 36},
  {"SSE-Haibara", 8, 7, 48, 39},
  {"E-Toba", 8, 13, 20, 25}, // Actual end is Nakagawa
  {"LE-Ujiyamada", 8, 20, 7, 19},
  {"SE-Haibara", 8, 21, 28, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 8, 29, 1, 44},
  {"E-Nabari", 8, 30, 25, 38},
  {"LE-Kashikojima", 8, 41, 5, 47},
  {"L-Haibara", 8, 42, 31, 39}, // Actual end is Yamato-Asakura
  {"RE-Aoyamacho", 8, 49, 45, 27},
  {"L-Haibara", 8, 57, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 9, 3, 1, 2},
  {"LE-Toba", 9, 5, 6, 10},
  {"E-Toba", 9, 13, 20, 25}, // Actual end is Nakagawa
  {"LE-Ujiyamada", 9, 18, 7, 14},
  {"L-Haibara", 9, 21, 31, 39},
  {"LE-Nagoya", 9, 29, 1, 44},
  {"RE-Matsusaka", 9, 32, 44, 3},
  {"LE-Kashikojima", 9, 41, 5, 47},
  {"SE-Haibara", 9, 42, 28, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 9, 48, 24, 36},
  {"L-Haibara", 9, 56, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 10, 2, 1, 2},
  {"LE-Kashikojima", 10, 4, 5, 49},
  {"E-Toba", 10, 7, 20, 25}, // Actual end is Nakagawa
  {"SSE-Haibara", 10, 21, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 10, 29, 1, 44},
  {"E-Nabari", 10, 30, 25, 38},
  {"LE-Kashikojima", 10, 41, 5, 47},
  {"L-Haibara", 10, 42, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 10, 48, 24, 36},
  {"Shimakaze-Kashikojima", 10, 54, 51, 20},
  {"SSE-Haibara", 10, 56, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 11, 2, 1, 2},
  {"E-Toba", 11, 5, 20, 25}, // Actual end is Nakagawa
  {"Shimakaze-Kashikojima", 11, 11, 51, 20},
  {"SSE-Haibara", 11, 12, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 11, 26, 25, 38},
  {"L-Haibara", 11, 32, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Kashikojima", 11, 41, 5, 47},
  {"LE-Nagoya", 12, 2, 1, 2},
  {"SSE-Haibara", 12, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 12, 8, 20, 25}, // Actual end is Nakagawa
  {"SSE-Haibara", 12, 19, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 12, 31, 25, 38},
  {"LE-Kashikojima", 12, 41, 5, 47},
  {"SSE-Haibara", 12, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 12, 50, 24, 36},
  {"LE-Nagoya", 13, 2, 1, 2},
  {"SSE-Haibara", 13, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 13, 8, 20, 25}, // Actual end is Nakagawa
  {"L-Haibara", 13, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 13, 31, 25, 38},
  {"LE-Kashikojima", 13, 41, 5, 47},
  {"SSE-Haibara", 13, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 13, 50, 24, 36},
  {"LE-Nagoya", 14, 2, 1, 2},
  {"SSE-Haibara", 14, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Isuzugawa", 14, 8, 21, 33},
  {"L-Haibara", 14, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 14, 31, 25, 38},
  {"LE-Kashikojima", 14, 41, 5, 47},
  {"SSE-Haibara", 14, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 14, 50, 24, 36},
  {"LE-Nagoya", 15, 2, 1, 2},
  {"SSE-Haibara", 15, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Isuzugawa", 15, 8, 21, 33},
  {"L-Haibara", 15, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Kashikojima", 15, 41, 5, 47},
  {"SSE-Haibara", 15, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 15, 50, 20, 25}, // Actual end is Nakagawa
  {"LE-Nagoya", 16, 2, 1, 2},
  {"SSE-Haibara", 16, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 16, 11, 24, 36},
  {"SSE-Haibara", 16, 22, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 16, 29, 1, 44},
  {"E-Isuzugawa", 16, 31, 21, 33},
  {"SSE-Haibara", 16, 36, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Kashikojima", 16, 42, 5, 47},
  {"E-Aoyamacho", 16, 49, 24, 36},
  {"SSE-Haibara", 16, 55, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 17, 2, 1, 2},
  {"E-Toba", 17, 9, 20, 25}, // Actual end is Nakagawa
  {"SSE-Haibara", 17, 12, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 17, 22, 24, 36},
  {"LE-Nagoya", 17, 29, 1, 44},
  {"SSE-Haibara", 17, 31, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Ujiyamada", 17, 36, 22, 33},
  {"LE-Toba", 17, 42, 6, 45},
  {"SE-Haibara", 17, 47, 28, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 17, 52, 24, 36},
  {"LE-Nagoya", 18, 2, 1, 2},
  {"LE-Toba", 18, 4, 6, 10},
  {"SSE-Haibara", 18, 7, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Matsusaka", 18, 14, 8, 17},
  {"E-Toba", 18, 15, 20, 25}, // Actual end is Nakagawa
  {"SE-Haibara", 18, 23, 28, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 18, 29, 24, 36},
  {"SE-Haibara", 18, 36, 28, 39}, // Actual end is Yamato-Asakura
  {"LE-Toba", 18, 42, 6, 53}, // Actual end is Isuzugawa
  {"RE-Nabari", 18, 43, 46, 28},
  {"SE-Haibara", 18, 47, 28, 39},
  {"RE-Aoyamacho", 18, 55, 45, 26},
  {"LE-Nagoya", 19, 2, 1, 2},
  {"SE-Nabari", 19, 3, 27, 39},
  {"RE-Aoyamacho", 19, 10, 45, 26},
  {"LE-Matsusaka", 19, 14, 8, 17},
  {"SE-Haibara", 19, 19, 28, 39}, // Actual end is Yamato-Asakura
  {"RE-Matsusaka", 19, 24, 44, 3},
  {"SE-Haibara", 19, 30, 28, 39},
  {"LE-Ujiyamada", 19, 42, 7, 53},
  {"RE-Aoyamacho", 19, 43, 45, 26},
  {"SE-Haibara", 19, 48, 28, 39},
  {"RE-Aoyamacho", 19, 55, 45, 26},
  {"LE-Nagoya", 20, 2, 1, 2},
  {"SE-Nabari", 20, 3, 27, 39},
  {"RE-Toba", 20, 10, 11, 21},
  {"LE-Matsusaka", 20, 14, 8, 17},
  {"SE-Haibara", 20, 19, 28, 39}, // Actual end is Yamato-Asakura
  {"RE-Aoyamacho", 20, 24, 45, 26},
  {"SE-Haibara", 20, 30, 28, 39},
  {"LE-Matsusaka", 20, 42, 8, 17},
  {"RE-Matsusaka", 20, 43, 44, 3},
  {"LE-Nagoya", 20, 49, 1, 2},
  {"SE-Haibara", 20, 50, 28, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 20, 56, 24, 36},
  {"LE-Nagoya", 21, 2, 1, 2},
  {"SE-Haibara", 21, 7, 28, 39}, // Actual end is Yamato-Asakura
  {"E-Ujiyamada", 21, 11, 22, 33},
  {"LE-Matsusaka", 21, 22, 8, 17},
  {"SSE-Haibara", 21, 23, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 21, 30, 25, 38},
  {"LE-Matsusaka", 21, 42, 8, 17},
  {"SSE-Haibara", 21, 43, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 21, 49, 24, 36},
  {"E-Aoyamacho", 21, 57, 24, 36},
  {"LE-Nagoya", 22, 2, 1, 2},
  {"SSE-Haibara", 22, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 22, 12, 24, 36},
  {"SE-Haibara", 22, 15, 28, 39},
  {"LE-Matsusaka", 22, 23, 8, 17},
  {"E-Aoyamacho", 22, 26, 24, 36},
  {"SSE-Haibara", 22, 30, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nabari", 22, 42, 9, 18},
  {"E-Aoyamacho", 22, 44, 24, 36},
  {"SSE-Haibara", 22, 45, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nabari", 23, 0, 9, 18},
  {"E-Aoyamacho", 23, 3, 24, 36},
  {"SSE-Haibara", 23, 4, 48, 39}, // Actual end is Yamato-Asakura
  {"SSE-Haibara", 23, 17, 48, 39},
  {"Terminates Here", 23, 26, 38, 16}, // Added 1 minute to clear platform
  {"SE-Nabari", 23, 33, 27, 39},
  {"LE-Nabari", 23, 42, 9, 18},
  {"L-Haibara", 23, 43, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 23, 49, 24, 36},
  {"SSE-Haibara", 23, 57, 48, 39},
  {"Terminates Here", 0, 8, 38, 16}, // Added 1 minute to clear platform
  {"SSE-Haibara", 0, 17, 48, 39},
  {"Terminates Here", 0, 29, 38, 16} // Added 1 minute to clear platform
};

Timetable weekendTimetable[] = {
  {"Blank", 5, 8, 0, 0},
  {"L-Haibara", 5, 38, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 5, 52, 20, 25}, // Actual end is Nakagawa
  {"L-Haibara", 6, 6, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 6, 15, 25, 25},
  {"L-Haibara", 6, 24, 31, 39},
  {"LE-Nagoya", 6, 29, 1, 44},
  {"LE-Kashikojima", 6, 37, 5, 47},
  {"L-Haibara", 6, 38, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Isuzugawa", 6, 44, 21, 33},
  {"LE-Nagoya", 7, 0, 1, 2},
  {"SE-Haibara", 7, 3, 28, 39},
  {"LE-Ujiyamada", 7, 12, 7, 53},
  {"E-Isuzugawa", 7, 16, 21, 33},
  {"SSE-Nabari", 7, 23, 47, 39},
  {"LE-Nagoya", 7, 29, 1, 44},
  {"LE-Kashikojima", 7, 38, 5, 47},
  {"SSE-Haibara", 7, 39, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Isuzugawa", 7, 45, 21, 33},
  {"SSE-Haibara", 7, 53, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 8, 2, 1, 2},
  {"LE-Toba", 8, 3, 6, 10},
  {"E-Nabari", 8, 7, 25, 38},
  {"SSE-Haibara", 8, 8, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Ujiyamada", 8, 16, 7, 19},
  {"SSE-Haibara", 8, 23, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 8, 29, 1, 44},
  {"E-Ujiyamada", 8, 31, 22, 25}, // Actual end is Matsusaka
  {"LE-Kashikojima", 8, 40, 5, 47},
  {"SE-Haibara", 8, 42, 28, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 8, 50, 1, 44},
  {"E-Aoyamacho", 8, 54, 24, 36},
  {"L-Haibara", 8, 55, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 9, 2, 1, 2},
  {"LE-Kashikojima", 9, 4, 5, 49},
  {"E-Toba", 9, 9, 20, 25}, // Actual end is Nakagawa
  {"SSE-Haibara", 9, 10, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Ujiyamada", 9, 19, 7, 19},
  {"L-Haibara", 9, 20, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 9, 29, 1, 44},
  {"E-Aoyamacho", 9, 31, 24, 36},
  {"SSE-Haibara", 9, 36, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Toba", 9, 41, 6, 45},
  {"E-Nabari", 9, 50, 25, 38},
  {"SSE-Haibara", 9, 56, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 10, 2, 1, 2},
  {"LE-Kashikojima", 10, 4, 5, 49},
  {"E-Toba", 10, 9, 20, 25}, // Actual end is Nakagawa
  {"L-Haibara", 10, 10, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Toba", 10, 16, 6, 10},
  {"SSE-Haibara", 10, 20, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 10, 29, 1, 44},
  {"E-Nabari", 10, 30, 25, 38},
  {"L-Haibara", 10, 36, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Kashikojima", 10, 42, 5, 47},
  {"E-Aoyamacho", 10, 46, 24, 36},
  {"LE-Nagoya", 10, 49, 1, 44},
  {"Shimakaze-Kashikojima", 10, 54, 51, 20},
  {"SSE-Haibara", 10, 57, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 11, 2, 1, 2},
  {"Shimakaze-Kashikojima", 11, 11, 51, 20},
  {"E-Toba", 11, 12, 20, 25}, // Actual end is Nakagawa
  {"LE-Toba", 11, 19, 6, 10},
  {"SSE-Haibara", 11, 20, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 11, 31, 25, 38},
  {"LE-Kashikojima", 11, 41, 5, 47},
  {"SSE-Haibara", 11, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 11, 50, 24, 36},
  {"LE-Nagoya", 12, 2, 1, 2},
  {"SSE-Haibara", 12, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 12, 7, 20, 25}, // Actual end is Nakagawa
  {"L-Haibara", 12, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 12, 31, 25, 38},
  {"LE-Kashikojima", 12, 41, 5, 47},
  {"SSE-Haibara", 12, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 12, 50, 24, 36},
  {"LE-Nagoya", 13, 2, 1, 2},
  {"SSE-Haibara", 13, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 13, 8, 20, 25}, // Actual end is Nakagawa
  {"L-Haibara", 13, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 13, 31, 25, 38},
  {"LE-Kashikojima", 13, 41, 5, 47},
  {"SSE-Haibara", 13, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 13, 50, 24, 36},
  {"LE-Nagoya", 14, 2, 1, 2},
  {"SSE-Haibara", 14, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Isuzugawa", 14, 12, 21, 33},
  {"L-Haibara", 14, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 14, 31, 25, 38},
  {"LE-Kashikojima", 14, 41, 5, 47},
  {"SSE-Haibara", 14, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 14, 50, 24, 36},
  {"LE-Nagoya", 15, 2, 1, 2},
  {"SSE-Haibara", 15, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Isuzugawa", 15, 12, 21, 33},
  {"L-Haibara", 15, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 15, 31, 25, 38},
  {"LE-Kashikojima", 15, 41, 5, 47},
  {"SSE-Haibara", 15, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 15, 50, 20, 25}, // Actual end is Nakagawa
  {"LE-Nagoya", 16, 2, 1, 2},
  {"SSE-Haibara", 16, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 16, 7, 24, 36},
  {"L-Haibara", 16, 19, 31, 39}, // Actual end is Yamato-Asakura
  {"E-Toba", 16, 31, 20, 25}, // Actual end is Nakagawa
  {"LE-Kashikojima", 16, 41, 5, 47},
  {"SSE-Haibara", 16, 42, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 16, 50, 1, 44},
  {"E-Aoyamacho", 16, 51, 24, 36},
  {"LE-Nagoya", 17, 2, 1, 2},
  {"SSE-Haibara", 17, 3, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 17, 8, 24, 36},
  {"SE-Haibara", 17, 11, 28, 39}, // Actual end is Yamato-Asakura
  {"E-Nabari", 17, 21, 25, 38},
  {"SSE-Haibara", 17, 35, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Toba", 17, 42, 6, 45},
  {"E-Nabari", 17, 45, 25, 38},
  {"LE-Nagoya", 17, 51, 1, 44},
  {"RE-Isuzugawa", 17, 57, 12, 22},
  {"SSE-Haibara", 17, 58, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 18, 2, 1, 2},
  {"LE-Toba", 18, 4, 6, 10},
  {"E-Nabari", 18, 9, 25, 38},
  {"SE-Haibara", 18, 10, 28, 39}, // Actual end is Yamato-Asakura
  {"LE-Matsusaka", 18, 15, 8, 17},
  {"RE-Aoyamacho", 18, 20, 45, 26},
  {"L-Haibara", 18, 21, 31, 39},
  {"E-Aoyamacho", 18, 32, 24, 36},
  {"SE-Haibara", 18, 33, 28, 39}, // Actual end is Yamato-Asakura
  {"LE-Ujiyamada", 18, 40, 7, 53},
  {"L-Haibara", 18, 45, 31, 41},
  {"LE-Nagoya", 18, 50, 1, 44},
  {"RE-Ujiyamada", 18, 52, 13, 24},
  {"SE-Haibara", 18, 56, 28, 39}, // Actual end is Yamato-Asakura
  {"LE-Nagoya", 19, 2, 1, 2},
  {"RE-Aoyamacho", 19, 3, 45, 26},
  {"L-Nabari", 19, 8, 30, 39},
  {"LE-Toba", 19, 15, 6, 4},
  {"RE-Toba", 19, 17, 11, 21},
  {"SSE-Haibara", 19, 20, 48, 39},
  {"RE-Aoyamacho", 19, 31, 45, 26},
  {"SSE-Haibara", 19, 35, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Toba", 19, 41, 6, 53}, // Actual end is Isuzugawa
  {"E-Toba", 19, 49, 20, 25}, // Actual end is Nakagawa
  {"SE-Haibara", 19, 52, 28, 39}, // Actual end is Yamato-Asakura
  {"RE-Nabari", 19, 57, 46, 28},
  {"LE-Nagoya", 20, 2, 1, 2},
  {"SSE-Haibara", 20, 9, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Toba", 20, 16, 6, 58}, // Actual end is Isuzugawa
  {"E-Aoyamacho", 20, 17, 24, 36},
  {"SSE-Haibara", 20, 21, 48, 39},
  {"RE-Nabari", 20, 34, 46, 28},
  {"SSE-Haibara", 20, 36, 48, 39}, // Actual end is Yamato-Asakura
  {"LE-Matsusaka", 20, 41, 8, 17},
  {"LE-Nagoya", 20, 49, 1, 2},
  {"E-Nabari", 20, 50, 25, 38},
  {"SSE-Haibara", 20, 55, 48, 39},
  {"LE-Nagoya", 21, 3, 1, 2},
  {"E-Ujiyamada", 21, 4, 22, 33},
  {"SSE-Haibara", 21, 9, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 21, 16, 24, 36},
  {"LE-Matsusaka", 21, 22, 8, 17},
  {"SSE-Haibara", 21, 29, 48, 39},
  {"LE-Matsusaka", 21, 42, 8, 17},
  {"E-Nabari", 21, 44, 25, 38},
  {"SSE-Haibara", 21, 48, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 21, 59, 24, 36},
  {"LE-Nagoya", 22, 3, 1, 2},
  {"E-Aoyamacho", 22, 13, 24, 36},
  {"L-Haibara", 22, 17, 31, 39}, // Actual end is Yamato-Asakura
  {"LE-Matsusaka", 22, 22, 8, 17},
  {"SSE-Haibara", 22, 30, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 22, 36, 24, 36},
  {"LE-Nabari", 22, 41, 9, 18},
  {"SSE-Haibara", 22, 46, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 23, 2, 24, 36},
  {"SSE-Haibara", 23, 8, 48, 39},
  {"LE-Nabari", 23, 21, 9, 18},
  {"E-Aoyamacho", 23, 24, 24, 36},
  {"SSE-Haibara", 23, 32, 48, 39}, // Actual end is Yamato-Asakura
  {"E-Aoyamacho", 23, 48, 24, 36},
  {"SSE-Haibara", 23, 57, 48, 39},
  {"Terminates Here", 0, 9, 38, 16}, // Added 1 minute to clear platform
  {"SSE-Haibara", 0, 17, 48, 39},
  {"Terminates Here", 0, 30, 38, 16}, // Added 1 minute to clear platform
};

// Get timetable lengths
int weekdayTimetableLength = sizeof(weekdayTimetable) / sizeof(weekdayTimetable[0]);
int weekendTimetableLength = sizeof(weekendTimetable) / sizeof(weekendTimetable[0]);
int timetableLength;

int mod(int x, int n) {
  int rem = x % n;
  if (rem < 0) {
    rem += n;
  }
  return rem;
}


int previousTime = millis();
void updateTime(int dayUpdate) {
  struct tm currentTime;
  getLocalTime(&currentTime);
  hour = currentTime.tm_hour;
  minutes = currentTime.tm_min;
  if (dayUpdate) {
    wday = currentTime.tm_wday;
  }
}


void setup() {
  for (int i = 0; i < sizeof(flippers) / sizeof(flippers[0]); i++) {
    pinMode(flippers[i].in1, OUTPUT);
    pinMode(flippers[i].in2, OUTPUT);
    pinMode(flippers[i].enable, OUTPUT);
    pinMode(flippers[i].home, INPUT);
  }

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
  }

  configTime(0, 0, "pool.ntp.org");
  setenv("TZ","JST-9",1);
  tzset();

  updateTime(1);

  // Determine what Timetable to use
  if (wday == 0 || wday == 6) {
    timetable = weekendTimetable;
    timetableLength = weekendTimetableLength;
  } 
  else {
      timetable = weekdayTimetable;
      timetableLength = weekdayTimetableLength;
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
  if (wday == 0 || wday == 6) {
    timetable = weekendTimetable;
    timetableLength = weekendTimetableLength;
  } 
  else {
      timetable = weekdayTimetable;
      timetableLength = weekdayTimetableLength;
  }
  // All Flippers go Home when we start
  if (initialHomeBool) {
    int allHome = goHomeTick();
    if (allHome) {
      initialHomeBool = 0;
    }
  }
  // Move All Flippers to new Positions
  else if (displayedTrain != currentTrain) {
    flippers[0].flipAmount = mod(timetable[currentTrain].destinationFlap - flippers[0].flapPosition, 60);
    flippers[1].flipAmount = mod(timetable[currentTrain].stopFlap - flippers[1].flapPosition, 60);
    int haveFlips = 1;
    while (haveFlips) {
      haveFlips = goNewPositionTick();
    }
    flippers[0].flapPosition = timetable[currentTrain].destinationFlap;
    flippers[1].flapPosition = timetable[currentTrain].stopFlap;
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
            wday = mod(wday + 1, 7);
          }
        }
        break;
      case TIMEDROP:
        if ((trainStopTime < hour * 60 + minutes) || hour == 0) {
          currentTrain = nextTrain;
          // Starts new timetable if necessary
          if (currentTrain == 0) {
            wday = mod(wday + 1, 7);
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
  int noFlips = sizeof(flippers) / sizeof(flippers[0]);
  for (int i = 0; i < sizeof(flippers) / sizeof(flippers[0]); i++) {
    if (flippers[i].flipAmount) {
      singleFlip(i);
      flippers[i].flipAmount--;
    }
    else {
      delay(latchTime);
      noFlips--;
    }
    delay(parallelTime);
  }
  return noFlips;
}


int goHomeTick() {
  // Moves all Flipers towards the Home Position
  int allHome = 0;
  for (int i = 0; i < sizeof(flippers) / sizeof(flippers[0]); i++) {
    if (digitalRead(flippers[i].home)) {
      singleFlip(i);
    }
    else {
      delay(latchTime);
      allHome++;
    }
    delay(parallelTime);
  }
  return allHome == sizeof(flippers) / sizeof(flippers[0]);
}


void singleFlip(int index) {
  // Advances a Flipper by a Single Flip
  digitalWrite(flippers[index].enable, HIGH);
  digitalWrite(flippers[index].in1, flippers[index].flipState);
  digitalWrite(flippers[index].in2, !flippers[index].flipState);
  delay(latchTime);
  digitalWrite(flippers[index].enable, LOW);

  flippers[index].flipState = flippers[index].flipState == HIGH ? LOW : HIGH;
}