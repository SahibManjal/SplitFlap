#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi info
const char* ssid = "SpectrumSetup-F27F";
const char* password = "neatsystem293";

// Current Time info
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "jp.pool.ntp.org");

// Time holders
int hour;
int minutes;

// Pin data
int in1Pin1 = 4;
int in2Pin1 = 15;
int enablePin1 = 2;
int homePin1 = 25;
int in1Pin2 = 23;
int in2Pin2 = 22;
int enablePin2 = 21;
int homePin2 = 33;

// State and Flap data
byte latchTime = 75;
byte flipState = LOW;

// Going home phase
int notHomeBool1;
int notHomeBool2;
int initialHomeBool;

// TimeTable Index
int currentTrain;

// Displayed TimeTable Index
int displayedTrain = 3;

// byte isHomeDestination;
// byte isHomeStops;

struct Timetable
{
  String location;
  int hour;
  int minutes;
  int destinationFlap;
  int stopFlap;
};

Timetable timetable[] = {
    {"Terminates Here", 0, 8, 38, 16},
    {"SSE-Haibara", 0, 17, 48, 39},
    {"Terminates Here", 0, 29, 38, 16},
    {"Blank", 5, 8, 0, 0},   
    {"L-Haibara", 5, 38, 31, 39}, // Actual end is Yamato-Asakura
    {"E-Toba", 5, 51, 20, 25}, // Actual end is Nakagawa
    {"L-Haibara", 6, 3, 31, 39}, // Actual end is Yamato-Asakura
    {"E-Toba", 6, 11, 20, 25}, // Actual end is Nakagawa
    {"L-Nabari", 6, 24, 30, 39},
    {"LE-Kashikojima", 6, 37, 5, 47},
    {"L-Haibara", 6, 38, 31, 39},
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
    {"Terminates Here", 23, 26, 38, 16},
    {"SE-Nabari", 23, 33, 27, 39},
    {"LE-Nabari", 23, 42, 9, 18},
    {"L-Haibara", 23, 43, 31, 39}, // Actual end is Yamato-Asakura
    {"E-Aoyamacho", 23, 49, 24, 36},
    {"SSE-Haibara", 23, 57, 48, 39}
};



// String currentStop = timetable[0].location;
// String retrieveStop = currentStop;
// // These variables required for edge case when consecutive trains are same type and destination
// int requiredSet = 0;
// int currentSet = 0;

int mod(int x, int n) {
  int rem = x % n;
  if (rem < 0) {
    rem += n;
  }
  return rem;
}


void setup() {
  pinMode(in1Pin1, OUTPUT);
  pinMode(in2Pin1, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(homePin1, INPUT);
  pinMode(in1Pin2, OUTPUT);
  pinMode(in2Pin2, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(homePin2, INPUT);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    Serial.println("SNEE");
    delay(500);
  }

  timeClient.begin();
  timeClient.update();

  hour = timeClient.getHours() + 9;
  minutes = timeClient.getMinutes();

  notHomeBool1 = 1;
  notHomeBool2 = 1;
  initialHomeBool = 1;

  for (int i = 0; i < sizeof(timetable) / sizeof(timetable[0]); i++) {
    if (timetable[i].hour * 60 + timetable[i].minutes > hour * 60 + minutes) {
      currentTrain = i;
      Serial.println(i);
      break;
    }
  }

  // retrieveStop = retrieveStation(retrieveStop, curHour, curMinute);
  // setStation(retrieveStop);
  // currentStop = retrieveStop;
  // currentSet = requiredSet;
}

void loop() {
  // All Flippers go Home when we start
  if (initialHomeBool) {
    goHome();
    if (!notHomeBool1 && !notHomeBool2) {
      initialHomeBool = 0;
    }
  }
  else if (displayedTrain != currentTrain) {
    int destinationFlips = mod(timetable[currentTrain].destinationFlap - timetable[displayedTrain].destinationFlap, 60) + 1;
    int stopFlips = mod(timetable[currentTrain].stopFlap - timetable[displayedTrain].stopFlap, 60) + 1;
    while (destinationFlips > 0 || stopFlips > 0) {
      if (destinationFlips > 0) {
        Serial.println("Flip");
        singleFlip(enablePin1, in1Pin1, in2Pin1, flipState);
        destinationFlips -= 1;
      }
      delay(25);
      if (stopFlips > 0) {
        singleFlip(enablePin2, in1Pin2, in2Pin2, flipState);
        stopFlips -= 1;
      }
      delay(25);
      flipState = flipState == HIGH ? LOW : HIGH;
    }
    displayedTrain = currentTrain;
  }
  // if (timeClient.getSeconds() == 0) {
  //     hour = timeClient.getHours() % 24;
  //     minute = timeClient.getMinutes();

  // retrieveStop = retrieveStation(retrieveStop, curHour, curMinute);

  // if (retrieveStop != currentStop | currentSet != requiredSet) {
  //   setStation(retrieveStop);
  //   currentStop = retrieveStop;
  //   currentSet = requiredSet;
}

void goHome() {
  // Moves all Flipers towards the Home Position
  if (notHomeBool1 || notHomeBool2) {
    notHomeBool1 = digitalRead(homePin1) == HIGH ? 1 : 0;
    notHomeBool2 = digitalRead(homePin2) == HIGH ? 1 : 0;

    goHomeFlip(homePin1, enablePin1, in1Pin1, in2Pin1, flipState);
    goHomeFlip(homePin2, enablePin2, in1Pin2, in2Pin2, flipState);

    if ((!notHomeBool1 && notHomeBool2) || (notHomeBool1 && !notHomeBool2)) {
      delay(latchTime);
    }

    flipState = flipState == HIGH ? LOW : HIGH;
  }
}

void goHomeFlip(int homePin, int enablePin, int inputPin1, int inputPin2, byte flipState) {
  // Move a Flipper towards Home Position
  byte notHome = digitalRead(homePin);

  if (notHome != LOW) {
    singleFlip(enablePin, inputPin1, inputPin2, flipState);
  }
  delay(25);

}

void singleFlip(int enablePin, int inputPin1, int inputPin2, byte flipState) {
  // Advances a Flipper by a Single Flip
  digitalWrite(enablePin, HIGH);
  digitalWrite(inputPin1, flipState);
  digitalWrite(inputPin2, !flipState);
  delay(latchTime);
  digitalWrite(enablePin, LOW);
}


// // Set for-loop to 23 for 23 positions in flap lookup table
// void setStation(String station) {
//   goHome1();
//   int flaps = 0;
//   for (int j = 0; j < 23; j++) {
//     if (station == mappedTimetable[j].location) {
//       flaps = mappedTimetable[j].flips;
//     }
//   }
//   for (int i = 0; i < flaps; i++) {
//     singleFlip1();
//   }
//   goHome2();
//   int patternRequired = timetable[requiredSet].pattern;
//   for (int l = 0; l < patternRequired; l++) {
//     singleFlip2();
//   }
// }

// // Set for-loop to 116 for 116 positions in timetable
// String retrieveStation(String station, int hour, int minute) {
//   for (int i = 0; i < 116; i++) {
//     if ((timetable[i].hour == hour & timetable[i].minute >= minute) | timetable[i].hour > hour) {
//       requiredSet = i;
//       return timetable[i].location;
//     }
//   }
//   requiredSet = 0;
//   return timetable[0].location;
// }