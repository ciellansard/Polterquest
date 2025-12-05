#include <Boards.h>
#include <Firmata.h>
#include <FirmataConstants.h>
#include <FirmataDefines.h>
#include <FirmataMarshaller.h>
#include <FirmataParser.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

// Arduino pin numbers
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

const int buttonPin = 8;
int buttonState = 0;

void setup() {
  Serial.begin(9600);

  if (accel.begin())
  {
    Serial.println("None"); // This will print if no device is detected.
  }

  pinMode(buttonPin, INPUT);
}

void loop() {
  //analogRead(X_pin);
  //analogRead(Y_pin);

  Serial.print("Joystick: \n");
  Serial.print(analogRead(X_pin));
  Serial.print("\n");
  Serial.println(analogRead(Y_pin));
  Serial.print("\n\n");

  sensors_event_t event;
  accel.getEvent(&event);

  Serial.print("Accelerometer: \n");
  Serial.print(event.acceleration.x);
  Serial.print("\n");
  Serial.print(event.acceleration.y);
  Serial.print("\n");
  Serial.print(event.acceleration.z);
  Serial.print("\n\n");

  buttonState = digitalRead(buttonPin);

  Serial.print("Button: \n");
  if (buttonState == HIGH) {
    Serial.print("0");
  } 
  else {
    Serial.print("1");
  }
  Serial.print("\n\n");
  
  delay(1000);
  }