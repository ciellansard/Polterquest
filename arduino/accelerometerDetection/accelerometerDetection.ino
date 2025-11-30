// Code provided by Pi My Life Up on YouTube. https://www.youtube.com/watch?v=vOZEwe_o20Y

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (accel.begin())
  {
    Serial.println("None");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t event;
  accel.getEvent(&event);

  Serial.println();
  Serial.print("X: ");
  Serial.print(event.acceleration.x);
  Serial.print("    Y: ");
  Serial.print(event.acceleration.y);
  Serial.print("    Z: ");
  Serial.print(event.acceleration.z);
  delay(500);
}
