#include <Adafruit_NeoPixel.h>

enum {
  PIN = 6,
  STRIPE_TYPE = NEO_GRB + NEO_KHZ800,
};

enum {
  DELAY = 500,
  BRIGHTNESS = 5,
  COLOR_R = 0,
  COLOR_G = BRIGHTNESS,
  COLOR_B = 0,
};

enum {
  PIXELS_PER_SEGMENT = 2,
  SEGMENTS_PER_DIGIT = 7,
  PIXELS_PER_DIGIT = PIXELS_PER_SEGMENT * SEGMENTS_PER_DIGIT,
};

//digit map:
//      5
//    4   6
//      3
//    0   2
//      1

const uint8_t digits[] = { 
    B11101110,
    B00100010,
    B11010110,
    B01110110,
    B00111010,
    B01111100,
    B11111100,
    B00100110,
    B11111110,
    B01111110
  };

Adafruit_NeoPixel Stripe = Adafruit_NeoPixel( PIXELS_PER_DIGIT, PIN, STRIPE_TYPE );
const uint32_t PixelColor = Stripe.Color( COLOR_R, COLOR_G, COLOR_B );

void PrintDigit( uint8_t digit )
{
  for( byte s = 0; s < SEGMENTS_PER_DIGIT; ++s ) {
    bool show = digit & B10000000;
    for( byte p = 0; p < PIXELS_PER_SEGMENT; ++p ) {
      unsigned pixel = s * PIXELS_PER_SEGMENT + p;
      if( show )
         Stripe.setPixelColor( pixel, PixelColor );
      else
         Stripe.setPixelColor( pixel, 0 );
    }
    digit <<= 1;
  }
}

void setup() {
  Stripe.begin();
  Stripe.show();
}

void loop() {
  for( unsigned i = 0; i < sizeof( digits ) / sizeof( digits[0] ); ++i ) {
    PrintDigit( digits[i] );
    Stripe.show();
    delay( DELAY );
  }
}
