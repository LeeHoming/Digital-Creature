

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi.h>

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "1945X1"
#define AIO_USERNAME2  "Looomi"
#define AIO_KEY  "aio_nCJM06aUVhP1Ph0FsDEwlGbuC27r"
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Publish Feeds ***************************************/
Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ct3-pots");
//Adafruit_MQTT_Publish pubA = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ct3-pota");
//Adafruit_MQTT_Publish pubB = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ct3-potb");
//Adafruit_MQTT_Publish pubC = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ct3-potc");

/****************************** Subscribe Feeds *************************************/
Adafruit_MQTT_Subscribe sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/lhms");
//Adafruit_MQTT_Subscribe subA = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/lhma");
//Adafruit_MQTT_Subscribe subB = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/lhmb");
//Adafruit_MQTT_Subscribe subC = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/lhmc");

/******************************* MQTT Network ***************************************/
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }

  Serial.println("MQTT Connected!");
}


//Pin setting
#define POT_PIN A2
int led = 13;
int motionSensor = 27;
int val;

int current = 0;
int BioCur = 0;

//time counter

#include "Decode.h"


void setup() {
  Serial.begin(9600);
  //wifi define
  WiFi.disconnect();
  delay(3000);
  Serial.print("Account: ");
  Serial.println(AIO_USERNAME);
  Serial.println("START");
  //WiFi.begin("ACCD", "tink1930");
  WiFi.begin("Tenda_CFA0F8", "room4776");
  //WiFi.begin("Bell Pasadena WiFi", "");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));

  //Pin define
  pinMode(led, OUTPUT);
  //pinMode(motionSensor, INPUT);

  //subscribe
  mqtt.subscribe(&sub);

  //tone
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
  ledcAttachPin(TONE_OUTPUT_PIN2, TONE_PWM_CHANNEL2);
  ledcSetup(1, 3000, 8);
  ledcSetup(2, 3000, 8);
}


void loop() {
  MQTT_connect();

  //Analog converter
  current = analogRead(POT_PIN);
  //Serial.println(current);
  if (current > 200) {
    val = 1;
  } else {
    val = 0;
  }
  //Serial.println(val);

  //read and display val on led
  //val = digitalRead(button);
  if (val == HIGH)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
  SecondCounter();

  //information reciver
  mqttSub();
}
