#include <Adafruit_NeoPixel.h>


#define LED_PIN 6
#define NUM_LEDS 291
#define BRIGHTNESS 100

#define OFFSET 0
#define OFFSET2 96
#define OFFSET3 192

#define REP 1
#define METEOR 1
#define TRAIL 50
#define DECAY 35
#define SPEED 25


#define POT_PIN 2
int pot_val = 0;   // variable to store the value coming from the sensor
int pMin = 0;  //the lowest value that comes out of the potentiometer
int pMax = 1023; //the highest value that comes out of the potentiometer.


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
  meteorRain(0xff, 0x33, 0x00, METEOR, TRAIL, true, SPEED);
}
 

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}


void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
 
  for(int i = 0; i < NUM_LEDS; i++) {
      
    pot_val  = analogRead(POT_PIN);
    int scaled_pot = map(pot_val, pMin, pMax, 1, 100);
    meteorTrailDecay = scaled_pot;
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(100)<meteorTrailDecay) ) {
        fadeToBlack(j, meteorTrailDecay*2 );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorTrailDecay; j++) {
      if( ( i-j <NUM_LEDS ) && (i-j>=0) ) {
        setPixel((OFFSET +i) %NUM_LEDS, red, green, blue );
        setPixel((NUM_LEDS-i+OFFSET) %NUM_LEDS, red, green, blue );
        
        setPixel((OFFSET2 +i) %NUM_LEDS, red, green, blue );
        setPixel((NUM_LEDS-i+OFFSET2) %NUM_LEDS, red, green, blue );
        
        setPixel((OFFSET3 +i) %NUM_LEDS, red, green, blue );
        setPixel((NUM_LEDS-i+OFFSET3) %NUM_LEDS, red, green, blue );

      }
    }
    
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}
