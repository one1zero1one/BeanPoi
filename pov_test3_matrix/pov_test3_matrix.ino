// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy  replace,
#endif

#define PIN 1

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(1, 8, PIN,  NEO_MATRIX_BOTTOM, NEO_GRB + NEO_KHZ400);

//Color definitions
const uint32_t colors[] = {
  matrix.Color(252, 0, 0), matrix.Color(0, 252, 0), matrix.Color(0, 0, 252) };//Starting color
  
const uint32_t colorfade[] = {
  matrix.Color(36, 0, 0), matrix.Color(0, 36,0), matrix.Color(0, 0, 36) };//fade color
  

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.setTextColor(colors[0]);
  matrix.setTextSize(1);
  
}
int fade;
int randomNumber1; //rain graphic random numbers
int randomColor1; //rain graphic random numbers
int randomColor2; //rain graphic random numbers
int randomDelay; //rain graphic random numbers
int x   = matrix.width(); //matrix dimensions
int y = matrix.height();//matrix dimensions
int pass = 0;
int i =0;   // counter for graphics
int j = 0 ; //internal graphics counter

void loop() { // call graphic functions
				
  Serial.begin(9600);

	/*for (i = 0 ; i<2; i++){
		rainDrop();
	}
	*/
	
	textGraphic();
	

}


		void rainDrop (){ //rain graphic function
	
  					
				  	Serial.begin(9600);
				  	
				  	int randomColor;
				
					j=0;
					matrix.fillScreen(0); 
					randomSeed(i);
					randomNumber1 = random(10);
					int randomNumber2 = random(10);
					randomColor1 = random(0,2);
					randomColor2 = random(0,2);
					randomDelay = random(30,70);
				  	
				  
				  	for (int j = 0;j<=17;j++){
				  		
				  		delay (randomDelay);
				  		matrix.drawPixel(randomNumber1,j,colors[randomColor1]);
				  		fade = colors[randomColor]-colorfade[randomColor1];
						delay (randomDelay);
						
				  		matrix.drawPixel(randomNumber1,j-1,fade); 
				  		fade = fade-colorfade[randomColor1];
				  		delay (randomDelay);
				  	
				  		matrix.drawPixel(randomNumber1,j-2,fade); 
				  		fade = fade-colorfade[randomColor1];
				  		delay (randomDelay);
				  		
				  		matrix.drawPixel(randomNumber1,j-3,fade); 
				  		fade = fade-colorfade[randomColor1];
				  		delay (randomDelay);
				  	
				  		matrix.drawPixel(randomNumber1,j-4,fade); 
						fade = fade-colorfade[randomColor1];
						delay (randomDelay);
						
				  		matrix.drawPixel(randomNumber1,j-5,fade); 
				  		fade = fade-colorfade[randomColor1];
				  		delay (randomDelay);
				  		
				  		matrix.drawPixel(randomNumber1,j-6,fade);
				  		delay (randomDelay);
				  			
				  		matrix.drawPixel(randomNumber1,j-7,0);
				  		delay (randomDelay);
				  		matrix.show();
   						
  					}
  					i++;

			}


		void textGraphic (){ //Text graphic function
			
			for (i = 0 ; i<150; i++){
				matrix.fillScreen(0);  // scroll text 
  				matrix.setCursor(x, 0);
  				matrix.print(F(" Hello!"));
					 if(--x < -204) {
				    	x = matrix.width();
				    	if(++pass >= 3) pass = 0;
				    	
				    	
    			matrix.setTextColor(colors[pass]);
   				 }
				matrix.show();
  				//delay(1);
  				i++;
				}
			}	

  


