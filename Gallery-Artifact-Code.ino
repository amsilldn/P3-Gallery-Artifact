//All code for clock & display comes from:
//http://blog.arduinosensors.nl/2016/11/05/arduino-real-time-clock-7-segment-display/
//All code for neopixel and potentiometer integration written by Amanda Sill

#include <Wire.h>
#include <SevSeg.h> //https://github.com/sparkfun/SevSeg/tree/master/src
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 4
#define PIN 1

#define DS1307_ADDRESS 0x68
 
//Create an instance
SevSeg display7seg;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);

uint16_t pval;

int valor = 0;
 
byte zero = 0x00; 
unsigned long timer;
void setup() {
  Wire.begin();
  //Comment this line after you have set the date @ void TimeSelection() below
//   TimeSelection(); //Uncomment this line when your setting the time. Set time @ (line 66)
  
  //Change to COMMON_ANODE if you have a Common anode display.
  int displayType = COMMON_CATHODE; 
     
  int digit1 = 10; 
  int digit2 = 11; 
  int digit3 = 12; 
  int digit4 = 13; 
 
  int segA = 2;  
  int segB = 3; 
  int segC = 4; 
  int segD = 5; 
  int segE = 6; 
  int segF = 7; 
  int segG = 8; 
  int segDP= 9; 
   
  int numberOfDigits = 4;
 
  //Initialize display
  display7seg.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
   
  //Set brightness
  display7seg.SetBrightness(100);
  timer = millis();
  
  strip.begin();
}
 
void loop() {
  char tempString[10]; //Used for sprintf
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int seconds = ConvertToDecimal(Wire.read());
  int minutes = ConvertToDecimal(Wire.read());
  int hours = ConvertToDecimal(Wire.read() & 0b111111); 
  sprintf(tempString, "%02d%02d", hours, minutes);
  display7seg.DisplayString(tempString, 3);
  pval = analogRead(A0);
  pval = map(pval, 0, 1023, 0, 255);

  switch(minutes) {
    case 00:
    turnOn(strip.Color(150,255,0), pval);
    break;
    default:
    turnOff(strip.Color(150,255,0));
    break;
  }
}
 
void TimeSelection() {
  //Select date and time here below!!(line 69,70,71)
  //after setting time also comment time TimeSelection() @ void setup

  byte seconds = 10; //Valores de 0 a 59
  byte minutes = 30; //Valores de 0 a 59
  byte hours = 16; //Valores de 0 a 23
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //Stop no CI para que o mesmo possa receber os dados
 
  //As linhas abaixo escrevem no CI os valores de 
  //data e hora que foram colocados nas variaveis acima
  Wire.write(ConvertToPCD(seconds));
  Wire.write(ConvertToPCD(minutes));
  Wire.write(ConvertToPCD(hours));
  Wire.write(zero);
  Wire.endTransmission(); 
}

void turnOn(uint32_t c, uint16_t v) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
    strip.setPixelColor(i, c);
    strip.setBrightness(v);
    strip.show();
  }
}

void turnOff(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
    strip.setPixelColor(i, 0);
    strip.setBrightness(0);
    strip.show();
  }
}
 
byte ConvertToPCD(byte val) { 
  //Convert Decimal to binary
  return ( (val/10*16) + (val%10) );
}
 
byte ConvertToDecimal(byte val) { 
  //Convert binary to decimal
  return ( (val/16*10) + (val%16) );
}
