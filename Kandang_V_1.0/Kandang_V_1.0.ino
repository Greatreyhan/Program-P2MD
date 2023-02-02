#include "DHT.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DHTPIN D7
#define DHTTYPE DHT11 

#define RELAY1_AKTUATOR D0
#define RELAY2_AKTUATOR D1
#define RELAY_LAMPU D6
#define RELAY_POMPA D8

#define WIFI_SSID "infinergy"
#define WIFI_PASSWORD "okeokeoke"
#define API_KEY "AIzaSyCHUT44crImCjPfvKQaMhCaNjT7d6pxG0U"
#define DATABASE_URL "kknundipxrnb-default-rtdb.firebaseio.com" 
#define USER_EMAIL "kknundip@gmail.com"
#define USER_PASSWORD "rnb2022jaya"

#define DELAY_AKTUATOR 100000
#define DELAY_POMPA 10000

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

DHT dht(DHTPIN, DHTTYPE);

int intValue = 0;

void setup()
{

  Serial.begin(115200);

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
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  dht.begin();
  Firebase.begin(&config, &auth);

  pinMode(RELAY1_AKTUATOR, OUTPUT);
  pinMode(RELAY2_AKTUATOR, OUTPUT);
  pinMode(RELAY_LAMPU, OUTPUT);
  pinMode(RELAY_POMPA, OUTPUT);
}

void loop()
{
  Reset();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (Firebase.ready())
  {
     if (Firebase.RTDB.setFloat(&fbdo, "Monitoring/Variabel/Ruangan/Suhu", temperature)){
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "Monitoring/Variabel/Ruangan/Kelembapan", humidity)){
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.getInt(&fbdo, "/Spray/Value/Turn")) {
      if (fbdo.dataType() == "int") {
        intValue = fbdo.intData();
        Serial.println(intValue);
        if(intValue == 1){
          Naik();
          Semprot();
          Reset();
          Turun();
        }
        else{
          Reset();
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }
  delay(1000);
}

void Naik(){
  digitalWrite(RELAY1_AKTUATOR, HIGH);
  digitalWrite(RELAY2_AKTUATOR, LOW);
  delay(DELAY_AKTUATOR);
}

void Turun(){
  digitalWrite(RELAY1_AKTUATOR, LOW);
  digitalWrite(RELAY2_AKTUATOR, HIGH);
  delay(DELAY_AKTUATOR);
}

void Semprot(){
  digitalWrite(RELAY_POMPA, HIGH);
  delay(DELAY_POMPA);
  digitalWrite(RELAY_POMPA, LOW);
}

void Reset(){
  digitalWrite(RELAY1_AKTUATOR, LOW);
  digitalWrite(RELAY2_AKTUATOR, LOW);
  digitalWrite(RELAY_POMPA, LOW);
  digitalWrite(RELAY_LAMPU, LOW);
}
