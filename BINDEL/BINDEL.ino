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


byte isHomeDestination;
byte isHomeStops;

// Flipper Structure
struct Flipper
{
  String location;
  int flips;
};

Flipper mappedDestinations[] = {
   {"Blank", 0}, // Force end of service between 23h and 5h
   {"LE-Namba", 1},
   {"LE-Kyoto/Namba", 3},
   {"LE-Uehommachi", 9},
   {"E-Tsu", 13},
   {"LE-Kyoto", 21},
   {"LE-Nagoya", 24},
   {"RE-Uehommachi", 28},
   {"SRE-Uehommachi", 31},
   {"E-Uehommachi", 32},
   {"E-Nagoya", 34},
   {"Pass", 37},
   {"Charter", 38},
   {"Not In Service", 39},
   {"L-Ujiyamada", 40},
   {"L-Miyamachi", 42},
   {"L-Myojo", 43},
   {"L-Nakagawa", 44},
   {"L-Shiratsuka", 45},
   {"L-Higashi-Aoyama", 46},
   {"L-Nabari", 47},
   {"L-Toba", 48},
   {"L-Kashikojima", 49}
};

struct Timetable
{
  String location;
  int hour;
  int minutes;
  int futureStops;
};

// Weekday 16h timetable
Timetable timetable[] = {
   {"Blank", 4, 46, 0}, // Move off Blank 30 minutes before first train
   {"L-Nakagawa", 5, 16, 42},
   {"L-Nakagawa", 5, 36, 42},
   {"E-Uehommachi", 5, 39, 29}, // Actually for Nabari
   {"E-Uehommachi", 5, 51, 29}, // Actually for Nabari
   {"L-Nakagawa", 6, 1, 41},
   {"LE-Nagoya", 6, 13, 26},
   {"L-Nakagawa", 6, 18, 41},
   {"E-Nagoya", 6, 22, 34},
   {"LE-Nagoya", 6, 34, 26},
   {"L-Nakagawa", 6, 36, 41},
   {"E-Nagoya", 6, 40, 34},
   {"LE-Nagoya", 6, 52, 26},
   {"E-Nagoya", 7, 1, 37},
   {"LE-Nagoya", 7, 7, 26},
   {"LE-Nagoya", 7, 24, 26},
   {"E-Nagoya", 7, 31, 34},
   {"L-Nakagawa", 7, 33, 42},
   {"LE-Nagoya", 7, 44, 26},
   {"E-Nagoya", 7, 57, 37},
   {"LE-Nagoya", 8, 4, 26},
   {"L-Nakagawa", 8, 12, 42},
   {"L-Ujiyamada", 8, 31, 43},
   {"E-Uehommachi", 8, 36, 33},
   {"L-Myojo", 8, 52, 42},
   {"L-Nakagawa", 9, 4, 42},
   {"LE-Nagoya", 9, 8, 26},
   {"E-Uehommachi", 9, 11, 33},
   {"L-Nakagawa", 9, 29, 41},
   {"E-Nagoya", 9, 32, 34},
   {"LE-Namba", 9, 40, 56},
   {"LE-Nagoya", 9, 49, 26},
   {"L-Nakagawa", 10, 6, 41},
   {"LE-Nagoya", 10, 10, 25},
   {"E-Nagoya", 10, 13, 34},
   {"L-Nakagawa", 10, 37, 42},
   {"LE-Namba", 10, 40, 56},
   {"LE-Nagoya", 10, 49, 25},
   {"L-Nakagawa", 11, 7, 41},
   {"LE-Nagoya", 11, 10, 25},
   {"E-Nagoya", 11, 14, 34},
   {"L-Nakagawa", 11, 37, 42},
   {"LE-Namba", 11, 40, 56},
   {"LE-Nagoya", 11, 49, 25},
   {"L-Nakagawa", 12, 7, 41},
   {"LE-Nagoya", 12, 10, 25},
   {"E-Nagoya", 12, 14, 34},
   {"L-Nakagawa", 12, 37, 42},
   {"LE-Namba", 12, 40, 56},
   {"LE-Nagoya", 12, 49, 25},
   {"L-Nakagawa", 13, 7, 41},
   {"LE-Nagoya", 13, 10, 25},
   {"E-Nagoya", 13, 14, 34},
   {"L-Nakagawa", 13, 37, 42},
   {"LE-Namba", 13, 40, 56},
   {"LE-Nagoya", 13, 49, 25},
   {"L-Nakagawa", 14, 7, 41},
   {"LE-Nagoya", 14, 10, 25},
   {"E-Nagoya", 14, 14, 34},
   {"L-Nakagawa", 14, 37, 42},
   {"LE-Namba", 14, 40, 55},
   {"LE-Nagoya", 14, 49, 25},
   {"LE-Kyoto", 15, 1, 22},
   {"LE-Nagoya", 15, 11, 25},
   {"E-Nagoya", 15, 14, 34},
   {"L-Nakagawa", 15, 16, 42},
   {"Pass", 15, 29, 0}, // Limited Express Shimakaze Kyoto 
   {"L-Nakagawa", 15, 37, 42},
   {"LE-Namba", 15, 40, 55},
   {"LE-Nagoya", 15, 48, 25},
   {"E-Nagoya", 15, 55, 34},
   {"LE-Kyoto", 16, 2, 22},
   {"LE-Nagoya", 16, 9, 25},
   {"E-Uehommachi", 16, 13, 33},
   {"L-Nakagawa", 16, 16, 42},
   {"Pass", 16, 19, 0}, // Limited Express Shimakaze Nagoya
   {"LE-Uehommachi", 16, 24, 19},
   {"Pass", 16, 38, 0}, // Limited Express Shimakaze Namba
   {"L-Nakagawa", 16, 40, 42},
   {"LE-Namba", 16, 44, 55},
   {"LE-Nagoya", 16, 48, 25},
   {"E-Nagoya", 16, 54, 34},
   {"LE-Nagoya", 17, 10, 25},
   {"E-Uehommachi", 17, 13, 33},
   {"L-Nakagawa", 17, 16, 42},
   {"LE-Uehommachi", 17, 24, 17},
   {"E-Nagoya", 17, 35, 34},
   {"L-Nakagawa", 17, 37, 42},
   {"LE-Namba", 17, 40, 55},
   {"LE-Nagoya", 17, 48, 25},
   {"L-Nakagawa", 17, 58, 41},
   {"E-Nagoya", 18, 3, 34},
   {"LE-Nagoya", 18, 10, 25},
   {"L-Nakagawa", 18, 16, 42},
   {"L-Nakagawa", 18, 33, 42},
   {"E-Uehommachi", 18, 39, 34}, // Actually for Nabari
   {"LE-Namba", 18, 43, 55},
   {"LE-Nagoya", 18, 48, 25},
   {"L-Nakagawa", 18, 52, 41},
   {"E-Nagoya", 19, 1, 34},
   {"LE-Nagoya", 19, 10, 25},
   {"L-Nakagawa", 19, 17, 42},
   {"LE-Namba", 19, 27, 56},
   {"L-Nakagawa", 19, 34, 42},
   {"LE-Nagoya", 19, 48, 25},
   {"L-Shiratsuka", 19, 51, 42},
   {"LE-Nagoya", 20, 10, 25},
   {"L-Nakagawa", 20, 19, 42},
   {"E-Nagoya", 20, 23, 34},
   {"LE-Namba", 20, 27, 56},
   {"L-Nakagawa", 20, 49, 42},
   {"LE-Nagoya", 21, 9, 25},
   {"E-Nagoya", 21, 27, 34},
   {"L-Nakagawa", 21, 35, 42},
   {"L-Nakagawa", 22, 30, 42},
   {"L-Nakagawa", 23, 6, 42}
};


