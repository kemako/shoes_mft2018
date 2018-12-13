/*
  Arduinoを使用
  PIN3:  スタートスイッチの制御用
  PIN5:  テープLEDの制御用
  PIN9:  左側のファンの制御用
  PIN10: 右側のファンの制御用
*/

#include <Adafruit_NeoPixel.h>
#include <Servo.h>
Servo leftServo;
Servo rightServo;

#define LED_SIG 5    // テープLEDの制御用
#define LED_NUM 50

int button=3;        // スタートスイッチの制御用
int flag = 0;

Adafruit_NeoPixel led = Adafruit_NeoPixel(LED_NUM, LED_SIG, NEO_GRB + NEO_KHZ800);
// 3つめのパラメータは使うLEDテープの制御ICがWS2812ならNEO_KHZ800 / WS2811ならNEO_KHZ400

uint32_t rotateColor(byte WheelPos) {
  if(WheelPos < 85) {
   return led.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return led.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return led.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setup() {
  pinMode(button, INPUT);

  led.begin();
  led.show();   // LEDが一旦全てOFFの状態で反映

  leftServo.attach(9, 300, 2000);     // 左側のファンの設定
  rightServo.attach(10, 300, 2000);   // 右側のファンの設定

  leftServo.writeMicroseconds(700);   // 左側のファンの初期化
  rightServo.writeMicroseconds(700);  // 右側のファンの初期化

  Serial.begin(9600);

  delay(1000);
}


void loop() {
  if((digitalRead(button) == 1)) {
    // LEDが全て緑色に
    for(int i=0; i<led.numPixels(); i++) {
      led.setPixelColor( i, led.Color(0,255,0));
    }
    led.show();
    delay(1000);

    // LEDが全て黄色に
    for(int i=0; i<led.numPixels(); i++) {
      led.setPixelColor( i, led.Color(255,241,0));
    }
    led.show();
    delay(1000);

    // LEDが全て赤色に
    for(int i=0; i<led.numPixels(); i++) {
      led.setPixelColor( i, led.Color(255,0,0));
    }
    led.show();
    delay(1000);

    // ファン動かす
    Serial.println("Go");
    leftServo.writeMicroseconds(1400);
    rightServo.writeMicroseconds(1400);
    delay(1500);

    // ファン止める
    leftServo.writeMicroseconds(700);
    rightServo.writeMicroseconds(700);
    delay(500);

    // LEDが虹色に光りながら点滅
    for(int k=0; k<10; k++) {
      for(int j=0; j <10; j++) {
       for(int i=0; i < led.numPixels(); i++) {
         led.setPixelColor(i, rotateColor((((i) * 256 / led.numPixels()) + j) & 255));
       }
       led.show();
       delay(50);

       for(int i=0; i<led.numPixels(); i++) {
         led.setPixelColor( i, led.Color(0,0,0));
       }
       led.show();
       delay(50);
      }
    }

    // LEDオフ
    for(int i=0; i<led.numPixels(); i++) {
      led.setPixelColor( i, led.Color(0,0,0));
    }
    led.show();
    delay(1000);
  } else {
    // ファン止める
    leftServo.writeMicroseconds(700);
    rightServo.writeMicroseconds(700);
    delay(5000);

    // LEDオフ
    for(int i=0; i<led.numPixels(); i++) {
      led.setPixelColor( i, led.Color(0,0,0));
    }
    led.show();
    delay(1000);
  }
}
