#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define RING       D8
#define NUMPIXELS  299
#define FLIESPIN   D3
#define NUMFLIES   5

#define PUSH D7
int generalCounter = 0;

int MIN_TOUNGE = 233;
int MAX_TOUNGE = 244;
int tounge_counter = MIN_TOUNGE;
boolean tounge_active = true;
boolean tounge_reached = false;

int MIN_WATER = 0;
int MAX_WATER = 50;

int MIN_FLOWER = 50;
int MAX_FLOWER = 100;
int flowercounter = 0;

int MIN_FLOWER2 = 128;
int MAX_FLOWER2 = 150;

int MIN_FROG = 184;
int MAX_FROG = 202;

int FIREFLY_NUMBER = 5;
int SPECIAL_FIREFLY = 0;

boolean flyappear = false;
int flycounter = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, RING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel flies(NUMFLIES, FLIESPIN, NEO_GRB + NEO_KHZ800);

struct RGB {
  unsigned long r;
  unsigned long g;
  unsigned long b;
};

int NUMBER_OF_WATERCOLORS = 3;

RGB BLUE_1 = {0, 0, 255};
RGB BLUE_2 = {0,50, 250};
RGB BLUE_3 = {30,144,255};
//RGB BLUE_4 = {135,206,250};

RGB PINK = {255,20,147};
RGB WHITE = {255, 255, 255};
RGB RED = {255, 0, 0};
RGB GREEN = {154,205,50};
RGB YELLOW = {200, 200, 10};
RGB BLACK = {0,0,0};

RGB watercolors[] = {BLUE_1, BLUE_2, BLUE_3};

void setup() {
  pixels.begin();
  pixels.setBrightness(250);
  flies.begin();
  flies.setPixelColor(0, 0, 0, 0);
  flies.show();
  pinMode(PUSH, INPUT_PULLUP);
  Serial.begin(115200);
}

// void prolongTongue() {
//   counter++;
//   pixels.setPixelColor(counter, 255, 0, 0);
//   pixels.show();
//   if(counter == MAXTOUNGE) {
//     flycounter = 0;
//     flyappear = false;
//     flies.setPixelColor(0, 0, 0, 0);
//     flies.show();
//   }
// }

// void shorterTongue() {
//   pixels.setPixelColor(counter, 0, 0, 0);
//   pixels.show();
//   counter--;
// }

void water() {
  if(generalCounter%20==0) {
    for(int i= MIN_WATER; i< MAX_WATER; i++){
      int x = random(NUMBER_OF_WATERCOLORS);
      Serial.println(x);
      RGB c = watercolors[x];
      pixels.setPixelColor(i, c.r, c.g, c.b);
    }
    pixels.show();
  }
}

void flower() {
  if(generalCounter%20==0) {
    if(MIN_FLOWER + flowercounter > MAX_FLOWER -10) {
      flowercounter = 0;
    }
    for(int i= MIN_FLOWER; i< MAX_FLOWER; i++){
      RGB c = PINK;
      if(i >= MIN_FLOWER + flowercounter && i < MIN_FLOWER+flowercounter+10) {
        pixels.setPixelColor(i, WHITE.r, WHITE.g, WHITE.b);
      } else {
        pixels.setPixelColor(i, c.r, c.g, c.b);
      }
    }
    flowercounter++;
    pixels.show();
  }
}

void flower2() {
    for(int i= MIN_FLOWER2; i< MAX_FLOWER2; i++){
      RGB c = PINK;
      pixels.setPixelColor(i, c.r, c.g, c.b);
    }
    pixels.show();
}

void frog() {
  for(int i= MIN_FROG; i< MAX_FROG; i++){
    RGB c = GREEN;
    pixels.setPixelColor(i, c.r, c.g, c.b);
  }
  pixels.show();
}

void zunge() {
  if(tounge_active) {
    tounge_counter++;
    if(tounge_counter == MAX_TOUNGE) {
      tounge_active = false;
      flies.setPixelColor(SPECIAL_FIREFLY, BLACK.r, BLACK.g, BLACK.b);
      flies.show();
    }
  } else {
    if(tounge_counter > MIN_TOUNGE) {
     tounge_counter--;
   }
  }
  for(int i=MIN_TOUNGE; i<tounge_counter; i++) {
    RGB c = RED;
    pixels.setPixelColor(i, c.r, c.g, c.b);
  }
  for(int i=tounge_counter; i<MAX_TOUNGE; i++) {
    RGB c = BLACK;
    pixels.setPixelColor(i, c.r, c.g, c.b);
  }
  pixels.show();
}

void firefly(int ff_number) {
  for(int i= 0; i< FIREFLY_NUMBER; i++){
    if(i == ff_number) {
      if(!tounge_reached) {
        flies.setPixelColor(i, YELLOW.r, YELLOW.g, YELLOW.b);
      } else {
        flies.setPixelColor(i, BLACK.r, BLACK.g, BLACK.b);
      }
    } else {
      flies.setPixelColor(i, BLACK.r, BLACK.g, BLACK.b);
    }
  }
  flies.show();
}

void activateTounge() {
  delay(1000);
  tounge_active = true;
}

void loop() {
    generalCounter++;
    delay(2);
    water();
    flower();
    flower2();
    frog();
    zunge();
    if(generalCounter%150 == 0) {
          int fireflyToLight = random(FIREFLY_NUMBER+1);
          if(fireflyToLight >= FIREFLY_NUMBER) {
            fireflyToLight = SPECIAL_FIREFLY;
          }
          firefly(fireflyToLight);
          if(fireflyToLight == SPECIAL_FIREFLY) {
            activateTounge();
          }
    }
}
