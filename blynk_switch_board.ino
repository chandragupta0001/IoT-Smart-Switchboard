/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "EmonLib.h"
EnergyMonitor emon1;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "wUgcEj6z4uZMn-DM2Xsxi1q98wO0cocF";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Radar@2.4GHz";
char pass[] = "Kautilya@1234";
#include <EEPROM.h>
double Irms=0;
 unsigned long currentMillis;
unsigned long previousMillis=0;
int interval=1000;
void setup()
{
  // Debug console
  Serial.begin(9600);
emon1.current(A0, 111.1); 
  Blynk.begin(auth, ssid, pass);
  EEPROM.begin(512);
    pinMode(15,OUTPUT);//FAN
  pinMode(12,OUTPUT);//LED
  pinMode(13,OUTPUT);//TUBE-LIGHT
    digitalWrite(15,EEPROM.read(15));
  digitalWrite(12,EEPROM.read(12));
  digitalWrite(13,EEPROM.read(13));
// 
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

//int arr[100];
//int i=0;

//int rmsValue(int arr[], int n) 
//{ 
//    int square = 0; 
//    float mean = 0.0, root = 0.0; 
//  
//    // Calculate square. 
//    for (int i = 0; i < n; i++) { 
//        square += pow(arr[i], 2); 
//    } 
//  
//    // Calculate Mean. 
//    mean = (square / (float)(n)); 
//  
//    // Calculate Root. 
//    root = sqrt(mean); 
//  
//    return root; 
//}
void loop()
{
  Blynk.run();
//  int a=analogRead(A0);
//  if(i<50){
//    arr[i]=a;
//    i++;
//  }
//  else{
//    i=0;
//    Blynk.virtualWrite(V5, rmsValue(arr,50));
//  }
  currentMillis = millis();
  
  if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;   
Irms = emon1.calcIrms(150); 
Blynk.virtualWrite(V5, Irms);
EEPROM.write(15, digitalRead(15));
  EEPROM.write(13, digitalRead(13));
  EEPROM.write(12, digitalRead(12));
  
    EEPROM.commit();

  }
  


}
