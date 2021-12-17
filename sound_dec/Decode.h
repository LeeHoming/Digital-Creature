#include "IOSub.h"
#include "ValueDec.h"

//playlist generator int
int playC[20], playB[20], playA[20], playS[20];
int beatLength = 50;

//NeoPixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NeoPIN 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NeoPIN, NEO_GRB + NEO_KHZ800);

//color setting
uint16_t hue_c = 1200, hue_b = 10600, hue_a = 29700, hue_s = 55600;

//num of each line of LED
int eachLine;

void showCode() {
  for (byte i = 0; i < 20; i++) {
    Serial.print("【round ");
    Serial.print(i + 1);
    Serial.print("】 ");
    Serial.print(playC[i]);
    Serial.print(", ");
    Serial.print(playB[i]);
    Serial.print(", ");
    Serial.print(playA[i]);
    Serial.print(", ");
    Serial.print(playS[i]);
    Serial.println(". ");
  }
}


void decodeArray() {
  //clear playlist
  for (byte j = 0; j < 20; j++) {
    playC[j] = 0;
    playB[j] = 0;
    playA[j] = 0;
    playS[j] = 0;
  }
  Serial.println("Playlist Clear!");

  //decode array from front to rear
  for (byte i = 0; i < (sizeof(SubValue) / sizeof(SubValue[0])); i++) {
    //Go through the whole array and ingnore 0
    if (!SubValue[i] == 0) {
      if (SubValue[i] == 1) {
        for (byte j = 0; j < 20; j++) {
          if (random(0, 20) < T_c) {
            playC[j]++;
          }
        }
      }

      if (SubValue[i] == 2) {
        for (byte j = 0; j < 20; j++) {
          if (random(0, 20) < T_b) {
            playB[j]++;
          }
        }
      }

      if (SubValue[i] == 3) {
        for (byte j = 0; j < 20; j++) {
          if (random(0, 20) < T_a) {
            playA[j]++;
          }
        }
      }

      for (byte j = 0; j < 20; j++) {
        if (random(0, 20) < T_s) {
          playS[j]++;
        }
      }
    }
  }
}

void light(int C, int B, int A, int S, int Wait) {


  strip.setPixelColor(0, strip.ColorHSV(hue_c, size1*10, C * 20));
  strip.setPixelColor(eachLine, strip.ColorHSV(hue_b, size2*10, B * 20));
  strip.setPixelColor(eachLine * 2, strip.ColorHSV(hue_a, size3*10, A * 20));
  strip.setPixelColor(eachLine * 3, strip.ColorHSV(hue_s, (size0 + size2+ size4)*10, S * 15));

  strip.show();
  delay(Wait);

  for (int i = 1; i < eachLine; i++) {
    strip.setPixelColor(i, strip.getPixelColor(i - 1));
    strip.setPixelColor(i + eachLine, strip.getPixelColor(i - 1 + eachLine));
    strip.setPixelColor(i + eachLine * 2, strip.getPixelColor(i - 1 + eachLine * 2));
    strip.setPixelColor(i + eachLine * 3, strip.getPixelColor(i - 1 + eachLine * 3));
  }

}

void outputGen(int wait) {

  for (byte j = 0; j < 20; j++) {
    light(playC[j], playB[j], playA[j], playS[j], wait);
  }
}

//Action when receive a highest level
void Act() {
  //Each Line represent different array
  eachLine = strip.numPixels() / 4;

  decodeArray();
  showCode();
  outputGen(1000);
  
  //Clean Light
  for (int i = 0; i < eachLine; i++) {
    light(0, 0, 0, 0, 1000);
  }
  
  Serial.println(".......");
  CleanArray();
}

//mqtt subscription - Main
void mqttSub() {
  Adafruit_MQTT_Subscribe *subscription_name;
  while ((subscription_name = mqtt.readSubscription())) {
    if (subscription_name == &sub) {
      Serial.println("New value receive: ");
      Serial.println((char *)sub.lastread);

      countNum(atoi((char *)sub.lastread));
      if (size4 == 1) {
        Act();
      }
    }
  }
}
