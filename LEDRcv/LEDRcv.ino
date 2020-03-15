// LEDを光らせるためのライブラリ
#include <Adafruit_NeoPixel.h>

// 出力ピン番号
#define LED_OUTPIN    6
// LEDの連結数
#define LED_NUM        8

// LEDを操作するクラスのインスタンス作成
Adafruit_NeoPixel LED = Adafruit_NeoPixel(LED_NUM, LED_OUTPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // シリアル通信の初期化。PCとArduinoで通信するために使う
  // 転送速度（ボーレート）：119200bps
  Serial.begin(119200);
  
  LED.begin() ;                 // 初期化
  LED.setBrightness(127) ;       // 明るさの設定(0-255)
  for(int i=0; i<LED_NUM; i++){ // LEDの数分明るさを設定
    LED.setPixelColor(i,0,0,0); // 色の設定(LEDの番号, r, g, b)
  }
  LED.show() ;                  // 色をLEDに反映
}

void loop() {
  int val;

  // PCからシリアルデータが来ているかチェック
  // valの値は0-180または255
  // 0-180:色相(HSV色空間におけるHue)の1/2。2倍して使う
  // 255:光らせない
  if (Serial.available() > 0) {
    val = Serial.read();  // 来てる場合はデータをリード
  } else {
    return ;              // 来てない場合はそのままリターン
  }

  // 255だったら消灯
  if (val == 255) {
    for(int j=0; j<LED_NUM; j++){
      LED.setPixelColor(j,0,0,0);
    }

  // 255でない場合は0-180の値で得られた色相を２倍してLEDに反映
  } else {
    val *= 2;
    byte r, g, b;

    // HSV色空間からRGB色空間に変換
    // val:0-360を色相として与え、RGBの値を算出
    HSV_to_RGB(float(val), 100, 100, &r, &g, &b);
    // LEDに色を反映
    for(int j=0; j<LED_NUM; j++){
      LED.setPixelColor(j,r,g,b);
    }
  }
  LED.show();
}

// HSV色空間からRGB色空間に変換
// 以下から取得
// https://gist.github.com/hdznrrd/656996
void HSV_to_RGB(float h, float s, float v, byte *r, byte *g, byte *b)
{
  int i,f,p,q,t;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;
  
  if(s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v*255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      *r = round(255*v);
      *g = round(255*t);
      *b = round(255*p);
      break;
    case 1:
      *r = round(255*q);
      *g = round(255*v);
      *b = round(255*p);
      break;
    case 2:
      *r = round(255*p);
      *g = round(255*v);
      *b = round(255*t);
      break;
    case 3:
      *r = round(255*p);
      *g = round(255*q);
      *b = round(255*v);
      break;
    case 4:
      *r = round(255*t);
      *g = round(255*p);
      *b = round(255*v);
      break;
    default: // case 5:
      *r = round(255*v);
      *g = round(255*p);
      *b = round(255*q);
    }
}

