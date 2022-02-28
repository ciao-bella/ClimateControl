// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "" //these macros must be placed before the inclusion of <ESP8266WiFi.h> and <BlynkSimpleEsp8266.h> as per https://community.blynk.cc/t/invalid-auth-token-issue/54876
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define DEBUG 1 //set to 0 to save compile space

#if DEBUG
  // Comment this out to disable prints and save space
  #define BLYNK_PRINT Serial
#endif






#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// <Wire.h> is a library that allows you to communicate with I2C / TWI devices. We connect the SDA->SDA (serial clock pins)and SCL->SCL (serial data pins).
// On our Arduino Uno, SDA/SCL pins are just above the AREF pin. https://docs.arduino.cc/learn/communication/wire
// On our NodeMCU ESP8266 board we have pin D2 = SDA and D1 = SCL https://steve.fi/hardware/nodemcu/default-pins/ ; https://www.youtube.com/watch?v=SiU-QZwik8w (1:30 for schematic) ;
#include <Wire.h>




union PunnedFloat //This allows us to practice "type punning" https://en.wikipedia.org/wiki/Type_punning
                  //http://www.cplusplus.com/forum/beginner/18566/
                  //We receive the information from Wire.read() as individual bytes and this allows us to treat memory allocated to 4 bytes as a "float" type
                  //there should also be solutions that allow us to memory stream into variables
{
        float   f[3];
        unsigned char c[0];
};




char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";


PunnedFloat data; 
BlynkTimer timer; // Creating a timer object




void setup()
{
  // Debug console  
  #if DEBUG
    Serial.begin(115200);
    Serial.println("Starting setup...");
  #endif
  
  Blynk.begin(auth, ssid, pass);

  Wire.begin(D1, D2); // join i2c bus with SDA=D1 and SCL=D2 of NodeMCU
  timer.setInterval(300000L, refreshAndUploadEvent); //Staring a timer that triggers an event every 5 minutes
}

void loop()
{
  Blynk.run();
  timer.run();
}

void refreshAndUploadEvent()
{
  requestTemperatureData();
  #if DEBUG
    Serial.println("requesting temperature data...");
  #endif
  
  uploadTemperatureData();
  #if DEBUG
    Serial.println("requesting temperature data...");
  #endif
}

//Requests temperature data from the Arduino Uno via GPIO
//writes requested data to the global variable "data"
void requestTemperatureData()
{

    Wire.requestFrom(8, 12);    // request 12 bytes from peripheral device address #8

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
      Serial.println(data.f[0]); //humidity
      Serial.println(data.f[1]); //temperature
      Serial.println(data.f[2]); //heat index
    #endif
}


//Uploads the temperature data to Blynk.Cloud via virtual pins
void uploadTemperatureData()
{
  Blynk.virtualWrite(V1, data.f[0]); //humidity
  Blynk.virtualWrite(V0, data.f[1]); //temperature
  Blynk.virtualWrite(V2, data.f[2]); //heat index
}
