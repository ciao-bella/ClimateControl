// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, edited by cia0-bella, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <DHT.h> //lets us read the temperature and humidity

// <Wire.h> is a library that allows you to communicate with I2C / TWI devices. We connect the SDA->SDA (serial clock pins)and SCL->SCL (serial data pins).
// On our Arduino Uno, SDA/SCL pins are just above the AREF pin. https://docs.arduino.cc/learn/communication/wire
// On our NodeMCU ESP8266 board we have pin D2 = SDA and D1 = SCL https://steve.fi/hardware/nodemcu/default-pins/ ; https://www.youtube.com/watch?v=SiU-QZwik8w (1:30 for schematic) ;
#include <Wire.h>

#define RELAYPIN 8 //control the relay to heating source with this pin
#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#define DEBUG 1 //set DEBUG macro to 1 to print values to Serial monitor, 0 otherwise



// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);


union ByteChunk { //"type punning" so we can force 3 floats through the wires as a series of bits
  float   f[3];
  byte c[12];
};
ByteChunk data;


void setup() {
  #if DEBUG 
      Serial.begin(9600);
      Serial.println(F("DHTxx DEBUGGING!"));
  #endif
  
//   pinMode(RELAYPIN, OUTPUT);

  //Begin reading 40-bit stream from AM2302
  dht.begin();
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}


void loop() {
  // Wait a few seconds between measurements.
  delay(10000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  data.f[0] = h;
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  data.f[1] = t;


  // Check if any reads failed and exit early (to try again).
  if ( isnan(t) || isnan(h) ) {
      #if DEBUG 
        Serial.println(F("Failed to read from DHT sensor!"));
      #endif
    return;
  }

//   Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  data.f[2] = hic;

// We burnt out a relay so we are commenting out this code until we find a safer solution. This is going to be a "read-only" thermostat for a while.

//TODO put this logic in its owns function
//  if(t <= 24.00) { //keep heating the space until the sensor reads a value of 20C or higher
//    digitalWrite(RELAYPIN, LOW); //allows current through
//    #if DEBUG 
//      Serial.println("Heater is ON");
//    #endif
//
//  }
//  else {
//    digitalWrite(RELAYPIN, HIGH); //pulls the pin on the relay
//    #if DEBUG 
//      Serial.println("Heater is OFF");
//    #endif
//  }

  #if DEBUG 
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.print(F("??C "));
      Serial.print(F("Heat index: "));
      Serial.print(hic);
      Serial.print(F("??C "));
      Serial.println();
  #endif
  
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() { 
  Wire.write(data.c, sizeof(data)); //I hate this
}
