#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "parameters.h"

#define LED 2
#define BUTTON 13
#define WAIT 100
#define REQUEST_TIMEOUT 5000

#define HOST "smsapi.free-mobile.fr"
#define HOST_CERTIFICATE_FINGERPRINT "3B 5C 64 35 F5 28 BF 1C FA 96 DC E7 65 B1 E1 B3 2E 84 35 77";

const char  host_certificate_fingerprint[] PROGMEM = HOST_CERTIFICATE_FINGERPRINT

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  pinMode(BUTTON, INPUT);
  Serial.begin(74880);
  Serial.println("");
  Serial.println("Connected button");
}

void loop() {
  delay(WAIT);
  if (digitalRead(BUTTON) == LOW) {
    connectToWifi();
    get(HOST, URL_1);
    //get(HOST, URL_2);
    WiFi.mode(WIFI_OFF);
  }
}

void connectToWifi() {

  Serial.println("");
  Serial.print("connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.forceSleepWake();
  WiFi.mode(WIFI_STA);  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED, LOW);
  Serial.println("");
  Serial.println("wifi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}


boolean get(const char* host, const char* url) {
  WiFiClientSecure client;
  boolean status = false;
  Serial.print(">  ");
  Serial.print(host);

  client.setFingerprint(host_certificate_fingerprint);
  client.setTimeout(15000);

  unsigned int retries = 5;
  while(!client.connect(host, 443) && (retries-- > 0)) {
    Serial.print(".");
  }

  if(!client.connect(host, 443)) {
    Serial.println("");
    Serial.println("connection failed");
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

  if (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.print("< ");
    Serial.println(line);
    status = true;
  }
  return status;
}
