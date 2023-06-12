#include <Wire.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>


const char* ssid = "IoT";
const char* password = "coltec2022";

SoftwareSerial loraSerial(15, 16);

void setup() {
  Serial.begin(9600);
  loraSerial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() { 
  if(loraSerial.available() > 1){
    delay(100);
    String input = loraSerial.readString();
    Serial.print(input);
    postNessage(input);
  }
  delay(50);
}

void postNessage(String msg){
  HTTPClient http;
  http.begin(msg);

  http.addHeader("Content-Type", "application/json");
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
    Serial.print(msg);
  }
  else{
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}