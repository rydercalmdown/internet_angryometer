/**
internet_angryometer - arduino components
continuously requests URL and updates servo accordingly
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
#include <Servo.h>


Servo servo;
int lastKnownPosition = 0;
int currentPosition = 0;
bool hasStarted = false;


int reversePosition(int pos) {
  return 180 - pos;
}


void updateServoPosition(int desiredPosition) {
  int writeDelay = 10;
  int bufferDegrees = 2;
  if (desiredPosition == 0) {
    desiredPosition = bufferDegrees;
  }
  if (desiredPosition >= 180) {
    desiredPosition = 180 - bufferDegrees;
  }
  if (desiredPosition > currentPosition) {
    for (currentPosition = lastKnownPosition; currentPosition <= desiredPosition; currentPosition +=1) {
      servo.write(reversePosition(currentPosition));
      delay(writeDelay);
    }  
  } else {
    for (currentPosition = lastKnownPosition; currentPosition >= desiredPosition; currentPosition -=1) {
      servo.write(reversePosition(currentPosition));
      delay(writeDelay);
    }
  }
  lastKnownPosition = currentPosition;
}


void sweepServo() {
  Serial.println("Sweeping servo...");
  updateServoPosition(180);
  delay(1000);
  updateServoPosition(0);
}


void setupSerial() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
}


void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WiFi Name", "WiFi Password");
}


void setupServo() {
  servo.attach(14);
}


void setup() {
  setupSerial();
  setupWifi();
  setupServo();
}


int getDegreesFromServer() {
    WiFiClientSecure client;
    HTTPClient http;
    client.setInsecure();
    String remoteServerUrl = "https://example.com/server_url_for_angryometer_server";
    if (http.begin(client, remoteServerUrl)) {
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          return payload.toInt();
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
    // Return an error degree
    return 500;
}


void runFirstTime() {
  if (hasStarted == false) {
    sweepServo();
    hasStarted = true;
  }
}


void updateMeter() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    runFirstTime();
    int desiredPosition = getDegreesFromServer();
    if (desiredPosition != 500) {
      updateServoPosition(desiredPosition);
    }
  }
}


void loop() {
  updateMeter();
  delay(30000);
}
