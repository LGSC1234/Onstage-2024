#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27    // Dirección I2C del LCD
#define LCD_COLUMNS 16      // Número de columnas del LCD
#define LCD_ROWS 2          // Número de filas del LCD

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);  // Inicializa el LCD mediante I2C

const int RelayLed1 = 2;   // Pin para controlar el relé 1
const int RelayLed2 = 3;   // Pin para controlar el relé 2
const int RelayLed3 = 4;   // Pin para controlar el relé 3
const int RelayLed4 = 5;   // Pin para controlar el relé 4

void setup() {
  Serial.begin(9600);     // Comunicación serial 9600 baudios
  pinMode(RelayLed1, OUTPUT);   // Pin del relé 1 como salida
  pinMode(RelayLed2, OUTPUT);   // Pin del relé 2 como salida
  pinMode(RelayLed3, OUTPUT);   // Pin del relé 3 como salida
  pinMode(RelayLed4, OUTPUT);   // Pin del relé 4 como salida
  lcd.init();               // Inicializa el LCD
  lcd.backlight();          // Enciende el LCD
  lcd.clear();              // Limpia el contenido del LCD
  lcd.print("Desactivado"); // Muestra un mensaje inicial en el LCD
}

void loop() {
  if (Serial.available() > 0) {  // Verifica si hay datos disponibles en el puerto serial
    char option = Serial.read();  // Lee el carácter recibido desde el puerto serial
    
    // Control de relés según la opción recibida
    if (option >= '1' && option <= '1') {
      digitalWrite(RelayLed1, HIGH);  // Activa el relé 1
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: ON 1");
    }
    
    if (option >= '2' && option <= '2') {
      digitalWrite(RelayLed1, LOW);   // Desactiva el relé 1
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: OFF 1");
    }
    
    if (option >= '3' && option <= '3') {
      digitalWrite(RelayLed2, HIGH);  // Activa el relé 2
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: ON 2");
    }
    
    if  (option >= '4' && option <= '4') {
      digitalWrite(RelayLed2, LOW);   // Desactiva el relé 2
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: OFF 2");
    }
    
    if (option >= '5' && option <= '5') {
      digitalWrite(RelayLed3, HIGH);  // Activa el relé 3
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: ON 3");
    }
    
    if (option >= '6' && option <= '6') {
      digitalWrite(RelayLed3, LOW);   // Desactiva el relé 3
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: OFF 3");
    }
    
    if (option >= '7' && option <= '7') {
      digitalWrite(RelayLed4, HIGH);  // Activa el relé 4
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: ON 4");
    }
    
    if (option >= '8' && option <= '8') {
      digitalWrite(RelayLed4, LOW);   // Desactiva el relé 4
      lcd.clear();
      lcd.print("Activado");
      lcd.setCursor(0, 1);
      lcd.print("Estatus: OFF 4");
    }
  }
}
