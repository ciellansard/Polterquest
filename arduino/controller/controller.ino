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
  /*
  Serial.print("Joystick X-axis: ");
  Serial.print(analogRead(X_pin));
  Serial.print("\n");
  Serial.print("Joystick Y-axis: ");
  Serial.println(analogRead(Y_pin));
  Serial.print("\n\n");

*/
/*
  sensors_event_t event;
  accel.getEvent(&event);

  Serial.println();
  Serial.print("Acceleromter X: ");
  Serial.print(event.acceleration.x);
  Serial.print("Accelerometer Y: ");
  Serial.print(event.acceleration.y);
  Serial.print("Accelerometer Z: ");
  Serial.print(event.acceleration.z);
  Serial.print("\n\n");
*/
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    Serial.print("\n");
    Serial.print("button off");
  } 
  else {
    Serial.print("button on");
    Serial.print("\n");
  }
  
  delay(1000);
  }