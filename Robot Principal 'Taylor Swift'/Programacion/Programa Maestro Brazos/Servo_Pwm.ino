#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SparkFun_APDS9960.h>
#include <SoftwareSerial.h>

#define SS_PIN 10
#define RST_PIN 9
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define APDS9960_INT 2

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
SparkFun_APDS9960 apds;
SoftwareSerial BT1(6, 7); // RX, TX para la comunicación Bluetooth con el esclavo

byte Usuario1[4] = {0xA3, 0xE4, 0x05, 0xAC};
bool activado = false;

void setup() {
  pinMode(APDS9960_INT, INPUT);
  Serial.begin(9600);
  BT1.begin(38400); // Iniciar la comunicación Bluetooth
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Desactivado");

  attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);

  if (apds.init()) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  if (apds.enableGestureSensor(true)) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (verificarUsuario()) {
      activado = !activado;
      if (activado) {
        lcd.clear();
        lcd.print("Activado");
        lcd.setCursor(0, 1);
        lcd.print("Estatus: ");
        BT1.write("1"); // Envía el código "1" al esclavo para activarlo
      } else {
        lcd.clear();
        lcd.print("Desactivado");
        BT1.write("0"); // Envía el código "0" al esclavo para desactivarlo
      }
      delay(1000); // Evitar múltiples lecturas mientras la tarjeta está presente
    }
  }

  if (activado) {
    handleGesture();
  }
}

bool verificarUsuario() {
  for (int i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != Usuario1[i])
      return false;
  }
  mfrc522.PICC_HaltA();
  return true;
}

void interruptRoutine() {
  // No se necesita una acción aquí si solo queremos manejar el gesto en el bucle principal
}

void handleGesture() {
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        lcd.setCursor(0,1);
        lcd.print("Estatus: Arriba ");
        break;
      case DIR_DOWN:
        lcd.setCursor(0,1);
        lcd.print("Estatus: Abajo ");
        break;
      case DIR_LEFT:
        lcd.setCursor(0,1);
        lcd.print("Estatus: Izquierda ");
        break;
      case DIR_RIGHT:
        lcd.setCursor(0,1);
        lcd.print("Estatus: Derecha ");
        break;
      case DIR_NEAR:
        lcd.setCursor(0,1);
        lcd.print("Estatus: Cerca ");
        break;
      case DIR_FAR:
        lcd.setCursor(0,1);
        lcd.print("Estatus: Lejos ");
        break;
      default:
        lcd.setCursor(0,1);
        lcd.print("Estatus: Ninguno ");
    }
    delay(1000); // Dar tiempo para que el gesto sea visible antes de limpiar la pantalla nuevamente
    lcd.setCursor(0,0);
    lcd.print("Activado"); 
  }
}
