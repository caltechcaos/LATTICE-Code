/**
 *  Sets the Satellite Receiver into Binding Mode with an ESP32:
 *  - Please use a Microcontroller which uses a 3.3V logic.
 *  - We power the Satellite via a digital GPIO Pin (e.g 23)
 *  - We use Serial2 to connect the Data line of the Satellite. Connect the RX pin of the Microcontroller (GPIO 23)
 *  - We use Serial for logging
 *
 *  The receiver is set into bind mode if it receives a defined number of falling signals right
 *  after the power up:  This is the reason why we need to power the receiver via a GPIO pin!
 *
 */

#include <Arduino.h>

#include "SpektrumCSV.h"
#include "SpektrumSatellite.h"

#define ONBOARD_LED 13

SpektrumSatellite<uint16_t> satellite(Serial1);
SpektrumCSV<uint16_t> csv(',', 0);
int rxPin = 19;
int powerPin = 2;

// blinking the LED
bool led_state = true;
int led_interval;
long led_timeout;

// print buffer for csv
uint8_t buffer[1024];

void setup() {
    // Setup logging
    Serial.begin(115200);
    Serial.println();
    Serial.println("setup");
    satellite.setLog(Serial);

    pinMode(ONBOARD_LED, OUTPUT);
    digitalWrite(ONBOARD_LED, HIGH);

    // we set the receiver into binding mode
    satellite.setBindingMode(Internal_DSMx_22ms);
    satellite.startBinding(powerPin, rxPin);

    // start blinking
    led_interval = 1000;

    // switch rxPin to receive data on Serial
    Serial1.begin(115200);
}

void loop() {
    // when we receive data we
    if (satellite.getFrame()) {
        led_interval = 0;
        led_state = false;

        // log the data as CSV
        csv.toString(satellite, buffer, 1024);
        Serial.print((char*)buffer);
    }

    // uint8_t str[] = {1, 2, 3};
    // satellite.sendData(str);
    // if (Serial1.available() > 0) {
    //   Serial.println(Serial1.read());
    // }

    // update led
    if (millis() > led_timeout) {
        if (led_interval > 0)
            led_state = !led_state;
        led_timeout = millis() + led_interval;
        digitalWrite(ONBOARD_LED, led_state);
    }
}