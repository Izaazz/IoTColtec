#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <SoftwareSerial.h>

#define BME_SCK 4
#define BME_MISO 5
#define BME_MOSI 6
#define BME_CS 7

#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

SoftwareSerial loraSerial(15, 16); // TX, RX

void setup() {
  Serial.begin(9600);
  loraSerial.begin(9600);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    loraSerial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {
  if (! bme.performReading()) {
    loraSerial.println("Failed to perform reading :(");
    return;
  }
  sendMessage();
  delay(4000);
}

void sendMessage(){
  String url = "https://script.google.com/macros/s/AKfycbxCofRAYm2pXUjpQaSAEDoUZKAP-wlDVxNzNh0GLp9h7evkjHqaNGyiYq_bivg4j2z_0Q/exec?UniqueID=ColtecIoT&temperatureC=" + (String)bme.temperature + "&humidity=" + (String)bme.humidity + "&temperatureF=" + (String)(bme.pressure / 100.0);
  loraSerial.print(url);
  Serial.print(url);
}