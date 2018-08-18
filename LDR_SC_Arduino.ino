/*                
  LDR_Color_Sensor
  Code by : LKbrilliant
  Date: 17:05:2016
  Used LED : WS2812B RGB LED
  
  The color sensor needs to be calibrated before using. First connect the RGB LED and the LDR to an arduino
  and uploade the code to it. Then connect the arduino to the PC and open the 'processing' application. By using
  the interface select the correct COM Port for the arduino. When establishing the connection the arduino will
  automatically 'reset' itself. Then by following the 'calibration' calibrate the sensor

  Calibration : Point the sensor to a white surface after the first color cycle.
                When the white calibration is done the second color cycle will begin.
                After the second cycle, point the sensor to a black surface to begin the next stage.
                When that is done the LED will turn to green and blink three times to indicate the end of the calibration.
  
  After calibrating the sensor by using black and white surfaces, 
  the sensor will measure the color on a targeted surface and get its color in RGB color code.
  By using 'Processing' the color is shown in the computer window.

 *       ***If the calibration fails, just hit the reset and start over. 
 *       ***If the sensor fails to detect a color, simply point the sensor somewhere else for a short period of time and try it again.
 *       ***Use 10k pull down resistor for the LDR >>>
 *       ***Connect WS2812B's DIN pin to one of arduino's digital pin >>>

                 WS2812B
             VCC  ______ DIN                                                            LDR
  +5v o----------|      |----------o  Arduino digital pin(ledPin - D3)        +5v O----(/\/\)----O------O Arduino analog pin(ldrPin - A6)
                 |      |                                                                        |
               --|______/----------o GND                                                       (/\/) 10k
             DOUT        GND                                                                     |
                                                                                                GND

*/

#include <Adafruit_NeoPixel.h>       // Include Adafruit NeoPixel library

#define PIN             3        // LED on digital pin 3
#define NUMLEDS         1        // Total number of LEDs
#define ldrPin          6        // LDR on analog pin 0
#define readingDelay   10        // Delay between two readings
#define wait            5        // Delay between each color(standard)
#define bnwTimes        5        // How much readings the sensor should get to calculate an average for black and white balance
#define kp              2        // Color cycle changes(increments and decrements)

Adafruit_NeoPixel led = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);       // Create 'led' object to drive LED

boolean calibration;
int average;
int rgb[] = {0, 0, 0};
int whiteBalance[] = {0, 0, 0};
int blackBalance[] = {0, 0, 0};
float color[] = {0, 0, 0};

void setup() {
  calibration = false;
  led.begin();                                  // Initialize 'led' object
  Serial.begin(9600);                           // Begin the Serial communication
  led.setPixelColor(0, led.Color(0, 0, 0));     // Turn off the LED
  led.show();                                   // Show new values
  delay(5000);                                  // Delay some time to catch up
}

void loop() {
  if (calibration == false) {
    colorSeq(0);
    getWhite();                   // Take RGB color values from a white surface
    colorSeq(0);
    getBlack();                   // Take values from a black surface
    colorSeq(1);
    calibration = true;           // Mark the calibration as done
  }
  getColor(5);                    // Measure RGB values from a pointed surface. (getColor(x) --> x = how much readings from each color)
  printColor();                   // Print the RGB value in "xxx : xxx : xxx" format
}

void getWhite() {                         // Get values from a white surface
  Serial.println("white:");
  for (int i = 0; i <= 2; i++) {
    int rgb[] = {0, 0, 0};
    if (i == 0) {
      rgb[0] = 255;
    }
    else if (i == 1) {
      rgb[1] = 255;
    }
    else if (i == 2) {
      rgb[2] = 255;
    }
    led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
    led.show();
    delay(20);
    getReadings(bnwTimes);
    whiteBalance[i] = average;
    Serial.println(whiteBalance[i]);
    
  }
}

void getBlack() {                                   // Take RGB color values from a black surface
  Serial.println("Black:");
  for (int i = 0; i <= 2; i++) {
    int rgb[] = {0, 0, 0};
    if (i == 0) {
      rgb[0] = 255;
    }
    else if (i == 1) {
      rgb[1] = 255;
    }
    else if (i == 2) {
      rgb[2] = 255;
    }
    led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
    led.show();
    delay(20);
    getReadings(bnwTimes);
    blackBalance[i] = average;
    Serial.println(blackBalance[i]);
  }
}

