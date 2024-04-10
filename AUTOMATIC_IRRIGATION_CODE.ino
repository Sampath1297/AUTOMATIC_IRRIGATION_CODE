#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int soilMoisturePin = A0;
const int rainSensorPin = 8;  // Digital pin for rain sensor
const int buzzerPin = 9;      // Digital pin for buzzer
const int relayPin = 7;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int soilMoistureThreshold = 700; // Adjust this value as needed
int rainThreshold = 500;         // Adjust this value based on your rain sensor readings

void setup() {
  Serial.begin(9600); // Initialize the Serial Monitor
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure the water pump is initially off

  pinMode(rainSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  lcd.setCursor(6, 0);
  lcd.print(temperature);
  lcd.setCursor(10, 0);
  lcd.print("C");

  lcd.setCursor(10, 1);
  lcd.print(humidity);
  lcd.setCursor(14, 1);
  lcd.print("%");

  int soilMoisture = analogRead(soilMoisturePin);
  int rainSensorValue = digitalRead(rainSensorPin);

  if (soilMoisture < soilMoistureThreshold) {
    digitalWrite(relayPin, HIGH); // Turn off the water pump
    Serial.println("Pump Status: OFF");
  } else {
    digitalWrite(relayPin, LOW); // Turn on the water pump
    Serial.println("Pump Status: ON");
  }

  if (rainSensorValue > rainThreshold) {
    digitalWrite(buzzerPin, HIGH); // Activate the buzzer
    Serial.println("Rain Detected! Buzzer ON");
  } else {
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    Serial.println("No Rain. Buzzer OFF");
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.print(", Rain Sensor: ");
  Serial.println(rainSensorValue);

  delay(1000); // Adjust the delay time as needed
}
