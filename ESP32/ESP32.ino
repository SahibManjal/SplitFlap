#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "WiFi_Info.h"
#include "Flipper_Config.h"
#include "Timetable.h"

// Time holders
int hour;
int minutes;
enum stateType {NORMAL, TIMEDROP, STALLTIME};
enum stateType state;

// Going home phase
int initialHomeBool = 1;

// TimeTable Index
int currentTrain;
String response;

// Displayed TimeTable Index
int displayedTrain = 0;

// Empty Times
char* emptyTimes[] = {"Pass", "Terminates Here", "Blank", "Not In Service", ""};

// Server Info
int port = 8080;
char serverAddress[] = "10.20.141.6";
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

TimetableEntry *timetable = nullptr;
int timetableLength;

int mod(int x, int n) {
  int rem = x % n;
  if (rem < 0) {
    rem += n;
  }
  return rem;
}


void updateTime() {
  struct tm currentTime;
  getLocalTime(&currentTime);
  hour = currentTime.tm_hour;
  minutes = currentTime.tm_min;
}

void getTimetable() {
  client.get("/");
  int statusCode = client.responseStatusCode();
  response = client.responseBody();

  // Allocate the JSON document
  JsonDocument doc;

  // Deserialize the JSON document and handle error
  while (DeserializationError error = deserializeJson(doc, response)) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }

  // Turn Json into TimetableEntry
  JsonArray array = doc.as<JsonArray>();
  delete[] timetable;
  timetableLength = array.size();
  timetable = new TimetableEntry[timetableLength];
  int index = 0;
  for(JsonVariant v : array) {
    JsonObject extracted = v.as<JsonObject>();
    timetable[index].location = String(extracted["location"]);
    timetable[index].hour = extracted["hour"];
    timetable[index].minutes = extracted["minutes"];
    timetable[index].destinationFlap = extracted["destinationFlap"];
    timetable[index].stopFlap = extracted["stopFlap"];
    index++;
  }
}


void setup() {
  for (int i = 0; i < FLIPPER_AMOUNT; i++) {
    pinMode(flippers[i].in1, OUTPUT);
    pinMode(flippers[i].in2, OUTPUT);
    pinMode(flippers[i].enable, OUTPUT);
    pinMode(flippers[i].home, INPUT);
  }
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
  }

  configTime(0, 0, "pool.ntp.org");
  setenv("TZ","JST-9",1);
  tzset();

  updateTime();

  // Determine what Timetable to use
  getTimetable();

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
  updateTime();
  
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

    for (int i = 0; i < FLIPPER_AMOUNT; i++) {
      int nextFlap = 0;
      switch (flippers[i].type) {
        case DESTINATION:
          nextFlap = timetable[currentTrain].destinationFlap;
          break;
        case STOP_PATTERN:
          nextFlap = timetable[currentTrain].stopFlap;
          break;
        case HOUR:
          nextFlap = noTime ? 60 : timetable[currentTrain].hour + 1;
          break;
        case TENS_MINUTE:
          nextFlap = noTime ? 60 : timetable[currentTrain].minutes / 10 + 1;
          break;
        case ONES_MINUTE:
          nextFlap = noTime ? 60 : timetable[currentTrain].minutes % 10 + 1;
          break;
      }
      flippers[i].flipAmount = mod(nextFlap - flippers[i].flapPosition, 60);
    }

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
            getTimetable();
          }
        }
        break;
      case TIMEDROP:
        if ((trainStopTime < hour * 60 + minutes) || hour == 0) {
          currentTrain = nextTrain;
          // Starts new timetable if necessary
          if (currentTrain == 0) {
            getTimetable();
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
