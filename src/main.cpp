#include <Arduino.h>
#include <FastLED.h>

const int NUM_LEDS = 16;      // 填入 LED 的數量取代 #1
const uint8_t DATA_PIN = 33;  // 填入 WS2812 通訊接腳號碼取代 #2

// 定義 LED 陣列
CRGB ws2812[NUM_LEDS];
/*================== FASTLED =================*/
// http://fastled.io/docs/3.1/class_c_fast_l_e_d.html#a3adb23ec5f919524928d576002cb45de
// Colors definition for WS2812
#define BLACK CRGB(0, 0, 0)        // Black
#define WHITE CRGB(255, 255, 255)  // White
#define RED CRGB(255, 0, 0)        // Red
#define ORANGE CRGB(255, 127, 0)   // Orange
#define YELLOW CRGB(255, 255, 0)   // Yellow
#define GREEN CRGB(0, 255, 0)      // Green
#define BLUE CRGB(0, 0, 255)       // Blue
#define INDIGO CRGB(0, 255, 255)
#define VIOLET CRGB(127, 0, 255)  // Violet

CRGB x02_colors[9] = {RED,    ORANGE, YELLOW, GREEN, BLUE,
                      INDIGO, VIOLET, WHITE,  BLACK};

void blink(const int8_t blink_count, const TickType_t delayTick,
           const CRGB color);
void x02_testLED();

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(ws2812,
                                      NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(10);

  x02_testLED();
  for (byte i = 0; i < 10; i++) {
    FastLED.showColor(RED);

    delay(500);
    FastLED.showColor(BLACK);

    delay(500);
  }

  blink(10, 500, ORANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
}

/**========================================================================
 **                           blink()
 *?  系統運行狀態的指示
 *@param blink_count const int8_t
 *@param color       CRGB
 *========================================================================**/
void blink(const int8_t blink_count,  // 閃爍次數
           const TickType_t delayTick,
           const CRGB color)  // WS2812 顏色
{
  uint8_t state = LOW;
  for (int x = 0; x < (blink_count << 1); ++x) {
    if (state ^= HIGH) {
      ws2812[0] = color;
    } else {
      ws2812[0] = BLACK;
    }
    FastLED.show();
    delay(delayTick);
  }
}

/**========================================================================
 **                           testLED
 *?  顏色顯示測試
 *========================================================================**/
void x02_testLED() {
  for (int i = 0; i < 9; i++) {
    blink(5, 100, x02_colors[i]);
    delay(1450);
  }
}