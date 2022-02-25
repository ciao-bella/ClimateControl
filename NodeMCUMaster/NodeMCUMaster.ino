// <Wire.h> is a library that allows you to communicate with I2C / TWI devices. We connect the SDA->SDA (serial clock pins)and SCL->SCL (serial data pins).
// On our Arduino Uno, SDA/SCL pins are just above the AREF pin. https://docs.arduino.cc/learn/communication/wire
// On our NodeMCU ESP8266 board we have pin D2 = SDA and D1 = SCL https://steve.fi/hardware/nodemcu/default-pins/ ; https://www.youtube.com/watch?v=SiU-QZwik8w (1:30 for schematic) ;
#include <Wire.h>

#define DEBUG 1 //set to 0 to save compile space

union PunnedFloat //This allows us to practice "type punning" https://en.wikipedia.org/wiki/Type_punning
                  //http://www.cplusplus.com/forum/beginner/18566/
                  //We receive the information from Wire.read() as individual bytes and this allows us to treat memory allocated to 4 bytes as a "float" type
                  //there should also be solutions that allow us to memory stream into variables
{
        float   f[3];
        unsigned char c[0];
};
PunnedFloat data; 



void setup() {
  Wire.begin(D1, D2); // join i2c bus with SDA=D1 and SCL=D2 of NodeMCU
    
  #if DEBUG
    Serial.begin(9600);  // start serial for output
  #endif

}


void loop()
{
  Wire.requestFrom(8, 12);    // request 12 bytes from peripheral device #8

  int i = 0;
  
  while (Wire.available()) { // peripheral may send less than requested
    data.c[i] = Wire.read(); // receive a byte
    #if DEBUG
      Serial.println((unsigned int)data.c[i]);         // print the character
    #endif
    ++i;
  }
  
  #if DEBUG
    Serial.println();
    Serial.println(data.f[0]);
    Serial.println(data.f[1]);
    Serial.println(data.f[2]);
  #endif
  
  delay(5000);

}
