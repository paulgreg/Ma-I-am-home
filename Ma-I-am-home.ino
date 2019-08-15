#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "parameters.h"

// default LED on board is 2
// I’m using here GPIO 5 to connect a LED with a 220 ohm resistor
#define LED 5
#define BUTTON 13
#define WAIT 100
#define REQUEST_TIMEOUT 5000

const char host_certificate_fingerprint[] PROGMEM = HOST_CERTIFICATE_FINGERPRINT

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(74880);
  while (!Serial);
  Serial.println("Connected button");
  blinkOnce();

  if (connectToWifi()) {
    for (int i = 0; i < URLS_SIZE; i++) {
      blinkOnce();
      get(HOST, URLS[i]);
      blinkOnce();
    }
    WiFi.mode(WIFI_OFF);
    longBlinkOnce();
  }

  Serial.println("Sleeping");
  ESP.deepSleep(0);
}

void loop() {
  // sleeping so wont get here
}

boolean connectToWifi() {
  Serial.print("\nconnecting to ");
  Serial.println(WIFI_SSID);

  WiFi.forceSleepWake();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned int retries = 50;
  while (WiFi.status() != WL_CONNECTED && (retries-- > 0)) {
    blinkOnce();
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWifi connection failed");
    return false;
  }
  Serial.println("");
  Serial.println("wifi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  return true;
}

boolean get(const char* host, const char* url) {
  WiFiClientSecure client;
  boolean status = false;
  Serial.print(">  ");
  Serial.print(host);

  client.setFingerprint(host_certificate_fingerprint);
  client.setTimeout(10000);

  unsigned int retries = 5;
  while(!client.connect(host, 443) && (retries-- > 0)) {
    blinkOnce();
    Serial.print(".");
  }

  if(!client.connect(host, 443)) {
    Serial.println("\nRequest failed");
    return false;
  }

  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    delay(1);
    if (millis() - timeout > REQUEST_TIMEOUT) {
      Serial.println("client Timeout !");
      client.stop();
      return false;
    }
  }

  return client.connected();
}

void blinkOnce() {
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}

void longBlinkOnce() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}
