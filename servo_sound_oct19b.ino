#include <Servo.h>

Servo myServo;
int servoPin = 9;
int angle = 0;  // Servo position

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
}

void loop() {
  if (Serial.available() > 0) {
    int command = Serial.parseInt();  // Read the incoming serial data

    if (command == 1) {
      // Move the servo to 90 degrees if volume is at max
      myServo.write(90);  // Adjust as needed for your servo
    } else if (command == 0) {
      // Stop the servo by setting it to a neutral position
      myServo.write(0);  // Adjust as needed
    }
  }
}

