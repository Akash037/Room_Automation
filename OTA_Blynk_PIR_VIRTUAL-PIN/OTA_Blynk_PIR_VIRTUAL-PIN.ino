#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp8266.h>


//INPUT
#define pir 5  //D1
#define fan 14 //D5
#define led 12 //D6
#define tubelight 13 //D7


//OUTPUT
#define fanop 4 //D2 -  V6
#define tubelightop 0 //D3 - V5
#define ledop 2 //D4 -  V4

BlynkTimer timer;

WidgetLED led1(V1);

int pirvalue;
int fanvalue;
int ledvalue;
int tubelightvalue;
int motion = 1;
int sensepin;

int led_state=0;
int last_ledvalue=0;

int tubelight_state=0;
int last_tubelightvalue=1;

int fan_state=0;
int last_fanvalue=0;

char auth[] = "XUM7d3oXlHHGDvP8wNKCkMfMAcJ-ocsR";


char ssid[] = "JioFiber4g";
char pass[] = "762122ds";

//const char* ssid = "JioFiber4g";
const char* password = "762122ds";



//LED
BLYNK_WRITE(V4)  /// LED output 2/D4
{
  if(param.asInt())
  {
    digitalWrite(2, LOW);
    led_state=1;
  }
  else
  {
    digitalWrite(2, HIGH);
    led_state=0;
  }  
}

void led_switch()
{
  if(ledvalue!=last_ledvalue)
  {
    last_ledvalue=ledvalue;

      if(led_state==1)
      {
        led_state=0;
        digitalWrite(2, HIGH);
        Blynk.virtualWrite(V4, LOW);
      }
      else
      {
        led_state=1;
        digitalWrite(2, LOW);
        Blynk.virtualWrite(V4, HIGH);
      }
  }
}



// Tubelight
BLYNK_WRITE(V5)  /// LED output 2/D4
{
  if(param.asInt())
  {
    digitalWrite(0, LOW);
    tubelight_state=1;
  }
  else
  {
    digitalWrite(0, HIGH);
    tubelight_state=0;
  }  
}

void tubelight_switch()
{
  if(tubelightvalue!=last_tubelightvalue)
  {
    last_tubelightvalue=tubelightvalue;

      if(tubelight_state==1)
      {
        tubelight_state=0;
        digitalWrite(0, HIGH);
        Blynk.virtualWrite(V5, LOW);
      }
      else
      {
        tubelight_state=1;
        digitalWrite(0, LOW);
        Blynk.virtualWrite(V5, HIGH);
      }
  }
}



// FAN
BLYNK_WRITE(V6)  /// LED output 2/D4
{
  if(param.asInt())
  {
    digitalWrite(4, LOW);
    fan_state=1;
  }
  else
  {
    digitalWrite(4, HIGH);
    fan_state=0;
  }  
}

void fan_switch()
{
  if(fanvalue!=last_fanvalue)
  {
    last_fanvalue=fanvalue;

      if(fan_state==1)
      {
        fan_state=0;
        digitalWrite(4, HIGH);
        Blynk.virtualWrite(V6, LOW);
      }
      else
      {
        fan_state=1;
        digitalWrite(4, LOW);
        Blynk.virtualWrite(V6, HIGH);
      }
  }
}


BLYNK_WRITE(V0)
{
  sensepin = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}



void blinkLedWidget()
{
  
if((motion==1)&&(pirvalue==HIGH))
{
  led1.on();
  motion=0;
}

if((motion==0)&&(pirvalue==LOW))
{
  led1.off();
  motion=1;
}


 if((sensepin==1)&&(pirvalue==HIGH))
{
  digitalWrite(fanop, LOW);
  digitalWrite(tubelightop, LOW);
  delay(500);
  digitalWrite(ledop, HIGH);
  fan_state=1;
  tubelight_state=1;
  led_state=0;
  Blynk.virtualWrite(V6, HIGH); //fan
  Blynk.virtualWrite(V5, HIGH); //tubelight
  Blynk.virtualWrite(V4, LOW); //LED
}


}


void setup() {
  Serial.begin(115200);
  
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(2000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("My_Room");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, pass);
  pinMode(pir, INPUT_PULLUP);
  pinMode(fan, INPUT_PULLUP);
  pinMode(led, INPUT_PULLUP);
  pinMode(tubelight, INPUT_PULLUP);

  pinMode(fanop, OUTPUT);
  pinMode(ledop, OUTPUT);
  pinMode(tubelightop, OUTPUT);
  

  timer.setInterval(10L, blinkLedWidget);  //1000L = 1 SECOND
}


void loop() 
{
  
ArduinoOTA.handle();
Blynk.run();
timer.run();

pirvalue = digitalRead(pir);


fanvalue = digitalRead(fan);
fan_switch();

tubelightvalue = digitalRead(tubelight);
tubelight_switch();

ledvalue = digitalRead(led);
led_switch();

}
