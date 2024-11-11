#include <Servo.h>

// Pin Definitions
const int micPin = A0;     // Microphone connected to analog pin A0
const int servoPin = 9;    // Servo connected to digital pin D9

// Variables
Servo tailServo;           // Servo object
int micValue = 0;          // Raw microphone reading
int volume = 0;            // Calculated volume (RMS approximation)
const int threshold = 290; // Volume threshold to trigger servo movement

void setup() {
  Serial.begin(9600);        // Start Serial communication for debugging
  tailServo.attach(servoPin); // Attach servo to the defined pin
  tailServo.write(90);        // Set servo to neutral position
  pinMode(micPin, INPUT);     // Microphone as input
}

void loop() {
  // Calculate volume from microphone readings
  volume = calculateVolume();
  Serial.println(volume); // Debug: print volume to Serial Monitor

  // Check if volume exceeds the threshold
  if (volume > threshold) {
    Serial.println("Speaking detected, moving the servo.");
    tailServo.write(0);  // Move servo to position
  } else {
    Serial.println("Quiet detected, stopping the servo.");
    tailServo.write(90);  // Return servo to neutral position
  }

  // delay(50); // Small delay for stability
}

// Function to calculate the "volume" (approximates RMS)
int calculateVolume() {
  long sum = 0;
  const int numSamples = 50; // Number of samples to read
  for (int i = 0; i < numSamples; i++) {
    int sample = analogRead(micPin);        // Read raw microphone value
    int deviation = sample - 512;          // Center around 0 (assuming 512 is baseline)
    sum += (long)deviation * deviation;    // Square of deviation
  }
  return sqrt(sum / numSamples); // RMS value
}

