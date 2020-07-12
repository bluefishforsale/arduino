#include <Adafruit_NeoPixel.h>

#define PIN 6
#define SIZE 291
#define BRIGHTNESS 100
#define OFFSET 96
#define REP 1
#define SPEED 1

// neopixel strip setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(SIZE, PIN, NEO_GRB + NEO_KHZ800);

// one time function
void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS); //adjust brightness here
  strip.show(); // Initialize all pixels txo 'off'
}

// run forever... forever...
void loop() {
  MirrorRainbow();
}

// the rainbow equally distributed
void MirrorRainbow() {
  uint16_t i, j;
  uint16_t a_size = strip.numPixels();
  for(j=256; j >= 0; j--) {
    for(i=0; i <= a_size/2 ; i++) {
      strip.setPixelColor(i +OFFSET %a_size, ColorWheel( ((i *256 *REP /strip.numPixels()) +j *SPEED) &255) );
      strip.setPixelColor((a_size-i+OFFSET) %a_size, ColorWheel( ((i *256 *REP /strip.numPixels()) +j *SPEED) &255) );
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

// take a array if unsigned 32 bit ints and map them onto the pixel strip 1:1
void array_to_strip(uint32_t pixel_array[], uint16_t a_size, Adafruit_NeoPixel *strip) {
  uint16_t i;
  for (i=0; i < a_size; i++) {
    strip->setPixelColor(i, pixel_array[i]);
  }
}
