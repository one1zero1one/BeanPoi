#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 1

// When acceleration change goes beyond this threshold, the LED will blink.
#define THRESHOLD 100

AccelerationReading previousAccel;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // strips reset
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'  
  // Turn off the Bean's LED
  Bean.setLed(0, 0, 0);  
  // Initial reading  
  previousAccel = Bean.getAcceleration(); 
}

void loop() { 
  // Blink the LED green to see the frequency shit happens

    Bean.setLed(0, 255, 0);
    Bean.setLed(0, 0, 0); 
    
  // Get the current acceleration with a conversion of 3.91×10-3 g/unit every 200 ms
  AccelerationReading currentAccel = Bean.getAcceleration();   
  
  // Find the difference between the current acceleration and that of 200ms ago.
  int accelDifference = getAccelDifference(previousAccel, currentAccel); 
  // Update previousAccel for the next loop.   
  previousAccel = currentAccel;                                            
  
  // Check if the Bean has been moved beyond our threshold and sleep in total 200ms.
  if(accelDifference > THRESHOLD){   



//   colorWipe(strip.Color(255, 0, 0), 50); // Red
//   colorWipe(strip.Color(0, 255, 0), 50); // Green
//   colorWipe(strip.Color(0, 0, 255), 50); // Blue 
     colorWipeAccel(150); // Accel
 
   blackout(50);
    
  }else{
    Bean.sleep(200);
  }
}

// This function calculates the difference between two acceleration readings
int getAccelDifference(AccelerationReading readingOne, AccelerationReading readingTwo){
  int deltaX = abs(readingTwo.xAxis - readingOne.xAxis);
  int deltaY = abs(readingTwo.yAxis - readingOne.yAxis);
  int deltaZ = abs(readingTwo.zAxis - readingOne.zAxis);
  // Return the magnitude
  return deltaX + deltaY + deltaZ;   
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();    
  }
  delay(wait);
}

// Fill the dots one after the other with a color
void colorWipeAccel(uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {    
   // Get the current acceleration with range of ±2g, 
   // and a conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units. 
   AccelerationReading accel = Bean.getAcceleration();
    // Update LED color
    uint16_t r = (abs(accel.xAxis)) / 4;
    uint16_t g = (abs(accel.yAxis)) / 4;
    uint16_t b = (abs(accel.zAxis)) / 4;
    strip.setPixelColor(i, ((uint8_t)r,(uint8_t)g,(uint8_t)b));
  }
    strip.show();
    delay(wait);
}

 void blackout(uint8_t wait) {  
    strip.setPixelColor(0, 0, 0, 0);  
    strip.setPixelColor(1, 0, 0, 0);  
    strip.setPixelColor(2, 0, 0, 0);  
    strip.setPixelColor(3, 0, 0, 0);  
    strip.setPixelColor(4, 0, 0, 0);  
    strip.setPixelColor(5, 0, 0, 0);  
    strip.setPixelColor(6, 0, 0, 0);      
    strip.setPixelColor(7, 0, 0, 0);  
    strip.setPixelColor(8, 0, 0, 0);      
    strip.setBrightness(255);  
    strip.show();  
    delay(wait);
  }  
