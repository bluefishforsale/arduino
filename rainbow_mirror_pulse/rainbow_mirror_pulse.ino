#include <Adafruit_NeoPixel.h>

#define PIN 6
#define SIZE 291
#define BRIGHTNESS 100
#define OFFSET 47
#define REP 1
#define SPEED 1

// setup of the neopixel strip object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(SIZE, PIN, NEO_GRB + NEO_KHZ800);

// one time function
void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS); //adjust brightness here
  strip.show(); // Initialize all pixels txo 'off'
}

// run till stop
void loop() {
  MirrorRainbow();
}

// the rainbow equally distributed
void MirrorRainbow() {
  uint16_t i, j;
  for(j=0; j < 256; j++) {
    for(i=0; i <= strip.numPixels()/2 ; i++) {
      strip.setPixelColor(i, ColorWheel( ((i *256 *REP /strip.numPixels()) +j *SPEED) &255) );
      strip.setPixelColor(strip.numPixels()-i, ColorWheel( ((i *256 *REP /strip.numPixels()) +j *SPEED) &255) );
    }
    strip.show();
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ColorWheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
