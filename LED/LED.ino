// LEDを光らせるためのライブラリ
#include <Adafruit_NeoPixel.h>

// 出力ピン番号
#define LED_OUTPIN 6
// LEDの連結数
#define LED_NUM 8

// LEDを操作するクラスのインスタンス作成
Adafruit_NeoPixel LED = Adafruit_NeoPixel(LED_NUM, LED_OUTPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  LED.begin() ;                 // 初期化
  LED.setBrightness(127) ;       // 明るさの設定(0-255)
  for (int i = 0; i < LED_NUM; i++) { // LEDの数分明るさを設定
    LED.setPixelColor(i, 0, 0, 0); // 色の設定(LEDの番号, r, g, b)
  }
  LED.show() ;                  // 色をLEDに反映
}

float hue = 0.0;

void loop() {
  float col[3];
  
  hue += 0.0001;
  if (hue >= 1.0) hue = 0.0;

  hsv2rgb(hue, 1.0, 1.0, col);

  for (int j = 0; j < LED_NUM; j++) {
    LED.setPixelColor(j, (int)(col[0] * 255), (int)(col[1] * 255), (int)(col[2] * 255));
  }
  LED.show();
}

// reference from
// https://gist.github.com/postspectacular/2a4a8db092011c6743a7
float fract(float x) { return x - int(x); }
float mix(float a, float b, float t) { return a + (b - a) * t; }
float* hsv2rgb(float h, float s, float b, float* rgb) {
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
}
