#include <Adafruit_NeoPixel.h>


#define LED_PIN 6
#define NUM_LEDS 291
#define SPEED 40

// not a const because we might want to manipulate this
int OFFSET = 96;

#define POT_PIN 2
int pot_val = 0;   // variable to store the value coming from the sensor
int pMin = 0;  //the lowest value that comes out of the potentiometer
int pMax = 1023; //the highest value that comes out of the potentiometer.

#define COLOR_POT_PIN 3
int color_pot_val = 0;   // variable to store the value coming from the sensor

// neopixel strip setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// one time function
void setup() {
  strip.begin();
  setAll(0,0,0);
  strip.show(); // Initialize all pixels txo 'off'
}

// run forever... forever...
void loop() {
  // red, green, blue, meteorSize, meteorTrailDecay, meteorRandomDecay, SpeedDelay
  twinkle(SPEED);
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

void fadeToBlack(int ledNo, byte fadeValue) {
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;
   
  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r=(r<=7)? 0 : (int) r-(r*fadeValue/256);
  g=(g<=7)? 0 : (int) g-(g*fadeValue/256);
  b=(b<=7)? 0 : (int) b-(b*fadeValue/256);
   
  strip.setPixelColor(ledNo, r,g,b);
}

void twinkle(int SpeedDelay) {  
    uint32_t color;
    uint16_t r;
    
    pot_val  = analogRead(POT_PIN);
    int scaled_pot = map(pot_val, pMin, pMax, 1, 99);
    int Decay = scaled_pot;

    color_pot_val  = analogRead(COLOR_POT_PIN);
    int color_scaled_pot = map(color_pot_val, pMin, pMax, 0, 256);
    color = ColorWheel(color_scaled_pot);
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!Decay) || (random(100) < Decay*1.3) ) {
        fadeToBlack(j, Decay);        
      }
    }
   
    // draw twinkly
    for(int j = 0; j < 5; j++) {
      r = random(0, NUM_LEDS);
      strip.setPixelColor(r, color);
    } 
    strip.show();
    delay(SpeedDelay);
}

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
