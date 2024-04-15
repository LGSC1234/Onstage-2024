#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>


Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

int pos0=102;
int pos180=512;

void setup() {
  servos.begin();
  servos.setPWMFreq(50);
}

void loop() {
  setServo(0,0);
  setServo(1,0);
  setServo(2,0);
  setServo(3,0);
  delay(1000);
  setServo(0,90);
  setServo(1,90);
  setServo(2,90);
  setServo(3,90);
  delay(1000);
  setServo(0,180);
  setServo(1,180);
  setServo(2,180);
  setServo(3,180);
  delay(1000);
}

void setServo (uint8_t n_servo, int angulo) {
  int duty;
  duty=map(angulo,0,180,pos0,pos180);
  servos.setPWM(n_servo, 0,duty);
}
