int in1Pin = 11;
int in2Pin = 9;
int enablePin = 6;
int homePin = 5;

byte latchTime = 75;
byte flipState = LOW;
byte isHome;

unsigned long previousDestination = 10000;
int index = 0;
String currentStop;

const String timetable[] = {"Six", "Two", "Eight", "Five", "Zero", "Three", "One", "Four", "Nine", "Seven"};

struct Mapping
{
  String location;
  int flips;
};

Mapping mappedTimetable[] = {
   {"One", 1},
   {"Two", 2},
   {"Three", 3},
   {"Four", 4},
   {"Five", 5},
   {"Six", 6},
   {"Seven", 7},
   {"Eight", 8},
   {"Nine", 9},
   {"Zero", 10},
};

void setup() {
  // put your setup code here, to run once:
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(homePin, INPUT);
  Serial.begin(9600);
}

void loop() {
  goHome();
  if (millis() - previousDestination >= 0) {
    previousDestination += 10000;
    currentStop = timetable[index];
    for (int i = 0; i < 10; i++) {
      if (mappedTimetable[i].location == currentStop){
        for (int j = 0; j < mappedTimetable[i].flips; j++) {
          singleFlip();
        }
        index += 1;
        if (index % 10 == 0) {
          index -= 10;
        }
        delay(10000);
        break;
      }
    }
  }

}

void goHome() {
  // Move to Home Position
  for (int i = 0; i <= 255; i++) {
    isHome = digitalRead(homePin);

    if (isHome != LOW) {
      singleFlip();
    }
    else {
      break;
    }
  }
}

void singleFlip() {
  // Advances a Single Flip
  flipState = flipState == HIGH ? LOW : HIGH;

  digitalWrite(enablePin, HIGH);
  digitalWrite(in1Pin, flipState);
  digitalWrite(in2Pin, !flipState);
  delay(latchTime);
  digitalWrite(enablePin, LOW);
  delay(50);
}