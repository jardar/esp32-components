#include <Arduino.h>
#include <WiFi.h>
#include <esp_task_wdt.h>

#include "inc/my_wifi.h"

// === watchdog ===============
#define WDT_TIMEOUT 10  // 10 seconds

typedef enum {
  UNKNOWN,
  RUNNING,
  CONNECTING,
  CONNECTED,
  DISCONNECTED,
} WifiReconnectState;

WifiReconnectState wifiReconnectState = UNKNOWN;

TaskHandle_t wifiReconnectTaskHandle;
void wifiReconnectTask(void *pvParam);

void setup() {
  Serial.begin(115200);
  Serial.println(F("\nx03_ESP32CamWiFiWatchdog Booting..."));

  // == WiFi first connect
  if (connectWifi()) {
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(F("cannot connect"));
    esp_restart();
  };
  // == WatchDog 初始化
  esp_task_wdt_init(WDT_TIMEOUT, true);

  /*======= WIFI RECONNECT =======*/
  xTaskCreatePinnedToCore(wifiReconnectTask, "wifiReconnectTask", 100000, NULL,
                          0,                           // Priotity 0
                          &wifiReconnectTaskHandle, 0  // Core 0
  );
  delay(250);

  //* 印出 ESP32 Heap 和 PSRAM 的資訊
  log_d("Total heap: %d", ESP.getHeapSize());
  log_d("Free heap: %d", ESP.getFreeHeap());
  log_d("Total PSRAM: %d", ESP.getPsramSize());
  log_d("Free PSRAM: %d", ESP.getFreePsram());
}
// int tryRun = 0;
void loop() {
  // Serial.println(wifiReconnectState);
  // delay(1000);
  // tryRun++;
  // if (tryRun % 10 == 0) {
  //   WiFi.disconnect();
  //   while (1) {
  //     Serial.print(String("@") + " ");
  //   }
  // }

  // delay(10);
}

// === wifi task with watchdog ===========
void wifiReconnectTask(void *pvParam) {
  wl_status_t wifiState,
      wifiPreState =
          WL_CONNECTED;  // 上次狀態必須是已連線，這樣才有機會 reconnect
  unsigned long lastTick = 0, now = 0;
  const long periodMs = 1000;

  esp_task_wdt_add(NULL);

  while (1) {
    wifiState = WiFi.status();

    if (wifiState != WL_CONNECTED && wifiPreState == WL_CONNECTED) {
      wifiReconnectState = DISCONNECTED;
      esp_task_wdt_reset();
      WiFi.disconnect();

      esp_task_wdt_reset();
      wifiReconnectState = CONNECTING;
      WiFi.reconnect();

    } else if (wifiState == WL_CONNECTED && wifiPreState == WL_CONNECTED) {
      wifiReconnectState = RUNNING;
      esp_task_wdt_reset();
      vTaskDelay(1000 / portTICK_PERIOD_MS);

    } else if (wifiState == WL_CONNECTED && wifiPreState != WL_CONNECTED) {
      wifiReconnectState = CONNECTED;
      esp_task_wdt_reset();

    } else {
      now = xTaskGetTickCount();
      if (now - lastTick > periodMs) {
        static int i = 0;
        Serial.print(String(++i) + " ");
        lastTick = now;
      }
    }  // else

    wifiPreState = wifiState;
  }  // while
}
