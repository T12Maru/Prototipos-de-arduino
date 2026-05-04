const int buttonPin = 2;
const int potXPin = 3;
const int potYPin = 1;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
}

void loop() {
  int x = (512 - analogRead(potXPin)) / 4;
  int y = (512 - analogRead(potYPin)) / 4;

  Serial.print("Data,");
  Serial.print(x,DEC);
  Serial.print(",");
  Serial.print(y,DEC);
  Serial.print(",");
  if (digitalRead(buttonPin) == LOW)
    Serial.print(1);
  else
    Serial.print(0);
  Serial.println(",");
  delay(50);
}
