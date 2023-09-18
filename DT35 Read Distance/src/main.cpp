#include <Arduino.h>
int readPin = 25;
float scale = 0.0f;
float minimum = 15.0f;
float maximum = 60.0f;
float adc_max = 4096.0f;
double distance = 0;

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
}

void loop () {
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    float x = analogRead(readPin);
    scale = (float)(((maximum - minimum) / adc_max));
    distance = (float) scale * x + minimum;

    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(F("cm"));
    // Serial.println(x);
    delay(1000);
}