void getColor(byte timesC) {
  for (int i = 0; i <= 2; i++) {
    int rgb[] = {0, 0, 0};
    if (i == 0) {
      rgb[0] = 255;
    }
    else if (i == 1) {
      rgb[1] = 255;
    }
    else if (i == 2) {
      rgb[2] = 255;
    }
    led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
    led.show();
    delay(20);

    int readingC = 0;
    int totalC = 0;
    for (byte u = 1; u <= timesC; u++) {
      readingC = analogRead(ldrPin);            // Get readings from the analog pin
      totalC += readingC;                       // Make a total from read values
      delay(readingDelay);                      // Use a delay between two readings
    }
    int preAverage = totalC / timesC;
    if (preAverage > blackBalance[i] && preAverage < whiteBalance[i]) {     // Make sure 'preAverage' is in between highest and lowest value
      average = preAverage;                                                 // Drop 'preAverage' value into 'average' variable

      float differ1 = whiteBalance[i] - blackBalance[i];                    // Calculate the RGB color value
      float differ2 = average - blackBalance[i];
      float dff3 = differ2 / differ1;
      color[i] = dff3 * 255;
    }
    else {
    }
  }
}


void getReadings(byte times) {          // Get readings from the LDR
  int reading;
  int total = 0;
  for (byte u = 1; u <= times; u++) {
    reading = analogRead(ldrPin);
    total += reading;                   // Make the total by adding each reading values together
    delay(readingDelay);                // delay some tome between two readings
  }
  average = total / times;              // Calculate the average
}

void colorSeq(byte caliDone) {
  if (caliDone == 0) {
    colorCycle();
  }
  else {
    for (int i = 1; i <= 3; i++) {
      led.setPixelColor(0, led.Color(0, 0, 0));
      led.show();
      delay(1000);
      led.setPixelColor(0, led.Color(0, 255, 0));
      led.show();
      delay(1000);
    }
    delay(1000);
  }
}

void colorCycle() {
  for (byte i = 1; i <= 6; i++) {
    if (i == 1) {
      rgb[0] = 255;
      rgb[1] = 0;
      for (rgb[2] = 0; rgb[2] <= 255; rgb[2] += kp) {
        led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
        delay(wait);
      }
    }
    else if (i == 2) {
      rgb[2] = 255;
      rgb[1] = 0;
      for (rgb[0] = 255; rgb[0] >= 0; rgb[0] -= kp) {
        led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
        delay(wait);
      }
    }
    else if (i == 3) {
      rgb[2] = 255;
      rgb[0] = 0;
      for (rgb[1] = 0; rgb[1] <= 255; rgb[1] += kp) {
        led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
        delay(wait);
      }
    }
    else if (i == 4) {
      rgb[1] = 255;
      rgb[0] = 0;
      for (rgb[2] = 255; rgb[2] >= 0; rgb[2] -= kp) {
        led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
        delay(wait);
      }
    }
    else if (i == 5) {
      rgb[1] = 255;
      rgb[2] = 0;
      for (rgb[0] = 0; rgb[0] <= 255; rgb[0] += kp) {
        led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
        delay(wait);
      }
    }
    else if (i == 6) {
      rgb[0] = 255;
      rgb[2] = 0;
      for (rgb[1] = 255; rgb[1] >= 0; rgb[1] -= kp) {
        led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
        delay(wait);
      }
    }
  }
  for ( int i = 1; i <= 10; i++) {
    led.setPixelColor(0, led.Color(0, 0, 0));
    led.show();
    delay(50);
    led.setPixelColor(0, led.Color(0, 0, 255));
    led.show();
    delay(100);
  }
  delay(1000);
}

void printColor() {                     // Print RGB values
  int pColor[] = {0, 0, 0};
  pColor[0] = color[0];                 // convert 'float' values into 'int'
  pColor[1] = color[1];
  pColor[2] = color[2];

  for (byte i = 0; i <= 1; i++) {                        // Print red and green values
    if (pColor[i] < 10) {                                // If a value less than 10, add two zeros in front
      Serial.print("00");
      Serial.print(pColor[i]);
    }
    else if (pColor[i] > 9 && pColor[i] < 100) {         // If a value greater than 9 and less than 99, add one zero in front
      Serial.print("0");
      Serial.print(pColor[i]);
    }
    else Serial.print(pColor[i]);                        // Else, print the value
    Serial.print(" : ");
  }
  if (pColor[2] < 10) {                                  // Print Blue value
    Serial.print("00");
    Serial.println(pColor[2]);
  }
  else if (pColor[2] > 9 && pColor[2] < 100) {
    Serial.print("0");
    Serial.println(pColor[2]);
  }
  else Serial.println(pColor[2]);
}
