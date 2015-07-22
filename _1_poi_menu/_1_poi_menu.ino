/* this is a self made poi using
- light blue bean x2
- sparkfun powercell (with a little solder touch to output 3.3V
- adafruit 8 pixelstick (because I couldn't find any proper led strip)

- serial commands

- color fx

- detection of movement

*/

// where is the neopixel stick connected  (mirrored)
#define PIN 1
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

// When acceleration is below this threshold, we consider it moving
#define THRESHOLD 100
AccelerationReading previousAccel;
int fallDuration = 0;
int fallCount = 0;

#define MAX_STRING_LEN 27

// this holds all serial input until a full command is in it
String cmdBuffer;

// for the blink command
bool doBlink = false;
bool blinkState = true;
int blinkCnt = 0;
LedReading oldLed;

void setup() {

  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);

  // on readBytes, return after 25ms or when the buffer is full
  Serial.setTimeout(25);

  strip.begin();
  strip.setBrightness(10);

  brutal();

  // Initial reading
  previousAccel = Bean.getAcceleration();
}

void loop() {

  // this is the short-term buffer that gets added to the cmdBuffer
  char buffer[64];
  size_t length = 64;

  // read as much as is available
  length = Serial.readBytes( buffer, length - 1 );

  // null-terminate the data so it acts like a string
  buffer[length] = 0;

  // if we have data, so do something with it
  if ( length > 0 )
  {
    // stick it to the end of the main buffer
    cmdBuffer = buffer;

    // find the end of the command input (a new line character)
    size_t lineEnd = cmdBuffer.indexOf( "\n" );

    // if there IS a new line character, then...
    if ( lineEnd > 0 )
    {
      // copy off the command, reusing our buffer variable
      cmdBuffer.substring( 0, lineEnd ).toCharArray( buffer, 64 );

      // and remove it from the command buffer
      cmdBuffer = cmdBuffer.substring( lineEnd + 1, cmdBuffer.length() + 1 );

      // -------------------------------------------
      // now we can do something with the command...


      // hello ... world
      if ( !strncmp( buffer, "hello", 5 ) )
      {
        Serial.println( "< world >" );
      }

      // request temperature data
      else if ( !strncmp( buffer, "temp", 4 ) )
      {
        int8_t newTemp = Bean.getTemperature();
        Serial.println( String("< Temperature: ") + String(newTemp) + "c >" );
      }

      // request accelerometer data
      else if ( !strncmp( buffer, "acc", 3 ) )
      {
        int8_t x = Bean.getAccelerationX();
        int8_t y = Bean.getAccelerationY();
        int8_t z = Bean.getAccelerationZ();
        Serial.println( String("< Accelerometer: ") + String(x) + "," + String(y) + "," + String(z) + " >" );
      }

      // blink
      else if ( !strncmp( buffer, "blink", 5 ) )
      {
        doBlink = !doBlink;
        if ( doBlink && ( Bean.getLedRed() + Bean.getLedGreen() + Bean.getLedBlue() == 0 ) )
          Bean.setLed( 255, 255, 255 );
        else if ( !doBlink )
          Bean.setLed( 0, 0, 0 );
        oldLed = Bean.getLed();
        Serial.println( String("< Blink ") + (doBlink ? "ON" : "OFF") + " >" );
      }

      // everything else, just echo it
      else if ( !strncmp( buffer, "1" , 1) )
      {
        Serial.println ( String("< rainbow >") );
        rainbow(20);
      }
      else if ( !strncmp( buffer, "2" , 1) )
      {
        Serial.println ( String("< rainbowCycle >") );
        rainbowCycle(20);
      }
      else if ( !strncmp( buffer, "3" , 1) )
      {
        Serial.println ( String("< theaterChaseRainbow >") );
        theaterChaseRainbow(50);
      }
      else if ( !strncmp( buffer, "4" , 1) )
      {
        Serial.println ( String("< theaterChase >") );
        theaterChase(strip.Color(127, 127, 127), 50); // White
        theaterChase(strip.Color(127, 0, 0), 50); // Red
        theaterChase(strip.Color(0, 0, 127), 50); // Blue

      }
      else
      {
        Serial.println( String("< ") + buffer + " >" );
      }
    }
  }
  // buffer work done

  // Get the current acceleration with a conversion of 3.91×10-3 g/unit.
  AccelerationReading currentAccel = Bean.getAcceleration();

  // Find the difference between the current acceleration and that of 20ms ago. ????????
  int accelDifference = getAccelDifference(previousAccel, currentAccel);
  // Update previousAccel for the next loop.
  previousAccel = currentAccel;

  // brightness based on acceleration, faster = brighter
  int y = map(accelDifference, 1, 3000, 1, 100);
  strip.setBrightness(y);

  // cycle wait time, slow when slow fast when fast.
  int z = map(accelDifference, 1, 1600, 10, 1);
  rainbowCycle(1); // zx8x256 = 2048ms?

  Serial.println( String("< ") + accelDifference +  " y " + y + " z " + z + " >" );

}// end of main loop.

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// This function calculates the difference between two acceleration readings
int getAccelDifference(AccelerationReading readingOne, AccelerationReading readingTwo) {
  int deltaX = abs(readingTwo.xAxis - readingOne.xAxis);
  int deltaY = abs(readingTwo.yAxis - readingOne.yAxis);
  int deltaZ = abs(readingTwo.zAxis - readingOne.zAxis);
  // Return the magnitude
  return deltaX + deltaY + deltaZ;
}


// COLOR FX
// this brutally initiates the strips by flashing each strip with r, g, b.
void brutal() {
  colorWipe(strip.Color(255, 0, 0), 5); // Red
  strip.show();
  colorWipe(strip.Color(0, 255, 0), 5); // Green
  strip.show();
  colorWipe(strip.Color(0, 0, 255), 5); // Blue
  strip.show();
  colorWipe(strip.Color(255, 555, 255), 5); // White
  strip.show();
  colorWipe(strip.Color(0, 0, 0), 0); // CLEAR
  strip.show();
}

// 256 x levels for each pixel to do RGB.
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels())) & 255));
    }
    strip.show();
    delay(wait);
}

//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for (j = 0; j < 256; j++) {
//    for (i = 0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


// some C magic

char* subStr (char* input_string, char *separator, int segment_number) {
  char *act, *sub, *ptr;
  static char copy[MAX_STRING_LEN];
  int i;
  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL) {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}
