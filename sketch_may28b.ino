#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>
#include <string.h>
#include <LiquidCrystal_I2C.h>  

int row = 1;
#define DHTPIN 26 
#define DHTTYPE DHT11

#define GAS_SENSOR_PIN 36 

RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);// Set the LCD I2C address (0x27) and the LCD size (16x2)


void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  dht.begin();
  lcd.begin(); 
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void loop() {
  display();
}

String floatToString(const char* before, float value, const char* after) {
  char tempBuffer[50] = "";
  char floatBuffer[50];
  dtostrf(value, 6, 2, floatBuffer);

  strcat(tempBuffer, before);
  strcat(tempBuffer, floatBuffer);
  strcat(tempBuffer, after);

  return String(tempBuffer);
}

String getTemp() {
  return floatToString("Temp: ", dht.readTemperature(), " C");
}

String getHumidity() {
  return floatToString("Hum: ", dht.readHumidity(), " %");
}

String getPPM() {
  return floatToString("Air: ", gasSensor.getPPM(), " PPM");
}

void clearRow(const int rowNo) {
  lcd.setCursor(0, row);
  lcd.println("                ");
  lcd.setCursor(0, row);
}

void displayTime() {
  DateTime now = rtc.now();
  clearRow(0);
  lcd.setCursor(0, 0);
  lcd.print(now.year()%1000);
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.day());
  lcd.print(' ');
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
}

void display() {
  displayTime();
  clearRow(1);
  lcd.println(getTemp());
  delay(1000);
  displayTime();
  clearRow(1);
  lcd.println(getHumidity());
  delay(1000);
  displayTime();
  clearRow(1);
  Serial.println(getPPM());
  delay(1000);
}
