//Valeria Suing, 3182717
//Code adapted from color sense example code 


// Circuit Playground Color Sensing Example
// Use a NeoPixel RGB LED and light sensor on the Circuit Playground board to
// do basic color detection.  By quickly flashing full red, green, and blue color
// light from the NeoPixel the light sensor can read the intensity of the
// reflected light and roughly approximate the color of the object.
//
// After uploading the sketch to Circuit Playground you can press and release the
// left or right button to do a color sense and print the red, green, blue component
// values to the serial monitor (115200 baud).  In addition, sewable NeoPixels will be lit up to the detected color.  
// You should hold a brightly colored object right above the light sensor and NeoPixel #1 (upper
// left part of board, look for the eye symbol next to the color sensor) when
// performing the color sense.
// Check Temperature fucntion was added to ensure safety of wearable 
// Certain tempetarures can be damaging to wearable, if a high temperature is detected
// there's a sound that will alert the user
//
// Author of Color Sense Example: Limor Fried & Tony DiCola
// License: MIT License (https://opensource.org/licenses/MIT)

//include NeoPixel library 
#include <Adafruit_NeoPixel.h>
#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

//A1=PIN6, PIN6 is connected to NeoPixels
//There are 6 NeooPixels attached 
//Declaring NeoPixel Object called strip: Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  // Initialize Circuit Playground library.
  CircuitPlayground.begin();
  //Initialize NeoPixel Strip 
  strip.begin();
  strip.show();
}

void loop() {
   
  // Detect if the left or right button is pressed by taking two readings with
  // a small delay inbetween.  If the button changes state (like it was pressed
  // or released) then the two readings will be different.
  bool left_first = CircuitPlayground.leftButton();
  bool right_first = CircuitPlayground.rightButton();
  delay(20);
  bool left_second = CircuitPlayground.leftButton();
  bool right_second = CircuitPlayground.rightButton();

  // Now check if either button was released, i.e. changed from a true (pressed)
  // state to a false (non-pressed) state.
  if ((left_first && !left_second) || (right_first && !right_second)) {
    // Button was pressed, perform a color sense.
    colorSense();
    //Check if temperature is good 
    checkTemperature();
  }
 
}

//Color Sense Function 
void colorSense (){
  // First turn off all the pixels to make sure they don't interfere with the
    // reading.
    CircuitPlayground.clearPixels();
    // Now take a color reading (the red, green, blue color components will be
    // returned in the parameters passed in).
    uint8_t red, green, blue;
    CircuitPlayground.senseColor(red, green, blue);
    // Print out the color components.
    Serial.print("Color: red=");
    Serial.print(red, DEC);
    Serial.print(" green=");
    Serial.print(green, DEC);
    Serial.print(" blue=");
    Serial.println(blue, DEC);
    // Gamma correction makes LED brightness appear more linear
    red   = CircuitPlayground.gamma8(red);
    green = CircuitPlayground.gamma8(green);
    blue  = CircuitPlayground.gamma8(blue);
    // Finally set all the NeoPixels to the detected color.
    for (int i=0; i<6; ++i) {
      strip.setPixelColor(i, red, green, blue);
      strip.show();
     }    
}

//Check Temperature function 
void checkTemperature(){
  //Temperature in Celsius
  float tempC;
  //find temperature
  tempC = CircuitPlayground.temperature();
  //if temperature is higher than 30, play beep to alarm user
  if (tempC > 30){
    CircuitPlayground.playTone(500, 100);
    delay(1000);
  }
}
