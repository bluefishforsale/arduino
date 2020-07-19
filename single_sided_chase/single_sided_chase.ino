#include <Adafruit_NeoPixel.h>
#define LED_PIN 10
#define NUM_LEDS 291
#define NUM_SIDES 3
#define LED_PER_SIDE NUM_LEDS/NUM_SIDES
#define MS_PER_FRAME  33  // 30fps

//int OFFSET = NUM_LEDS/REP;
int REP = 3;
int OFFSET = 97;

#define COLOR_POT_PIN 14
#define DECAY_POT_PIN 15

int color_pot_val = 0;
int decay_pot_val = 0;

int scaled_color_val = 0;
int scaled_decay_val = 0;

int potMin = 0;    //the lowest value that comes out of the potentiometers
int potMax = 1023; //the highest value that comes out of the potentiometers

uint32_t color;  
uint8_t decay_trail;
uint32_t i, j, k = 0;    

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t black = strip.Color(0,0,0);
bool token = true;


void setup() { 
  strip.begin();
  strip.show();
}


void loop() {
  Serial.printf("testing %d\n", millis() );

  double previous = millis();
  while (true) {
    double current = millis();
    processInput();
    update();
    render();
    double elapsed = current - previous;
    previous = current;
    delay(MS_PER_FRAME - elapsed);
  }
}


void processInput() {
  decay_pot_val  = analogRead(DECAY_POT_PIN);
  color_pot_val  = analogRead(COLOR_POT_PIN);
  
  scaled_decay_val = map(decay_pot_val, potMin, potMax, 1, 99);
  decay_trail = scaled_decay_val;

  scaled_color_val = map(color_pot_val, potMin, potMax, 0, 256);
  color = ColorWheel(scaled_color_val);
}


void update() {

    if (token == true) {
      strip.setPixelColor((OFFSET +i) %NUM_LEDS, color );                 // forward
      i=(i+1) %NUM_LEDS;
      
    } else if (token == false) {
      strip.setPixelColor( (OFFSET +j)-(LED_PER_SIDE) %NUM_LEDS, black);   // erase 
      j=(j+1) %NUM_LEDS;
    }

    if (token == false && j >= i) {
      token = true;
    }

    if (token == true && isCorner(i, LED_PER_SIDE) )  {
      token = false;
    }

}


void render() {
  strip.show();
}


void fadeToBlack(int ledNo, byte fadeValue) {
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
 
  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r=(r<=7)? 0 : (int) r-(r*fadeValue/256);
  g=(g<=7)? 0 : (int) g-(g*fadeValue/256);
  b=(b<=7)? 0 : (int) b-(b*fadeValue/256);

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


bool isCorner(int x, int corner) {
  if (x  > 0 ) {              // can't pick both 0 and final corner
    if (x % corner == 0 ) {   // if the pixel mod corner is 0 then we're at a corner
      return true;
    }
  }
  return false;
}
