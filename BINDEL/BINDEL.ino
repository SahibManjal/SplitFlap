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
    {"Blank",        4,  46,  0,  0}, // Move off Blank 30 minutes before first train
    {"L-Nakagawa",   5,  16, 44, 42},
    {"L-Nakagawa",   5,  36, 44, 42},
    {"E-Uehommachi", 5,  39, 32, 29}, // Actually for Nabari
    {"E-Uehommachi", 5,  51, 32, 29}, // Actually for Nabari
    {"L-Nakagawa",   6,   1, 44, 41},
    {"LE-Nagoya",    6,  13, 24, 26},
    {"L-Nakagawa",   6,  18, 44, 41},
    {"E-Nagoya",     6,  22, 34, 34},
    {"LE-Nagoya",    6,  34, 24, 26},
    {"L-Nakagawa",   6,  36, 44, 41},
    {"E-Nagoya",     6,  40, 34, 34},
    {"LE-Nagoya",    6,  52, 24, 26},
    {"E-Nagoya",     7,   1, 34, 37},
    {"LE-Nagoya",    7,   7, 24, 26},
    {"LE-Nagoya",    7,  24, 24, 26},
    {"E-Nagoya",     7,  31, 34, 34},
    {"L-Nakagawa",   7,  33, 44, 42},
    {"LE-Nagoya",    7,  44, 24, 26},
    {"E-Nagoya",     7,  57, 34, 37},
    {"LE-Nagoya",    8,   4, 24, 26},
    {"L-Nakagawa",   8,  12, 44, 42},
    {"L-Ujiyamada",  8,  31, 40, 43},
    {"E-Uehommachi", 8,  36, 32, 33},
    {"L-Myojo",      8,  52, 43, 42},
    {"L-Nakagawa",   9,   4, 44, 42},
    {"LE-Nagoya",    9,   8, 24, 26},
    {"E-Uehommachi", 9,  11, 32, 33},
    {"L-Nakagawa",   9,  29, 44, 41},
    {"E-Nagoya",     9,  32, 34, 34},
    {"LE-Namba",     9,  40,  1, 56},
    {"LE-Nagoya",    9,  49, 24, 26},
    {"L-Nakagawa",  10,   6, 44, 41},
    {"LE-Nagoya",   10,  10, 24, 25},
    {"E-Nagoya",    10,  13, 34, 34},
    {"L-Nakagawa",  10,  37, 44, 42},
    {"LE-Namba",    10,  40,  1, 56},
    {"LE-Nagoya",   10,  49, 24, 25},
    {"L-Nakagawa",  11,   7, 44, 41},
    {"LE-Nagoya",   11,  10, 24, 25},
    {"E-Nagoya",    11,  14, 34, 34},
    {"L-Nakagawa",  11,  37, 44, 42},
    {"LE-Namba",    11,  40,  1, 56},
    {"LE-Nagoya",   11,  49, 24, 25},
    {"L-Nakagawa",  12,   7, 44, 41},
    {"LE-Nagoya",   12,  10, 24, 25},
    {"E-Nagoya",    12,  14, 34, 34},
    {"L-Nakagawa",  12,  37, 44, 42},
    {"LE-Namba",    12,  40,  1, 56},
    {"LE-Nagoya",   12,  49, 24, 25},
    {"L-Nakagawa",  13,   7, 44, 41},
    {"LE-Nagoya",   13,  10, 24, 25},
    {"E-Nagoya",    13,  14, 34, 34},
    {"L-Nakagawa",  13,  37, 44, 42},
    {"LE-Namba",    13,  40,  1, 56},
    {"LE-Nagoya",   13,  49, 24, 25},
    {"L-Nakagawa",  14,   7, 44, 41},
    {"LE-Nagoya",   14,  10, 24, 25},
    {"E-Nagoya",    14,  14, 34, 34},
    {"L-Nakagawa",  14,  37, 44, 42},
    {"LE-Namba",    14,  40,  1, 55},
    {"LE-Nagoya",   14,  49, 24, 25},
    {"LE-Kyoto",    15,   1, 21, 22},
    {"LE-Nagoya",   15,  11, 24, 25},
    {"E-Nagoya",    15,  14, 34, 34},
    {"L-Nakagawa",  15,  16, 44, 42},
    {"Pass",        15,  29, 37,  0}, // Limited Express Shimakaze Kyoto
    {"L-Nakagawa",  15,  37, 44, 42},
    {"LE-Namba",    15,  40,  1, 55},
    {"LE-Nagoya",   15,  48, 24, 25},
    {"E-Nagoya",    15,  55, 34, 34},
    {"LE-Kyoto",    16,   2, 21, 22},
    {"LE-Nagoya",   16,   9, 24, 25},
    {"E-Uehommachi",16,  13, 32, 33},
    {"L-Nakagawa",  16,  16, 44, 42},
    {"Pass",        16,  19, 37,  0}, // Limited Express Shimakaze Nagoya
    {"LE-Uehommachi",16, 24,  9, 19},
    {"Pass",        16,  38, 37,  0}, // Limited Express Shimakaze Namba
    {"L-Nakagawa",  16,  40, 44, 42},
    {"LE-Namba",    16,  44,  1, 55},
    {"LE-Nagoya",   16,  48, 24, 25},
    {"E-Nagoya",    16,  54, 34, 34},
    {"LE-Nagoya",   17,  10, 24, 25},
    {"E-Uehommachi",17,  13, 32, 33},
    {"L-Nakagawa",  17,  16, 44, 42},
    {"LE-Uehommachi",17, 24,  9, 17},
    {"E-Nagoya",    17,  35, 34, 34},
    {"L-Nakagawa",  17,  37, 44, 42},
    {"LE-Namba",    17,  40,  1, 55},
    {"LE-Nagoya",   17,  48, 24, 25},
    {"L-Nakagawa",  17,  58, 44, 41},
    {"E-Nagoya",    18,   3, 34, 34},
    {"LE-Nagoya",   18,  10, 24, 25},
    {"L-Nakagawa",  18,  16, 44, 42},
    {"L-Nakagawa",  18,  33, 44, 42},
    {"E-Uehommachi",18,  39, 32, 34}, // Actually for Nabari
    {"LE-Namba",    18,  43,  1, 55},
    {"LE-Nagoya",   18,  48, 24, 25},
    {"L-Nakagawa",  18,  52, 44, 41},
    {"E-Nagoya",    19,   1, 34, 34},
    {"LE-Nagoya",   19,  10, 24, 25},
    {"L-Nakagawa",  19,  17, 44, 42},
    {"LE-Namba",    19,  27,  1, 56},
    {"L-Nakagawa",  19,  34, 44, 42},
    {"LE-Nagoya",   19,  48, 24, 25},
    {"L-Shiratsuka",19,  51, 45, 42},
    {"LE-Nagoya",   20,  10, 24, 25},
    {"L-Nakagawa",  20,  19, 44, 42},
    {"E-Nagoya",    20,  23, 34, 34},
    {"LE-Namba",    20,  27,  1, 56},
    {"L-Nakagawa",  20,  49, 44, 42},
    {"LE-Nagoya",   21,   9, 24, 25},
    {"E-Nagoya",    21,  27, 34, 34},
    {"L-Nakagawa",  21,  35, 44, 42},
    {"L-Nakagawa",  22,  30, 44, 42},
    {"L-Nakagawa",  23,   6, 44, 42}
};



