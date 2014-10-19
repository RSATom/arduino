#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <RTClib.h>

enum {
  PIN = 6,
  STRIP_TYPE = NEO_GRB + NEO_KHZ800,
};

enum {
  DELAY = 500,
  BRIGHTNESS = 5,
  COLOR_R = BRIGHTNESS,
  COLOR_G = BRIGHTNESS,
  COLOR_B = BRIGHTNESS,
};
  
enum {
  PIXELS_PER_SEGMENT = 2,
  SEGMENTS_PER_DIGIT = 7,
  PIXELS_PER_DIGIT = PIXELS_PER_SEGMENT * SEGMENTS_PER_DIGIT,
  PIXELS_PER_SECONDS_INDICATOR = 2,
};

//digit map:
//      1
//    0   2
//      3
//    4   6
//      5

const uint8_t digitsMasks[] = { 
    B11101110, //0
    B00100010, //1
    B01111100, //2
    B01110110, //3
    B10110010, //4
    B11010110, //5
    B11011110, //6
    B01100010, //7
    B11111110, //8
    B11110110, //9
  };

Adafruit_NeoPixel Strip = Adafruit_NeoPixel( PIXELS_PER_DIGIT * 4 + PIXELS_PER_SECONDS_INDICATOR, PIN, STRIP_TYPE );
const uint32_t PixelColor = Strip.Color( COLOR_R, COLOR_G, COLOR_B );
typedef RTC_DS1307 RTC;

void SkipPixels( uint8_t count, uint8_t* offset )
{
  for( byte p = 0; p < count; ++p ) {
    Strip.setPixelColor( *offset, 0 );
    ++(*offset);
  }
}

void PrintDigit( uint8_t digit, uint8_t* offset )
{
  uint8_t digitMask = digitsMasks[digit];
  
  for( byte s = 0; s < SEGMENTS_PER_DIGIT; ++s ) {
    bool show = digitMask & B10000000;
    for( byte p = 0; p < PIXELS_PER_SEGMENT; ++p ) {
      if( show )
         Strip.setPixelColor( *offset, PixelColor );
      else
         Strip.setPixelColor( *offset, 0 );
         
      ++(*offset);
    }
    digitMask <<= 1;
  }
}

void PrintSecondsIndicator( bool show, uint8_t* offset )
{
  for( byte p = 0; p < PIXELS_PER_SECONDS_INDICATOR; ++p ) {
    if( show )
       Strip.setPixelColor( *offset, PixelColor );
    else
       Strip.setPixelColor( *offset, 0 );
    ++(*offset);
  }
}

void setup() {
  Strip.begin();
  Wire.begin();
  RTC::begin();

  const DateTime buildTime( __DATE__, __TIME__ );
  if( buildTime.unixtime() > RTC::now().unixtime() )
    RTC::adjust( buildTime );
  
  Strip.show();
}

uint8_t lastSecond = 0;

void loop()
{
  DateTime now = RTC::now();
  
  uint8_t hour = now.hour();
  uint8_t minute = now.minute();
  uint8_t sec = now.second();

  uint8_t offset = 0;  

  PrintDigit( ( hour / 10 ) % 10, &offset );
  PrintDigit( hour % 10, &offset );

  PrintSecondsIndicator( sec != lastSecond, &offset ) ;

  PrintDigit( ( minute / 10 ) % 10, &offset );
  PrintDigit( minute % 10, &offset );

  Strip.show();

  lastSecond = sec;
  delay( DELAY );
}
