// <Wire.h> is a library that allows you to communicate with I2C / TWI devices. We connect the SDA->SDA (serial clock pins)and SCL->SCL (serial data pins).
// On our Arduino Uno, SDA/SCL pins are just above the AREF pin. https://docs.arduino.cc/learn/communication/wire
// On our NodeMCU ESP8266 board we have pin D2 = SDA and D1 = SCL https://steve.fi/hardware/nodemcu/default-pins/ ; https://www.youtube.com/watch?v=SiU-QZwik8w (1:30 for schematic) ;
#include <Wire.h>


#define DEBUG 1 //set to 0 to save compile space

void setup() {
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
  //Wire.begin();        // join i2c bus (address optional for master) but I prefer the above line for readability


  
  #if DEBUG
    Serial.begin(9600);  // start serial for output
  #endif
}

void loop() {
  Wire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8

  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character

    #if DEBUG
      Serial.print(c);         // print the character
    #endif
  }
  
  delay(500);
}
