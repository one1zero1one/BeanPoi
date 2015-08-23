/* this is a self made poi using
- light blue bean x2
- sparkfun powercell (with a little solder touch to output 3.3V)
- adafruit 8 pixelstick (because I couldn't find any proper led strip)

v1 when shanken changes program
- brigtness via serial

*/

// where is the neopixel stick connected  (mirrored)
#define PIN 1
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(1, 8, PIN,  NEO_MATRIX_BOTTOM, NEO_GRB + NEO_KHZ400);

// When acceleration is below this threshold, we consider shake happens
#define THRESHOLD 700
AccelerationReading previousAccel;

#define MAX_STRING_LEN 27 //??

// this holds all serial input until a full command is in it
String cmdBuffer;

// integer used to count sequence in loop
int j;

// integer used to keep brightness
int keepBrightness = 0;

// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int type = 1;                   // menu state

//timer0
elapsedMillis timer0;
#define interval 1000
// the interval in mS

int dynInterval = 1000;

long randNumber;
long time = 0;

int periode = 5000;

void setup() {

  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);

  // on readBytes, return after 25ms or when the buffer is full
  Serial.setTimeout(25);

  // begin strip
  strip.begin();
  strip.setBrightness(keepBrightness);

  //do startup check instead of brutal
  brutal();

  // Initial reading
  previousAccel = Bean.getAcceleration();

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;

  timer0 = 0; // clear the timer at the end of startup

  randomSeed(analogRead(0));

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

      if ( !strncmp( buffer, "0" , 1) )
      {
        Serial.println ( String("< Brightness 0 >") );

        keepBrightness = 0;
      }
      if ( !strncmp( buffer, "1" , 1) )
      {
        Serial.println ( String("< Brightness 5 >") );
        keepBrightness = 100;
      }
      else if ( !strncmp( buffer, "debug" , 1) )
      {
        Serial.println( String(" type ") + type);
        Serial.println( String(" average ") + average);
        Serial.println( String(" keepBrightness ") + keepBrightness);
        Serial.println( String(" dynInterval ") + dynInterval);
      }
      else // everything else, just echo it
      {
        Serial.println( String("< ") + buffer + " >" );

      }
    }
    dynInterval = 1000; //give it a second until type detection in the attempt to avoid type change on serial comm
  }
  // buffer work done

  // Get the current acceleration with a conversion of 3.91×10-3 g/unit.
  AccelerationReading currentAccel = Bean.getAcceleration();

  // Find the difference between the current acceleration and that of 20ms ago. ????????
  int accelDifference = getAccelDifference(previousAccel, currentAccel);

  // Update previousAccel for the next loop.
  previousAccel = currentAccel;

  // subtract the last reading:
  total = total - readings[index];
  // read from the sensor:
  readings[index] = accelDifference;
  // add the reading to the total:
  total = total + readings[index];
  // advance to the next position in the array:
  index = index + 1;
  // if we're at the end of the array...
  if (index >= numReadings)   {
    // ...wrap around to the beginning:
    index = 0;
    // calculate the average:
    average = total / numReadings;
    //every send it to the computer as ASCII digits
    //Serial.println( String("< (1-3000) ") + average); after a while, it crashes the device. Maybe display some status once a minute?
  }

  // once a second logic to change the type
  if (timer0 > dynInterval) {
    timer0 -= dynInterval;
    dynInterval = 100;
    // if shaken moving through the menus else stop reset
    if (average > THRESHOLD) {
      type++;
      dynInterval = 2000;
      if (type == 6) {
        type = 1;
      }
      Serial.println( String(" type changed to ") + type);
    }
  }

  if (type == 1) { // blank
    keepBrightness = 0; // shutup.
    strip.setBrightness(keepBrightness);
    strip.show();
  }
  if (type == 2) { // all pixels are random
    keepBrightness = 5;
    strip.setBrightness(keepBrightness);
    for (int i = 0; i < strip.numPixels(); i++) {
      randNumber = random(0, 255);
      strip.setPixelColor(i, Wheel(randNumber));
      strip.show();
    }
  }
  if (type == 3) { // all pixels are cosine with time displacement
    strip.setBrightness(keepBrightness);
    for (int i = 0; i < strip.numPixels(); i++) { // each pixel
      int time = millis();
      int value = 128 + 127 * cos(2 * PI / 5000 * (255 * i - time)); // get a value between 0 to 255 according to cosinus, with a time displacement
      strip.setPixelColor(i, Wheel(value));
      strip.show();
    }
  }

  if (type == 4) { // cosine with larger timespan 
    strip.setBrightness(keepBrightness);
    for (int i = 0; i < strip.numPixels(); i++) { // each pixel
        int time = millis();
        int value = 128 + 127 * cos(2 * PI / 10000 * (255 * i - time)); // get a value between 0 to 255 according to cosinus, with a time displacement
        strip.setPixelColor(i, Wheel(value));

        int value2 = 4 + 4 * cos(2 * PI / 5000 * (255 * i - time));
        strip.setPixelColor(value2, 0);
        strip.show();

    }
  }


  if (type == 5)  {
    // the lousy one pixel moves every 2 seconds while the rest loop rgb.
    if (j < 256) {
      for (int i = 0; i < strip.numPixels(); i++) {
        int pixel = map(j, 1 , 256, 1, 8);
        //   strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        strip.setPixelColor(i, Wheel(j));
        strip.setPixelColor(pixel, Wheel(256 - j) );
      }
      j++;
      strip.show();
    }
    else {
      j = 0;
      strip.show();
    }
  }

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
  delay(200);
  colorWipe(strip.Color(0, 255, 0), 5); // Green
  strip.show();
  delay(200);
  colorWipe(strip.Color(0, 0, 255), 5); // Blue
  strip.show();
  delay(200);
  colorWipe(strip.Color(255, 555, 255), 5); // White
  strip.show();
  delay(200);
  colorWipe(strip.Color(127, 127, 127), 127); // mid
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
  strip.setBrightness(10);
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
