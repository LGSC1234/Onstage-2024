#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

const int RelayLed1 = 2;
const int RelayLed2 = 3;
const int RelayLed3 = 4;
const int RelayLed4 = 5;

void setup() {
Serial.begin(9600);
pinMode (RelayLed1, OUTPUT);
pinMode (RelayLed2, OUTPUT);
pinMode (RelayLed3, OUTPUT);
pinMode (RelayLed4, OUTPUT);
lcd.init();
lcd.backlight(); 
lcd.clear();
lcd.print("Desactivado");
}

void loop() {
if (Serial.available()>0) {
char option = Serial.read();
     
if (option >= '1' && option <= '1') {
digitalWrite(RelayLed1, HIGH);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: ON 1");
}

if (option >= '2' && option <= '2') {
digitalWrite(RelayLed1, LOW);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: OFF 1");
}

if (option >= '3' && option <= '3') {
digitalWrite(RelayLed2, HIGH);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: ON 2");
}

if (option >= '4' && option <= '4') {
digitalWrite(RelayLed2, LOW);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: OFF 2");
}

if (option >= '5' && option <= '5') {
digitalWrite(RelayLed3, HIGH);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: ON 3");
}

if (option >= '6' && option <= '6') {
digitalWrite(RelayLed3, LOW);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: OFF 3");
}

if (option >= '7' && option <= '7') {
digitalWrite(RelayLed4, HIGH);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: ON 4");
}

if (option >= '8' && option <= '8') {
digitalWrite(RelayLed4, LOW);
 lcd.clear();
 lcd.print("Activado");
 lcd.setCursor(0, 1);
 lcd.print("Estatus: OFF 4");
}
}
}
