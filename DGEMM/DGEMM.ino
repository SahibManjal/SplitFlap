int in1Pin = 11;
int in2Pin = 9;
int enablePin = 6;
int homePin = 5;

byte latchTime = 75;
byte flipState = LOW;
byte isHome;

const int destinationFlips[] = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int timetable[] = {6, 2, 8, 5, 0, 3, 1, 4, 9, 7};

unsigned long previousDestination = 15000;
int index = 0;
int currentStop;

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
    previousDestination += 15000;
    currentStop = timetable[index];
    for (int i = 0; i < destinationFlips[currentStop]; i++) {
      singleFlip();
    }
    index += 1;
    if (index % 10 == 0) {
      index -= 10;
    }
    delay(15000);
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