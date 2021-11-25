void bluefruit(String rxString) {
    String command = rxString.substring(0,2);
    if(command == "!C") {
        uint8_t r = (uint8_t)rxString[2];
        uint8_t g = (uint8_t)rxString[3];
        uint8_t b = (uint8_t)rxString[4];

        Serial.printf("R: %d | G: %d | B: %d \n\r",r,g,b);

        for(int i=0;i<LED_COUNT;i++) {
            leds.setColor(leds.Color(r,g,b));
        }

        //update AnimateRing
        _r = g; _g = r; _b = b;
    }

    if(command == "!B") {
        String button = rxString.substring(2);
        Serial.print("button:");
        Serial.println(button);
        if(off == true) {
            leds.setColor(_r,_g,_b);
            off = false;
        }
        // UP
        if(button == "516"){
            if(leds.getBrightness() < 255) {
            leds.setBrightness(leds.getBrightness() + 5);
            Serial.println(leds.getBrightness());
            }
        }

        // DOWN
        if(button == "615") {
            if(leds.getBrightness() > 0) {
            leds.setBrightness(leds.getBrightness() - 5);
            Serial.println(leds.getBrightness());
            }
        }

        // LEFT
        if(button == "804") {
            if(leds.getMode() < 56) {
            leds.setMode((leds.getMode() + 1));
            leds.setColor(_r,_g,_b);
            Serial.println(leds.getMode());
            }
        }
        // RIGHT
        if(button == "705") {
            if(leds.getMode() > 0) {
            leds.setMode((leds.getMode() -1));
            leds.setColor(_r,_g,_b);
            Serial.println(leds.getMode());
            }
        }

        // 1
        if(button == "10;") {
            leds.setMode(FX_MODE_STATIC);
            leds.setColor(BLACK);
            off = true;
        }

        // 2
        if(button == "20:") {
            leds.setMode(FX_MODE_BLINK);
            leds.setSpeed(100);
        }

        // 3
        if(button == "309") {
            leds.setMode(FX_MODE_BLINK);
            leds.setSpeed(1000);
        }

        // 4
        if(button == "408") {
            leds.setMode(FX_MODE_RAINBOW);
            leds.setSpeed(100);
        }
    }
}