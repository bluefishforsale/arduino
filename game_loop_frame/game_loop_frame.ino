#include <Adafruit_NeoPixel.h>

#define LED_PIN 10
#define NUM_LEDS 291
#define NUM_SIDES 3
#define SIDE_LENGTH NUM_LEDS/NUM_SIDES
#define MS_PER_FRAME  33  // 30fps
#define REP 3
#define COLOR_POT_PIN 14
#define DECAY_POT_PIN 15

int OFFSET = NUM_LEDS/REP;

int color_pot_val = 0;
int decay_pot_val = 0;

int scaled_color_val = 0;
int scaled_decay_val = 0;

int potMin = 0;    //the lowest value that comes out of the potentiometers
int potMax = 1023; //the highest value that comes out of the potentiometers

uint32_t color;  
uint8_t decay_trail;
uint32_t h, i, j, k = 0;    


uint8_t sin_lut[256];


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() { 
  for (i=0; i < sizeof(sin_lut); i++){
    sin_lut[i] = (uint8_t) 256 * sinf(TWO_PI * i / sizeof(sin_lut) );
  }
  Serial.begin(9600);
  strip.begin();
  strip.show();
}


void loop() {
  double previous = millis();
  while (true) {
    double current = millis();
    processInput();

///// Effects
//    trails();
//    twinkle();
//    fadingTrails();
    rainbow();

    render();
    double elapsed = current - previous;
    previous = current;
    delay(MS_PER_FRAME - elapsed);
  }
}



/////////////////////////  Effects functions

void trails() {
  // fade brightness all LEDs one step
  for(int j=0; j<NUM_LEDS; j++) {
    if( random(0,100) < (decay_trail/random(1,10)) ) {
      fadeToBlack(j, decay_trail*2 );        
    }
  }
   
  // draw meteor
  for(int j = 0; j < decay_trail; j++) {
    for (int k=1; k <=REP; k++) { // generate most bright *spark*
      strip.setPixelColor((OFFSET *k +i) %NUM_LEDS, color );            // forward 
      strip.setPixelColor((NUM_LEDS -i +(OFFSET*k)) %NUM_LEDS, color ); // reverse
    }
  }
  // increment pixel and loop back around
  i=(i+1) %NUM_LEDS;
}



void twinkle() {
  // fade brightness all LEDs by decay amount
  for (int j=0; j < NUM_LEDS; j++) {
      if( random(0,100) < decay_trail) {
//        fadeToBlack(j, map(decay_trail, 0, SIDE_LENGTH, 128, 1) );  // we need to sample the pixel color here
        fadeToBlack(j, decay_trail );  // we need to sample the pixel color here
      }
  }

  // draw j random bright spots
  for ( int j=0; j < decay_trail/3; j++) {
    if( random(0,100) > decay_trail) {
      i = random(0,NUM_LEDS);
      strip.setPixelColor(i, fadeColor(color, map(decay_trail, 0, SIDE_LENGTH, 255, 128) ));  // no need to sample color, just reduce brightness proportional to how many we show
//      strip.setPixelColor(i, fadeColor(color, decay_trail) );  // no need to sample color, just reduce brightness proportional to how many we show
    }
  }
}



void fadingTrails() {
 
  // fade brightness all LEDs one step
  for(int j=0; j<NUM_LEDS; j++) {
    if( random(0,100) < (decay_trail/random(1,10)) ) {
      fadeToBlack(j, decay_trail*2 );        
    }
  }
   
  // draw pixel
  for(int j = 0; j < decay_trail; j++) {
    for (int k=1; k <= REP; k++) { // generate most bright *spark*

      color = fadeColor(color, i  % SIDE_LENGTH);
      strip.setPixelColor((OFFSET *k +i) %NUM_LEDS, color );            // forward 

      color = fadeColor(color, i % SIDE_LENGTH );
      strip.setPixelColor((NUM_LEDS -i +(OFFSET*k)) %NUM_LEDS, color ); // reverse
    }
  }
  // increment pixel and loop back around
  i=(i+1) %NUM_LEDS;
}


void rainbow() {
  for(j=0; j < NUM_LEDS; j++) {
     color = ColorWheel( (j *256 / NUM_LEDS)+i %255 );
     strip.setPixelColor(j, color);
  }
    
  i=(i+(map(decay_trail, 1, 99, -3, 3))) %256;
}


/////////////////////////  common utility functions

void render() {
  strip.show();
}


void processInput() {
  decay_pot_val  = analogRead(DECAY_POT_PIN);
  color_pot_val  = analogRead(COLOR_POT_PIN);
  
  scaled_decay_val = map(decay_pot_val, potMin, potMax, 1, 99);
  decay_trail = scaled_decay_val;

  scaled_color_val = map(color_pot_val, potMin, potMax, 1, 255);
  color = ColorWheel( scaled_color_val );

//  for (i=0; i < sizeof(sin_lut); i++){
//    Serial.printf("%d, ", sin_lut[i]);
//  }
//  Serial.println();

}


uint32_t fadeColor(uint32_t oldColor, byte fadeValue) {
  uint8_t r, g, b;
 
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
  g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
  b=(b<=10)? 0 : (int) b-(b*fadeValue/256);

  return strip.Color(r,g,b);
}


void fadeToBlack(int ledNo, byte fadeValue) {
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
 
  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
  g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
  b=(b<=10)? 0 : (int) b-(b*fadeValue/256);

  strip.setPixelColor(ledNo, r,g,b);
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