String currentStop = timetable[0].location;
String retrieveStop = currentStop;
// These variables required for edge case when consecutive trains are same type and destination
int requiredSet = 0;
int currentSet = 0;


void setup() {
  pinMode(in1Pin1, OUTPUT);
  pinMode(in2Pin1, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(homePin1, INPUT);
  pinMode(in1Pin2, OUTPUT);
  pinMode(in2Pin2, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(homePin2, INPUT);
  // Serial.begin(9600);

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
  }

  timeClient.begin();
  timeClient.update();

  hour = timeClient.getHours() % 24;
  minute = timeClient.getMinutes();

  retrieveStop = retrieveStation(retrieveStop, curHour, curMinute);
  setStation(retrieveStop);
  currentStop = retrieveStop;
  currentSet = requiredSet;
}

void loop() {
  if (timeClient.getSeconds() == 0) {
      hour = timeClient.getHours() % 24;
      minute = timeClient.getMinutes();

  retrieveStop = retrieveStation(retrieveStop, curHour, curMinute);

  if (retrieveStop != currentStop | currentSet != requiredSet) {
    setStation(retrieveStop);
    currentStop = retrieveStop;
    currentSet = requiredSet;
  }
  }
}  

void goHome(byte isHome, int homePin) {
  // Move to Home Position
  for (int i = 0; i <= 255; i++) {
    isHome = digitalRead(homePin);

    if (isHome != LOW) {
      singleFlip1();
    }
    else {
      break;
    }
  }
}

void goHome1() {
  // Move to Home Position
  for (int i = 0; i <= 255; i++) {
    isHome1 = digitalRead(homePin1);

    if (isHome1 != LOW) {
      singleFlip1();
    }
    else {
      break;
    }
  }
}

void goHome2() {
  // Move to Home Position
  for (int i = 0; i <= 255; i++) {
    isHome2 = digitalRead(homePin2);

    if (isHome2 != LOW) {
      singleFlip2();
    }
    else {
      break;
    }
  }
}

void singleFlip(int enablePin, int inputPin1, int inputPin2) {
  // Advances a Single Flip
  flipState = flipState == HIGH ? LOW : HIGH;

  digitalWrite(enablePin, HIGH);
  digitalWrite(inputPin1, flipState);
  digitalWrite(inputPin2, !flipState);
  delay(latchTime);
  digitalWrite(enablePin, LOW);
  delay(50);
}

void singleFlip1() {
  // Advances a Single Flip
  flipState = flipState == HIGH ? LOW : HIGH;

  digitalWrite(enablePin1, HIGH);
  digitalWrite(in1Pin1, flipState);
  digitalWrite(in2Pin1, !flipState);
  delay(latchTime);
  digitalWrite(enablePin1, LOW);
  delay(50);
}

void singleFlip2() {
  // Advances a Single Flip
  flipState = flipState == HIGH ? LOW : HIGH;

  digitalWrite(enablePin2, HIGH);
  digitalWrite(in1Pin2, flipState);
  digitalWrite(in2Pin2, !flipState);
  delay(latchTime);
  digitalWrite(enablePin2, LOW);
  delay(50);
}

// Set for-loop to 23 for 23 positions in flap lookup table
void setStation(String station) {
  goHome1();
  int flaps = 0;
  for (int j = 0; j < 23; j++) {
    if (station == mappedTimetable[j].location) {
      flaps = mappedTimetable[j].flips;
    }
  }
  for (int i = 0; i < flaps; i++) {
    singleFlip1();
  }
  goHome2();
  int patternRequired = timetable[requiredSet].pattern;
  for (int l = 0; l < patternRequired; l++) {
    singleFlip2();
  }
}

// Set for-loop to 116 for 116 positions in timetable
String retrieveStation(String station, int hour, int minute) {
  for (int i = 0; i < 116; i++) {
    if ((timetable[i].hour == hour & timetable[i].minute >= minute) | timetable[i].hour > hour) {
      requiredSet = i;
      return timetable[i].location;
    }
  }
  requiredSet = 0;
  return timetable[0].location;
}