#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//#include <WiFi.h>
//#include <BlynkSimpleEsp32.h>

//#include <WiFiClient.h>

#include <TimeLib.h>
#include <WidgetRTC.h>

#include "creds.h"
#include "SSID_Select.h"
#include "ledUi.h"

char auth[] = BLYNK_AUTH;
extern char* ssid;
extern char* pswd;
#define BLYNK_CHECK_INTERVAL 15000 //15sec

BlynkTimer timer;
WidgetRTC rtc;

#define RELAY_PIN 13
WidgetLED relayLed(V10);

long startTime;
long stopTime;

#define CM_OFF 1
#define CM_ON 2
#define CM_AUTO 3
#define CS_OFF 1 //relay is normally connected
#define CS_ON 0 // to disconnect, we write HIGH to RELAY_PIN

int controller_mode = CM_ON;
int controller_state = CS_ON; //Relay is started in connected state

boolean connectionWasAlive = false;

void controllerTick()
{
  switch (controller_mode)
  {
    case CM_OFF:
      controller_state = CS_OFF;
      break;
    case CM_ON:
      controller_state = CS_ON;
      break;
    case CM_AUTO:
      long currentTime = second() + 60*minute() + 60*60*hour();
      if (startTime <= currentTime && currentTime <= stopTime)
        controller_state = CS_ON;
      else
        controller_state = CS_OFF;
      break;
  }
  digitalWrite(RELAY_PIN, controller_state);
  if (CS_ON==controller_state)
    relayLed.on();
   else
    relayLed.off();
}

void checkBlynk()
{
  if (WiFi.status() == WL_CONNECTED)  
    {
      if(!Blynk.connected())
      {
        Blynk.connect(); 
      }
    }
}

void setup()
{
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Serial.begin(115200);
  initLedUi(&timer);
  setLedUiState(LED_UI_ON);

  init_ssid();
  WiFi.begin(ssid,pswd);
  Blynk.config(auth);
  
  setSyncInterval(10 * 60); // RTC Sync interval in seconds (10 minutes)
  setLedUiState(LED_UI_FAST_BLINK);
  timer.setInterval(1000, controllerTick);
  timer.setInterval(BLYNK_CHECK_INTERVAL, checkBlynk);
}

void loop()
{
  if (Blynk.connected())
  {
    if (!connectionWasAlive)
    {
      connectionWasAlive = true;
      setLedUiState(LED_UI_SLOW_BLINK);
    }
    Blynk.run();
  }
  else
    if (connectionWasAlive)
    {
      connectionWasAlive = false;
      setLedUiState(LED_UI_FAST_BLINK);
    }
  
  timer.run();
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
  Blynk.syncAll();
}

BLYNK_WRITE(V1)
{   
  startTime = param[0].asInt();
  stopTime = param[1].asInt();
  Serial.print("Start: ");
  Serial.println(startTime);
  Serial.print("Stop: ");
  Serial.println(stopTime);
}

BLYNK_WRITE(V2)
{   
  controller_mode = param.asInt();
  controllerTick(); 
}

