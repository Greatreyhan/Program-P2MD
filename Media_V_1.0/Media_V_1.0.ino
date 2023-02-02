#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHTPIN D7
#define DHTTYPE DHT11
#define sensor_pin A0  

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600); 
  dht.begin();
  lcd.begin();
}

void loop() {
  float moisture_percentage;
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Suhu : ");
  lcd.setCursor(7, 0);
  lcd.print(t);
  lcd.setCursor(13, 0);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("Kelembapan:");
  lcd.setCursor(12, 1);
  lcd.print(moisture_percentage);
  
  Serial.println(moisture_percentage);
  Serial.println(h);
  Serial.println(t);

  delay(1000);
}
