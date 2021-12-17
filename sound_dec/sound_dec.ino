

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi.h>

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "Your User Name"
#define AIO_USERNAME2  "Pair User Name"
#define AIO_KEY  "Your Key"
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Publish Feeds ***************************************/
Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lhms");
//Adafruit_MQTT_Publish pubA = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lhma");
//Adafruit_MQTT_Publish pubB = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lhmb");
//Adafruit_MQTT_Publish pubC = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lhmc");

/****************************** Subscribe Feeds *************************************/
Adafruit_MQTT_Subscribe sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/ct3-pots");
//Adafruit_MQTT_Subscribe subA = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/ct3-pot");
//Adafruit_MQTT_Subscribe subA = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/ct3-pot");
//Adafruit_MQTT_Subscribe subA = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME2 "/feeds/ct3-pot");

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
int led = 13;
int button = 14;
int val;

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
  WiFi.begin("WifiName", "WifiPassword");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));

  //Pin define
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

  //subscribe
  mqtt.subscribe(&sub);

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  // Initialize all pixels to 'off'
  strip.begin();
  strip.setBrightness(40);
  strip.show();

}


void loop() {
  MQTT_connect();


  //read and display val on led
  val = digitalRead(button);
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
