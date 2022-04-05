#include "inc/my_wifi.h"

#include <WiFi.h>

#include "inc/my_setting.h"

int connectWifi() {
  Serial.print("Connecting to ");
  Serial.println(SSID1);

  unsigned long wifiStart = millis();
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(DEVICENAME);

  WiFi.begin(SSID1, WiFiPASS1);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID1);

    if ((millis() - wifiStart) > 10 * 1000)  // 10秒 timeout , 試第二組
      break;
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    wifiStart = millis();
    WiFi.begin(SSID2, WiFiPASS2);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(SSID2);
      //等待 10秒
      if ((millis() - wifiStart) > 10000) {
        return 0;
      }

      delay(500);
      Serial.print("#");
    }
  }

  return 1;
}