// String currentStop = timetable[0].location;
// String retrieveStop = currentStop;
// // These variables required for edge case when consecutive trains are same type and destination
// int requiredSet = 0;
// int currentSet = 0;


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

  hour = timeClient.getHours() % 24;
  minutes = timeClient.getMinutes();

  notHomeBool1 = 1;
  notHomeBool2 = 1;

  // retrieveStop = retrieveStation(retrieveStop, curHour, curMinute);
  // setStation(retrieveStop);
  // currentStop = retrieveStop;
  // currentSet = requiredSet;
}

void loop() {
  if (notHomeBool1 || notHomeBool2) {
    goHomeFlip(homePin1, enablePin1, in1Pin1, in2Pin1, flipState);
    goHomeFlip(homePin2, enablePin2, in1Pin2, in2Pin2, flipState);

    notHomeBool1 = digitalRead(homePin1) == HIGH ? 1 : 0;
    notHomeBool2 = digitalRead(homePin2) == HIGH ? 1 : 0;

    flipState = flipState == HIGH ? LOW : HIGH;
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

void goHomeFlip(int homePin, int enablePin, int inputPin1, int inputPin2, byte flipState) {
  // Move towards Home Position
  byte notHome = digitalRead(homePin);

  if (notHome != LOW) {
    singleFlip(enablePin, inputPin1, inputPin2, flipState);
  }
  delay(25);
}

void singleFlip(int enablePin, int inputPin1, int inputPin2, byte flipState) {
  // Advances a Single Flip
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