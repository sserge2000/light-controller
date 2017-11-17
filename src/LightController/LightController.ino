#define BLYNK_PRINT Serial


//#include <BlynkSimpleEsp8266.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

#include "creds.h"
#include "SSID_Select.h"
#include "ledUi.h"

char auth[] = BLYNK_AUTH;
extern char* ssid;
extern char* pswd;

BlynkTimer timer;
WidgetRTC rtc;

#define RELAY_PIN 13
WidgetLED relayLed(V10);

long startTime;
long stopTime;

#define CM_OFF 1
#define CM_ON 2
#define CM_AUTO 3
#define CS_OFF 0
#define CS_ON 1

int controller_mode = CM_OFF;
int controller_state = CS_OFF;

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

void setup()
{
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Debug console
  Serial.begin(115200);
  initLedUi(&timer);
  setLedUiState(LED_UI_OK);

  //setLedUiState(LED_UI_CONNECTING);
  //if (init_ssid() != 0);
  //     setLedUiState(LED_UI_ERROR);
  init_ssid();
  Blynk.begin(auth, ssid, pswd);
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  //setLedUiState(LED_UI_OK);
  setLedUiState(LED_UI_CONNECTING);
  timer.setInterval(1000, controllerTick);
}

void loop()
{
  Blynk.run();
  timer.run();
}

//BLYNK_WRITE(V0)
//{   
//  int value = param.asInt(); // Get value as integer
//  setLedUiState(value-1);
//}

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

