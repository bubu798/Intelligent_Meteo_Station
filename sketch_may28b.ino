#include <string.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MQ135.h>
#include <RTClib.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "TP-Link_CDE9";
const char* password = "57788821";
String serverUrl = "http://192.168.0.104:3000/";

#define DHTPIN 26
#define DHTTYPE DHT11
#define GAS_SENSOR_PIN 36

MQ135 gasSensor(GAS_SENSOR_PIN);
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);
int row = 1;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  lcd.begin();
  dht.begin();
  Serial.begin(9600);
  lcd.backlight();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  lcd.setCursor(0, 0); 
  lcd.print("Connected to:");
  clearRow(1);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 0);
  delay(5000);
}

void loop() {
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "text/plain");

  String temp = getTemp();
  String hum = getHumidity();
  String ppm = getPPM();

  String stringArray[] = {temp, hum, ppm};
  for (int i = 0; i < 3; i++) {
    displayTime();
    clearRow(row);
    lcd.print(stringArray[i]);
    int httpResponseCode = http.POST(stringArray[i]);
    if (httpResponseCode != HTTP_CODE_OK) {
      Serial.print("HTTP POST failed with code ");
      Serial.println(httpResponseCode);
    }
    delay(1000);
  }
  http.end();
}

String floatToString(const char* before, float value, const char* after) {
  char tempBuffer[50] = "";
  char floatBuffer[10];
  dtostrf(value, 6, 2, floatBuffer);
  strcat(tempBuffer, before);
  strcat(tempBuffer, floatBuffer);
  strcat(tempBuffer, after);
  return String(tempBuffer);
}

String getTemp() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return String("Temp: Error");
  } else {
    return floatToString("Temp: ", t, " C");
  }
}

String getHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return String("Hum: Error");
  } else {
    return floatToString("Hum: ", h, " %");
  }
}

String getPPM() {
  float r = analogRead(GAS_SENSOR_PIN);
  if (isnan(r)) {
    Serial.println("Failed to read from MQ135 sensor!");
    return String("Air: Error");
  } else {
    return floatToString("Air: ", r, " PPM");
  }
}

void clearRow(const int rowNo) {
  lcd.setCursor(0, rowNo);
  for (int i = 0; i < 16; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, rowNo);
}

void displayTime() {
  DateTime now = rtc.now();
  clearRow(0);
  lcd.setCursor(0, 0);
  lcd.print(now.year() % 1000, DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print(' ');
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
}
