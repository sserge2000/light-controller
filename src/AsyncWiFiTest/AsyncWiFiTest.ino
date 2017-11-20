#include <ESP8266WiFi.h>
#define HEART_BEAT_DELAY 10000


#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

int lastWiFiStatus = -1;
int maxDelay = -1;
long lastTime;
long lastHeartBeatTime = 0;
long currentDelay = -1;
bool firstRun = true;
bool lastBlynkConnectionState = false;


void setup() {
  Serial.begin(115200);
  lastTime = millis();
  WiFi.begin("iPhone serge", "mace6155~");
  
  Blynk.config("f10d16a3d4344232bf50bc896af8139f");


}

void loop() {
  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
    bool blynkConnected = Blynk.connected();
    if (lastBlynkConnectionState != blynkConnected) {
      lastBlynkConnectionState = blynkConnected;
      Serial.print("Blynk connected: ");
      Serial.println(lastBlynkConnectionState);
    }
  }
  
  currentDelay = millis() - lastTime;
  lastTime = millis();
  if (firstRun) {
    firstRun = false;
    Serial.println("");
    Serial.print("Setup Delay: ");
    Serial.println(currentDelay);
//    Serial.println(maxDelay);
  }
  else {
    if (currentDelay > maxDelay) {
      maxDelay = currentDelay;
      Serial.print("Max Delay: ");
      Serial.println(currentDelay);
//      Serial.println(maxDelay);
    }
  }
  
  int currentWiFiStatus = WiFi.status();
  if (currentWiFiStatus != lastWiFiStatus) {
    lastWiFiStatus = currentWiFiStatus;
    Serial.print("WiFi Status: ");
    Serial.println(currentWiFiStatus);
  }
  if (millis() - lastHeartBeatTime > HEART_BEAT_DELAY) {
    lastHeartBeatTime = millis();
    Serial.print("HB- WiFi: ");
    Serial.print(currentWiFiStatus);
    Serial.print(" Max Delay: ");
    Serial.print(maxDelay);
    Serial.print(" Blynk connected: ");
    Serial.println(lastBlynkConnectionState);
    maxDelay = -1;
  }
}
