// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <IRrecv.h>
#include <EEPROM.h>
uint16_t RECV_PIN = 14;
IRrecv irrecv(RECV_PIN);
#include <IRutils.h>

decode_results results;

// WiFi network info.
char ssid[] = "Radar";
char wifiPassword[] = "radar1234@";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "7a3a4c40-b32d-11e6-b51c-abe30c418779";
char password[] = "2ac5a08fa30d47b3a5534950b7924708bc819f93";
char clientID[] = "a05b9270-1bb0-11e8-9f38-9fae3d42ebf0";
int sensorPin = A0;
int sensorValue = 0;  
unsigned long lastMillis = 0;
unsigned int irdata=0;
int bulb=0;
int count=0;
 double Irms=0 ;
int tube=0;
#include "EmonLib.h"
EnergyMonitor emon1;
void setup() {
	Serial.begin(9600);
 EEPROM.begin(512);
 irrecv.enableIRIn();
	 emon1.current(A0, 111.1); 
  pinMode(15,OUTPUT);//FAN
  pinMode(12,OUTPUT);//LED
  pinMode(13,OUTPUT);//TUBE-LIGHT
  digitalWrite(15,EEPROM.read(15));
  digitalWrite(12,EEPROM.read(12));
  digitalWrite(13,EEPROM.read(13));
 
    Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Cayenne.loop();
 if (irrecv.decode(&results)) {
  irdata=results.value;
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, DEC);
    Serial.println("");
   
    switch(irdata){
      case 2155846245  :digitalWrite(13, !digitalRead(13));
                     break;
      case 2155813605 : digitalWrite(12, !digitalRead(12));
                      break;
      case 2155829925  : digitalWrite(15, !digitalRead(15));
                       break;
      case 33444015  :digitalWrite(13, !digitalRead(13));
                     break;
      case 33478695 : digitalWrite(12, !digitalRead(12));
                      break;
      case 33486855  : digitalWrite(15, !digitalRead(15));
                       
                     break;
      default: break;               

    }
    
    irrecv.resume();  // Receive the next value
  }
  
  Irms = emon1.calcIrms(1480);  
  if (Irms>10 && Irms<14){
   bulb=1;
   tube=0;
  }
  if( Irms>14 && Irms<18)
 {
  bulb=0;
  tube=1;
 }
  else if( Irms>26 && Irms<32)
 {
  bulb=1;
  tube=1;
 }
  else if(  Irms<1)
 {
  bulb=0;
  tube=0;
 }
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.


// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  digitalWrite(request.channel, !digitalRead(request.channel));
}
CAYENNE_OUT_DEFAULT()

{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  //Cayenne.virtualWrite(0, millis());
  // Some examples of other functions you can use to send data.
  //Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
 
Cayenne.virtualWrite(18,digitalRead(15) );
 Cayenne.virtualWrite(16,tube );
 Cayenne.virtualWrite(17,bulb );


EEPROM.write(15, digitalRead(15));
  EEPROM.write(13, digitalRead(13));
  EEPROM.write(12, digitalRead(12));
  
    EEPROM.commit();
}

