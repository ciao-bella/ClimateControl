// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, edited by cia0-bella, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

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



void setup() {
  #if DEBUG 
      Serial.begin(9600);
      Serial.println(F("DHTxx DEBUGGIN!"));
  #endif

   pinMode(RELAYPIN, OUTPUT);

  //Begin reading 40-bit stream from AM2302
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if ( isnan(t) || isnan(h) ) {
      #if DEBUG 
        Serial.println(F("Failed to read from DHT sensor!"));
      #endif
    return;
  }

//   Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  if(t <= 20.00) { //keep heating the space until the sensor reads a value of 20C or higher
    digitalWrite(RELAYPIN, HIGH); //turns the relay on
    #if DEBUG 
      Serial.println("Heater is ON");
    #endif

  }
  else {
    digitalWrite(RELAYPIN, LOW); //pulls the pin on the relay
    #if DEBUG 
      Serial.println("Heater is OFF");
    #endif
  }

  #if DEBUG 
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.print(F("°C "));
      Serial.print(F("Heat index: "));
      Serial.print(hic);
      Serial.print(F("°C "));
      Serial.println();
  #endif

}