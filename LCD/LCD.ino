#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
  lcd.begin();

}
void loop()
{
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Suhu Udara : 31.32C");
  lcd.setCursor(0, 1);
  lcd.print("Kelembapan : 65.41%");
  lcd.setCursor(0, 2);
  lcd.print("Penyinaran : 4J 24M");
}
