#include "IOSub.h"
#include "ValueDec.h"

//playlist generator int
int playC[20], playB[20], playA[20], playS[20];
int beatLength = 50;

//setup tone pin
const int TONE_OUTPUT_PIN = 16;
const int TONE_OUTPUT_PIN2 = 17;
const int TONE_PWM_CHANNEL = 1;
const int TONE_PWM_CHANNEL2 = 2;



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

int pwmFreq(int val) {
  int out = map(val, 0, 24, 0, 1600);
  return out;
}

int pwmSize(int sizeN) {
  if (sizeN > 120) {
    sizeN = 120;
  }
  int out = map(sizeN, 0, 120, 100, 1600);
  return out;
}

void outputGen(int wait, int pause) {
  for (byte j = 0; j < 20; j++) {

    ledcWriteTone(TONE_PWM_CHANNEL, pwmFreq(playC[j]));
    ledcWriteTone(TONE_PWM_CHANNEL2, pwmSize(size1));
    delay(wait - pause);
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
    ledcWriteTone(TONE_PWM_CHANNEL2, 0);
    delay(pause);

    ledcWriteTone(TONE_PWM_CHANNEL, pwmFreq(playB[j]));
    ledcWriteTone(TONE_PWM_CHANNEL2, pwmSize(size2));
    delay(wait - pause);
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
    ledcWriteTone(TONE_PWM_CHANNEL2, 0);
    delay(pause);

    ledcWriteTone(TONE_PWM_CHANNEL, pwmFreq(playA[j]));
    ledcWriteTone(TONE_PWM_CHANNEL2, pwmSize(size3));
    delay(wait - pause);
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
    ledcWriteTone(TONE_PWM_CHANNEL2, 0);
    delay(pause);

    ledcWriteTone(TONE_PWM_CHANNEL, pwmFreq(playS[j]));
    ledcWriteTone(TONE_PWM_CHANNEL2, pwmSize(size0));
    delay(wait - pause);
    ledcWriteTone(TONE_PWM_CHANNEL, 0);
    ledcWriteTone(TONE_PWM_CHANNEL2, 0);
    delay(pause);
  }
}

//Action when receive a highest level
void Act() {
  decodeArray();
  showCode();
  outputGen(250,50);
  Serial.println(".......");
  CleanArray();
}

//mqtt subscription - Main
void mqttSub() {
  Adafruit_MQTT_Subscribe *subscription_name;
  while ((subscription_name = mqtt.readSubscription(1))) {
    if (subscription_name == &sub) {
      Serial.println("New value receive: ");
      Serial.println((char *)sub.lastread);

      //Convert from char to int
      int n = atoi((char *)sub.lastread);

      countNum(n);
      if (size4 == 1) {
        Act();
      }
    }
  }
}
