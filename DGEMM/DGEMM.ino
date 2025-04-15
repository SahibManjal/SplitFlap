int in1Pin = 11;
int in2Pin = 9;
int enablePin = 6;

byte latchTime = 75;
byte flipState = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  flipState = flipState == HIGH ? LOW : HIGH;

  digitalWrite(enablePin, HIGH);
  digitalWrite(in1Pin, flipState);
  digitalWrite(in2Pin, !flipState);
  delay(latchTime);
  digitalWrite(enablePin, LOW);
  delay(5000);
}
