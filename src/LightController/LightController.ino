#define BLYNK_PRINT Serial


//#include <BlynkSimpleEsp8266.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

#include "SSID_Select.h"
#include "ledUi.h"

char auth[] = "c43af642e7ed4a738bbfd80869b4434f";
extern char* ssid;
extern char* pswd;

BlynkTimer timer;
WidgetRTC rtc;

long startTime;
long stopTime;

void setup()
{
  // Debug console
  Serial.begin(115200);
  initLedUi(&timer);
  //setLedUiState(LED_UI_CONNECTING);
  //if (init_ssid() != 0);
  //     setLedUiState(LED_UI_ERROR);
  init_ssid();
  Blynk.begin(auth, ssid, pswd);
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  setLedUiState(LED_UI_OK);
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V0)
{   
  int value = param.asInt(); // Get value as integer
  setLedUiState(value-1);
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

