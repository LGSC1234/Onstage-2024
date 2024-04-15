//Arduino Human Following Robot

//include the library code:
#include <NewPing.h>           
#include <Servo.h>             
#include <AFMotor.h>           

#define LEFT A5             // Right IR sensor connected to analog pin A2 of Arduino Uno:
#define RIGHT A1            // Left IR sensor connected to analog pin A3 of Arduino Uno:
#define TRIGGER_PIN A2      // Trigger pin connected to analog pin A1 of Arduino Uno:
#define ECHO_PIN A3         // Echo pin connected to analog pin A0 of Arduino Uno:
#define MAX_DISTANCE 200    // Maximum ping distance:

unsigned int distance = 0;    // Variable to store ultrasonic sensor distance:
unsigned int Right_Value = 0; // Variable to store Right IR sensor value:
unsigned int Left_Value = 0;  // Variable to store Left IR sensor value:
int d = 10;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance:

// Create motor objects for two motors
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);

Servo myservo; // Create servo object to control the servo:
int pos = 0;   // Variable to store the servo position:

void setup() {
  Serial.begin(9600);   // Initialize serial communication at 9600 bits per second:
  myservo.attach(10);    // Servo attached to pin 10 of Arduino UNO
  stop();               // Stop motors initially
}

void loop() {
  delay(50);                        // Wait 50ms between pings:
  distance = sonar.ping_cm();       // Send ping, get distance in cm and store it in 'distance' variable:
  Serial.print("distance: ");
  Serial.println(distance);         // Print the distance in serial monitor:

  Right_Value = digitalRead(RIGHT); // Read the value from Right IR sensor:
  Left_Value = digitalRead(LEFT);   // Read the value from Left IR sensor:
 
  Serial.print("RIGHT: ");
  Serial.println(Right_Value);      // Print the right IR sensor value in serial monitor:
  Serial.print("LEFT: ");
  Serial.println(Left_Value);       // Print the left IR sensor value in serial monitor:

  if (distance > 1 && distance < d) {
    // Move Forward:
    Motor1.setSpeed(130);
    Motor1.run(FORWARD);
    Motor2.setSpeed(130);
    Motor2.run(FORWARD);
  } else if (Right_Value == 1 && Left_Value == 0) {
    // Turn right
    Motor1.setSpeed(150);
    Motor1.run(FORWARD);
    Motor2.setSpeed(150);
    Motor2.run(BACKWARD);
    delay(150);
  } else if (Right_Value == 0 && Left_Value == 1) {
    // Turn left
    Motor1.setSpeed(150);
    Motor1.run(BACKWARD);
    Motor2.setSpeed(150);
    Motor2.run(FORWARD);
    delay(150);
  } else {
    stop(); // Stop motors if none of the above conditions are met
  }
}

void stop() {
  // Stop both motors
  Motor1.setSpeed(0);
  Motor1.run(RELEASE);
  Motor2.setSpeed(0);
  Motor2.run(RELEASE);
}
