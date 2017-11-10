#include "creds.h"

#include "SSID_Select.h"
#include <string.h>
//#include <ESP8266WiFi.h>
#include "WiFi.h"


char* ssid;
char* pswd;

int check_ssid(const char* ssid_to_check, char* l_ssid, char* l_pswd)
{
    if (0 == strcmp(l_ssid, ssid_to_check))
    {
        ssid = l_ssid;
        pswd = l_pswd;
        return 1;
    }
    else
        return 0;
}

int select_ssid(const char* ssid_to_check)
{
    if (check_ssid(ssid_to_check, SSID1, PSWD1))
        return 1;
    else if (check_ssid(ssid_to_check, SSID2, PSWD2))
        return 1;
    else
        return 0;
}

int init_ssid()
{
  int res = -1;
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Setup done");
  
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
      Serial.println("no networks found");
    else
    {
      for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
  
        if (select_ssid(WiFi.SSID(i).c_str()))
        {
          res = 0;
          Serial.println(" Selected");
          break;
        }
        Serial.println(" Skipped");
        delay(10);
      }
    }
    return res;
}
