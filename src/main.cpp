#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Adafruit_BMP280.h>
#include "secrets.h"

//Define Firebase Data objects
FirebaseData fbdo;
FirebaseData firebaseData;
FirebaseJson updateSensorValue;
FirebaseJson json;

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C

const int STATUS_PIN = 33; 
bool estadoLed = false;  
float Temperatura = 0;
float Presion = 0;
float Altitud = 0;

void setup() {
  Serial.begin(9600);
  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, LOW);
  
  Serial.println(F("BMP280 Forced Mode Test."));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);                 
}

void enviarDatosFirebase() 
{
  estadoLed = !estadoLed; 
  digitalWrite(STATUS_PIN, estadoLed);
  
  updateSensorValue.clear();
  updateSensorValue.add("Temperatura", Temperatura);
  updateSensorValue.add("Presion", Presion);
  updateSensorValue.add("Altitud", Altitud);
  updateSensorValue.add("Weather", json); 
  
  Firebase.RTDB.updateNode(&fbdo, "/Weather", &updateSensorValue);
}

void loop() {
  // must call this to wake sensor up and get new measurement data
  // it blocks until measurement is complete
  if (bmp.takeForcedMeasurement()) {
    // can now print out the new measurements
    Serial.print(F("Temperature = "));
    Temperatura = bmp.readTemperature();
    Serial.print(Temperatura);
    Serial.println(" °C");

    float pressurePa = bmp.readPressure();
    float pressureMmHg = pressurePa / 133.322368;

    Serial.print(F("Presion = "));
    Presion = pressureMmHg;
    Serial.print(Presion, 2);
    Serial.println(" mmHg");

    Serial.print(F("Altitud = "));
    Altitud = bmp.readAltitude(1013.25); /* Adjusted to local forecast! */
    Serial.print(Altitud); 
    Serial.println(" m");
    enviarDatosFirebase();
    delay(2000);
  } else {
    Serial.println("Forced measurement failed!");
  }
}