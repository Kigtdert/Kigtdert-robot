#include <Adafruit_NeoPixel.h>

#define PIN    13 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 2 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, pixels.Color(150, 150, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 150));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(500);

  
}
