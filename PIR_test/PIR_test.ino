#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define BLYNK_PRINT Serial
#define pir 5
#define fan D5
#define led D6
#define tubelight D7


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;

WidgetLED led1(V1);
WidgetLED led2(V2);

int pirvalue;
int fanvalue;
int ledvalue;
int tubelightvalue;

char auth[] = "NOSGh5xu4UWCo5GXNdedrxV_T8QGJWyI";


char ssid[] = "JioFiber4g";
char pass[] = "762122ds";

//const char* ssid = "JioFiber4g";
const char* password = "762122ds";

void setup() {
  Serial.begin(115200);
  
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
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
  pinMode(pir, INPUT);
  pinMode(fan, INPUT);
  pinMode(led, INPUT);
  pinMode(tubelight, INPUT);
}


void loop() {
  ArduinoOTA.handle();
  Blynk.run();
pirvalue = digitalRead(pir);
fanvalue = digitalRead(fan);
ledvalue = digitalRead(led);
tubelightvalue = digitalRead(tubelight);


if (pirvalue == HIGH)    
{
led1.on();
}
if (pirvalue == LOW)
{
led1.off();
}

if (fanvalue == HIGH)    
{
led2.on();
}
else
{
led2.off();
}

if (ledvalue == HIGH)    
{
Blynk.virtualWrite(V3, HIGH);
}
else
{
Blynk.virtualWrite(V3, LOW);
}

if (tubelightvalue == HIGH)    
{
Blynk.virtualWrite(V4, HIGH);
}
else
{
Blynk.virtualWrite(V4, LOW);
}





}
