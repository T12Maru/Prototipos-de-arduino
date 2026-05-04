#include <SoftwareSerial.h>

const int rxpin = 2;
const int txpin = 3;

SoftwareSerial serial_lcd(rxpin,txpin);

void setup() {
  Serial.begin(9600);
  serial_lcd.begin(9600);
}

int number = 0;

void loop() {
  serial_lcd.print("El número es ");
  serial_lcd.println(number);

  Serial.print("El número es ");
  Serial.println(number);

  delay(500);
  number++;
}
