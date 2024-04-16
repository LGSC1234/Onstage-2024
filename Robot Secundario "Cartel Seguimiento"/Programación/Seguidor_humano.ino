#include <NewPing.h>           // NewPing para sensor de ultrasonido
#include <Servo.h>             // Librería Servo para el motor
#include <AFMotor.h>           // Librería AFMotor para motores DC

#define LEFT A4               // Pin A4 LEFT para el sensor en la izquierda
#define RIGHT A5             // Pin A5 RIGHT para el sensor en derecha
#define TRIGGER_PIN A3        // Pin A2 TRIGGER_PIN para  sensor de ultrasonido
#define ECHO_PIN A2           // Pin A3 ECHO_PIN para el sensor de ultrasonido
#define MAX_DISTANCE 200      // La distancia máxima para el sensor de ultrasonido

unsigned int distance = 0;    // Variable para almacenar la distancia medida por el sensor de ultrasonido
unsigned int Right_Value = 0; // Variable para almacenar el valor leído del sensor de línea derecha
unsigned int Left_Value = 0;  // Variable para almacenar el valor leído del sensor de línea izquierda
int d = 30;                    // Distancia de detección para activar el giro

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Instancia del sensor de ultrasonido

// Creación de objetos de motor para dos motores DC
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);

Servo myservo;  // Objeto servo para controlar el servo motor
int pos = 0;    // Variable para almacenar la posición del servo motor

void setup() {
  Serial.begin(9600);   
  myservo.attach(10);   
   {
    for (pos = 90; pos <= 180; pos += 1) {  // De 90 a 180 grados
      myservo.write(pos);                   // Posición deseada
      delay(15);                            // espera 15 milisegundos
    }
    for (pos = 180; pos >= 0; pos -= 1) {   // De 180 a 0 grados
      myservo.write(pos);                   // Posición deseada
      delay(15);                            // espera 15 milisegundos
    }
    for (pos = 0; pos <= 90; pos += 1) {    // De 0 a 90 grados
      myservo.write(pos);                   // Posición deseada
      delay(15);                            // espera 15 milisegundos
    }
  }
  pinMode(RIGHT, INPUT); // Pin Right como Entrada
  pinMode(LEFT, INPUT);  // Pin Left como Entrada
}    

void loop() {
  delay(50); // Pausa entre para estabilizar las lecturas

  distance = sonar.ping_cm();       // Mide la distancia en centímetros con el sensor ultrasónico
  Serial.print("distance: ");       // Imprime un mensaje para la distancia medida
  Serial.println(distance);         // Imprime la distancia medida en el monitor serial

  Right_Value = digitalRead(RIGHT); // Lee el valor del sensor de línea derecho
  Left_Value = digitalRead(LEFT);   // Lee el valor del sensor de línea izquierdo
 
  Serial.print("RIGHT: ");          // Imprime un mensaje para el sensor de línea derecho
  Serial.println(Right_Value);      // Imprime el valor del sensor de línea derecho
  Serial.print("LEFT: ");           // Imprime un mensaje para el sensor de línea izquierdo
  Serial.println(Left_Value);       // Imprime el valor del sensor de línea izquierdo

  if (distance > 1 && distance < d) {
    // Avanzar si la distancia medida está dentro del rango deseado
    Motor1.setSpeed(150);  // Establece la velocidad del motor 1
    Motor1.run(FORWARD);   // Hace avanzar el motor 1 hacia adelante
    Motor2.setSpeed(150);  // Establece la velocidad del motor 2
    Motor2.run(FORWARD);   // Hace avanzar el motor 2 hacia adelante
  } else if (Right_Value == 1 && Left_Value == 0) {
    // Girar a la derecha si solo el sensor de línea derecho detecta la línea
    Motor1.setSpeed(150);   // Establece una velocidad menor en el motor 1
    Motor1.run(FORWARD);    // Hace avanzar el motor 1 hacia adelante
    Motor2.setSpeed(150);   // Establece una velocidad menor en el motor 2
    Motor2.run(BACKWARD);   // Hace retroceder el motor 2
    delay(150);             // Espera para realizar el giro
  } else if (Right_Value == 0 && Left_Value == 1) {
    // Girar a la izquierda si solo el sensor de línea izquierdo detecta la línea
    Motor1.setSpeed(150);   // Establece una velocidad menor en el motor 1
    Motor1.run(BACKWARD);   // Hace retroceder el motor 1
    Motor2.setSpeed(150);   // Establece una velocidad menor en el motor 2
    Motor2.run(FORWARD);    // Hace avanzar el motor 2 hacia adelante
    delay(150);             // Espera para realizar el giro
  } else {
    stop();  // Detiene ambos motores si no se cumple ninguna condición de movimiento
  }
}

void stop() {
  // Detiene ambos motores
  Motor1.setSpeed(0);    // Establece la velocidad del motor 1 en 0
  Motor1.run(RELEASE);   // Detiene el motor 1
  Motor2.setSpeed(0);    // Establece la velocidad del motor 2 en 0
  Motor2.run(RELEASE);   // Detiene el motor 2
}
