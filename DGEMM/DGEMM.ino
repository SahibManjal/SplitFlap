int in1Pin = 11;
int in2Pin = 9;
int enablePin = 6;
int homePin = 5;

byte latchTime = 75;
byte flipState = LOW;
byte isHome;

void setup() {
  // put your setup code here, to run once:
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(homePin, INPUT);
  Serial.begin(9600);
  singleFlip();
}

void loop() {
  // singleFlip();
  goHome();
}

void goHome() {
  // Move to Home Position
  for (int i = 0; i <= 255; i++) {
    isHome = digitalRead(homePin);
    Serial.println(isHome);
    Serial.println("----------------");

    if (isHome == LOW) {
      singleFlip();
    }
    else {
      delay(10000);
      break;
    }
    delay(50);
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
  delay(1000);
}
