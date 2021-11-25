#include "nimble.h"
#include <WS2812FX.h>

#define BLENAME "BLEDS"
#define LED_PIN        16
#define LED_COUNT      32
WS2812FX leds = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int _r = 255;
int _g = 0;
int _b = 0;
bool off = false;
std::string rxValue;

//https://stonez56.blogspot.com/2016/08/android-app-control-rgb-led-w-color.html

#include "bluefruit.h"

void commands(String rxString) {
    Serial.println(rxString);

    // Compatibility with Adafruit BlueFruit Connect (see bluefruit.h)
    bluefruit(rxString);

    /* INSERT Custom command here */
    if(rxString == "reset") {
      ESP.restart();
    }

    if(rxString == "name") {
      Serial.println(BLENAME);
    }

    if(rxString == "test") {
      leds.setMode(FX_MODE_STATIC);
      leds.setColor(255,255,255);
      Serial.println("BLEDS OK!");
    }

    if(rxString == "battery") {
      float voltage = analogRead(35) / 4096.0 * 7.46;
      if (voltage > 1 ) {
        uint8_t percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
        if (voltage >= 4.20) percentage = 100;
        if (voltage <= 3.50) percentage = 0;
        Serial.println(percentage);
      }
    }
}


void bluetooth_Connect() {
  leds.setMode(FX_MODE_STATIC);
  leds.setColor(RED);
}

void bluetooth_Disconnect() {
  ESP.restart();
}

void setup() {
  Serial.begin(115200);

  // LEDS init
  leds.init();
  leds.setBrightness(255);
  leds.setSpeed(400);
  leds.setMode(FX_MODE_RAINBOW_CYCLE);
  leds.start();

  // Bluetooth init
  BLEDevice::init(BLENAME);
  setupBLE();
}

void loop() {
  leds.service();
   while (Serial.available()) {
     String rxString = Serial.readStringUntil('\n');
     commands(rxString);
   }
}