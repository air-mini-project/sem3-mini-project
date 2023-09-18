#include <Arduino.h>

#include <HCSR04.h>

UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
}

void loop () {
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    double distance = distanceSensor.measureDistanceCm();

    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(F("cm"));
    delay(1000);
}