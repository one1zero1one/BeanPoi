#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 1


Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // strips reset
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'  
  // Turn off the Bean's LED
  Bean.setLed(0, 0, 0);  
}

void loop() { 
  // Blink the LED green to see the frequency shit happens
    Bean.setLed(0, 255, 0);
    Bean.setLed(0, 0, 0); 
    

   colorWipe(strip.Color(255, 0, 0), 500); // Red
   colorWipe(strip.Color(0, 255, 0), 500); // Green
   colorWipe(strip.Color(0, 0, 255), 500); // Blue 

 
   blackout(5000);
    
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();    
  }
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
