//Robo India Tutorial on Motion Detection With Blynk App
//Hardware Required: NodeMCU & PIR Sensor
//Software: Blynk App
//http://roboindia.com/tutorials/

#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial       
#include <BlynkSimpleEsp8266.h>
char auth[] = "NOSGh5xu4UWCo5GXNdedrxV_T8QGJWyI";

/* WiFi credentials */
char ssid[] = "JioFiber4g";
char pass[] = "762122ds";

/* HC-SR501 Motion Detector */
#define pirPin 5                // Input for HC-S501
int pirValue;                   // Place to store read PIR Value
int pinValue;                   //Variable to read virtual pin

BLYNK_WRITE(V0)
{
 pinValue = param.asInt();    
} 

void setup()
  {
    Serial.begin(115200);
    delay(10);
    Blynk.begin(auth, ssid, pass);
    pinMode(pirPin, INPUT);
  }

void loop()
  {
     if (pinValue == HIGH)    
      {
        getPirValue();
      }
    Blynk.run();
  }

void getPirValue(void)        //Get PIR Data
  {
   pirValue = digitalRead(pirPin);
    if (pirValue) 
     { 
       Serial.println("Motion detected");
       Blynk.notify("Motion detected");  
     }
  }
