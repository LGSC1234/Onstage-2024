#include <Adafruit_PWMServoDriver.h>  // Librería para controlar el driver de servo PCA9685
#include <SPI.h>                      // Librería SPI para la comunicación SPI
#include <MFRC522.h>                  // Librería para el lector RFID MFRC522
#include <Wire.h>                     // Comunicación I2C
#include <LiquidCrystal_I2C.h>        // Controlar LCD mediante I2C
#include <SparkFun_APDS9960.h>        // Sensor de gestos APDS9960

#define SS_PIN 10                      // Pin para el SS del MFRC522
#define RST_PIN 9                      // Pin para el RST del MFRC522
#define LCD_ADDRESS 0x27               // Dirección I2C del LCD
#define LCD_COLUMNS 16                 // Número de columnas del LCD
#define LCD_ROWS 2                     // Número de filas del LCD
#define APDS9960_INT 2                 // Pin de interrupción para el sensor de gestos APDS9960

MFRC522 mfrc522(SS_PIN, RST_PIN);      // Objeto lector RFID MFRC522
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);  // Objeto LCD mediante I2C
SparkFun_APDS9960 apds;                 // Objeto ensor de gestos APDS9960

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40); // Driver PCA9685 para control de servos

byte Usuario1[4] = {0xA3, 0xE4, 0x05, 0xAC}; // UID de la tarjeta RFID autorizada
bool activado = false; // Estado de activación/desactivación

int pos0=102; // Posición de servo para ángulo 0 grados
int pos180=512; // Posición de servo para ángulo 180 grados

void setup() {
  pinMode(APDS9960_INT, INPUT); // pin de interrupción del APDS9960 como entrada
  Serial.begin(9600); // Comunicación serial a 9600
  SPI.begin(); // Comunicación SPI
  servos.begin(); // Controlador PCA9685
  servos.setPWMFreq(50); // Frecuencia PWM del PCA9685
  mfrc522.PCD_Init(); // Inicializa el MFRC522
  lcd.init(); // Inicializa el LCD
  lcd.backlight(); // Enciende la retroiluminación del LCD
  lcd.clear(); // Limpia el contenido del LCD
  lcd.print("Desactivado"); // Muestra mensaje inicial en el LCD

  attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING); // Asocia la rutina de interrupción al pin APDS9960_INT
 
  if (apds.init()) {
    Serial.println(F("APDS-9960 initialization complete")); // Inicialización exitosa del APDS9960
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!")); // Error durante la inicialización del APDS9960
  }

  if (apds.enableGestureSensor(true)) {
    Serial.println(F("Gesture sensor is now running")); // Inicio exitoso del sensor de gestos APDS9960
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!")); // Error durante el inicio del sensor de gestos APDS9960
  }
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (verificarUsuario()) {
      activado = !activado; // Cambia el estado de activación/desactivación
      if (activado) {
        lcd.clear();
        lcd.print("Activado");
        lcd.setCursor(0, 1);
        lcd.print("Estatus: ");
      } else {
        lcd.clear();
        lcd.print("Desactivado");
      }
      delay(1000); // Evitar múltiples lecturas mientras la tarjeta está presente
    }
  }

  if (activado) {
    handleGesture(); // Maneja los gestos si el sistema está activado
  }
}

bool verificarUsuario() {
  for (int i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != Usuario1[i])
      return false; // Tarjeta RFID no autorizada
  }
  mfrc522.PICC_HaltA(); // Finaliza la comunicación con la tarjeta RFID
  return true; // Tarjeta RFID autorizada
}

void interruptRoutine() {
  // No se necesita una acción aquí si solo queremos manejar el gesto en el bucle principal
}

void handleGesture() {
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP: // Gesto: Arriba -> Configura servos para mirar hacia arriba
        setServo(0,0); // Izquierda arriba
        setServo(1,180);// Derrecha arriba
        setServo(2,180);// Izquierda abajo
        setServo(3,0);  // Derecha abajo
        lcd.setCursor(0,1);
        lcd.print("Estatus: Arriba ");
        break;
      case DIR_DOWN:  // Gesto: Abajo -> Configura servos para mirar hacia abajo
        setServo(0,180);
        setServo(1,0);
        setServo(2,180);
        setServo(3,0);
        lcd.setCursor(0,1);
        lcd.print("Estatus: Abajo ");
        break;
      case DIR_LEFT: // Gesto: Izquierda -> Configura servos para mirar hacia la izquierda
        setServo(1, 0);
        setServo(0,180);
        setServo(1,0);
        setServo(2,90);
        setServo(3,135);
        lcd.setCursor(0,1);
        lcd.print("Estatus: Izquierda ");
        break;
      case DIR_RIGHT: // Gesto: Derecha -> Configura servos para mirar hacia la derecha
        setServo(0,180);
        setServo(1,0);
        setServo(2,55);
        setServo(3,90);
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
    lcd.setCursor(0,0); // Regresa el cursor al inicio de la primera línea
    lcd.print("Activado");  // Muestra "Activado" en el LCD
  }
}

void setServo (uint8_t n_servo, int angulo) {
  int duty;
  duty=map(angulo,0,180,pos0,pos180); // Mapea el ángulo a valores de PWM
  servos.setPWM(n_servo, 0,duty);  // Configura el ángulo del servo mediante PWM
}
