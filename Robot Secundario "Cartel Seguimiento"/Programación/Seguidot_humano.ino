#include <NewPing.h>           
#include <Servo.h>             
#include <AFMotor.h>           

#define LEFT A5             
#define RIGHT A4            
#define TRIGGER_PIN A2      
#define ECHO_PIN A3         
#define MAX_DISTANCE 200    

unsigned int distance = 0;    
unsigned int Right_Value = 0; 
unsigned int Left_Value = 0;  
int d = 15;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

// Create motor objects for two motors
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);

Servo myservo; 
int pos = 0;  

void setup() {
  Serial.begin(9600);   
  myservo.attach(10);   
  stop();              
}

void loop() {
  delay(50); 
  distance = sonar.ping_cm();       
  Serial.print("distance: ");
  Serial.println(distance);        

  Right_Value = digitalRead(RIGHT); 
  Left_Value = digitalRead(LEFT);  
 
  Serial.print("RIGHT: ");
  Serial.println(Right_Value);  
  Serial.print("LEFT: ");
  Serial.println(Left_Value);    
  if (distance > 1 && distance < d) {
    // Move Forward:
    Motor1.setSpeed(150);
    Motor1.run(FORWARD);
    Motor2.setSpeed(150);
    Motor2.run(FORWARD);
  } else if (Right_Value == 1 && Left_Value == 0) {
    // Turn right
    Motor1.setSpeed(130);
    Motor1.run(FORWARD);
    Motor2.setSpeed(130);
    Motor2.run(BACKWARD);
    delay(150);
  } else if (Right_Value == 0 && Left_Value == 1) {
    // Turn left
    Motor1.setSpeed(130);
    Motor1.run(BACKWARD);
    Motor2.setSpeed(130);
    Motor2.run(FORWARD);
    delay(150);
  } else {
    stop();
  }
}

void stop() {
  // Stop both motors
  Motor1.setSpeed(0);
  Motor1.run(RELEASE);
  Motor2.setSpeed(0);
  Motor2.run(RELEASE);
}
