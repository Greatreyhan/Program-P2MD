#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DHTPIN D7
#define DHTTYPE DHT11
#define sensor_pin A0  
#define oneWireBus 13    

#define WIFI_SSID "infinergy"
#define WIFI_PASSWORD "okeokeoke"
#define API_KEY "AIzaSyCHUT44crImCjPfvKQaMhCaNjT7d6pxG0U"
#define DATABASE_URL "kknundipxrnb-default-rtdb.firebaseio.com" 
#define USER_EMAIL "kknundip@gmail.com"
#define USER_PASSWORD "rnb2022jaya"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  sensors.begin();
  lcd.begin();
  Firebase.begin(&config, &auth);
}

void loop()
{
  float moisture_percentage;
  sensors.requestTemperatures(); 
  float temperature = sensors.getTempCByIndex(0);

  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Suhu : ");
  lcd.setCursor(7, 0);
  lcd.print(temperature);
  lcd.setCursor(13, 0);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("Kelembapan:");
  lcd.setCursor(12, 1);
  lcd.print(moisture_percentage);
  Serial.println(temperature);
  Serial.println(moisture_percentage);
  
  if (Firebase.ready())
  {
     if (Firebase.RTDB.setFloat(&fbdo, "Monitoring/Variabel/Media/2/Suhu", temperature)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "Monitoring/Variabel/Media/2/Kelembapan", moisture_percentage)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  delay(1000);
}
