#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ezButton.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* ssid = "ROBOTIIK";
const char* password = "81895656";

// Endpoint API
const char* serverName = "https://wasiatd-kecmlivutq-et.a.run.app/addDataIOT";

// Timer for sending data every 30 seconds (30000 ms)
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

RTC_DS3231 rtc;

#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4095.0 // ADC resolution for ESP32 (0-4095)
#define TEMP_PIN       33 // ESP32 pin GPIO33 connected to LM35

#define LIGHT_PIN      32 // ESP32 pin GPIO32 connected to light sensor
#define MOISTURE_PIN   34
#define RELAY_PIN      14
#define MODE_SETTER    26
#define BUZZER_PIN     2  // An indicator buzzer for connection status

ezButton timeSetter(25);

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

DateTime relayActivationTime; // Store the last relay activation time

int pill = 1;
int input_sblm = HIGH;
int tepi_naik = HIGH;
unsigned long waktu_debouncing = 0;
unsigned long delay_debouncing = 40;
int input = LOW;
int kondisi_sblm = HIGH;

struct Time {
  int hour;
  int minute;
  int second;
};

Time alarmTime = {9, 13, 0};

unsigned long previousMillis = 0;
unsigned long interval = 3000; // interval untuk mengganti tampilan (ms)
int displayState = 0;

void setup() {
  Serial.begin(115200);
  timeSetter.setDebounceTime(20);
  Wire.begin(22, 23); // Setting wire (22 untuk SDA dan 23 untuk SCL)

  lcd.begin();

  rtc.begin();
  esp_sleep_enable_timer_wakeup(10 * 3600000000);

  pinMode(MOISTURE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MODE_SETTER, INPUT_PULLUP);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);

  // WiFi Connection
  Serial.println();
  Serial.println();
  Serial.print("Connecting to Wi-Fi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Flash the buzzer to indicate successful connection
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Timer set to 30 seconds (timerDelay variable), it will take 30 seconds before publishing the first reading.");
}

void loop() {
  timeSetter.loop();
  DateTime now = rtc.now();
  int kondisi = digitalRead(MODE_SETTER);

  if (kondisi != kondisi_sblm) {
    waktu_debouncing = millis();
  }

  if ((millis() - waktu_debouncing) > delay_debouncing) {
    if (kondisi != input) {
      input = kondisi;
    }
  }

  kondisi_sblm = kondisi;

  if (input == LOW && input_sblm == HIGH) {
    input_sblm = input;
  } else if (input == HIGH && input_sblm == LOW) {
    input_sblm = input;
    tepi_naik = LOW;
  }

  if (pill == 1) {
    mainDisplay();
    if (tepi_naik == LOW) {
      pill++;
      lcd.clear();
    }
  } else if (pill == 2) {
    hourSetter();
    if (tepi_naik == LOW) {
      pill++;
      lcd.clear();
    }
  } else if (pill == 3) {
    minuteSetter();
    if (tepi_naik == LOW) {
      pill = 1;
      lcd.clear();
    }
  }
  tepi_naik = HIGH;

  if (alarmTime.hour > 23) alarmTime.hour = 0;
  if (alarmTime.minute > 59) alarmTime.minute = 0;

  if (now.hour() == 19) {
    esp_light_sleep_start();
  }

  delay(10);

  // Send HTTP POST request every 30 seconds
  if ((millis() - lastTime) > timerDelay) {
    sendHTTP(now);
    lastTime = millis();
  }
}

void mainDisplay() {
  Serial.println("Mode idle deteksi kelembapan");
  int moistureValue = 100 - analogRead(MOISTURE_PIN) * 100 / 4095;
  int lightValue = analogRead(LIGHT_PIN);
  int tempValue = analogRead(TEMP_PIN);
  float milliVolt = tempValue * (ADC_VREF_mV / ADC_RESOLUTION);
  float tempC = milliVolt / 10;

  // Menampilkan RTC pada variable now
  DateTime now = rtc.now();

  // Display time in the first row
  displayTime(now);

  // Alternately display humidity, light, and temperature in the second row
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    displayState = (displayState + 1) % 3; // Cycle through 0, 1, 2
  }

  switch (displayState) {
    case 0:
      displayHumidity(moistureValue);
      break;
    case 1:
      displayLight(lightValue);
      break;
    case 2:
      displayTemperature(tempC);
      break;
  }

  if (now.hour() == alarmTime.hour) {
    if (now.minute() == alarmTime.minute) {
      if (now.second() == alarmTime.second) {
        digitalWrite(RELAY_PIN, HIGH);
        relayActivationTime = now; // Store activation time
        delay(7000);
        digitalWrite(RELAY_PIN, LOW);
        sendRelayActivationTime(now);
      }
    }
  }

  // Trigger relay if humidity is low
  if (moistureValue < 20) {
    digitalWrite(RELAY_PIN, HIGH);
    relayActivationTime = now; // Store activation time
    delay(7000);
    digitalWrite(RELAY_PIN, LOW);
    sendRelayActivationTime(now);
  }

  // Print the sensor values to the Serial Monitor
  Serial.print("Moisture: ");
  Serial.print(moistureValue);
  Serial.print("%, Light: ");
  Serial.print(lightValue);
  Serial.print(", Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");

  delay(10);
}

void hourSetter() {
  Serial.println("Mode mengatur jam");
  lcd.setCursor(0, 0);
  lcd.print("Waktu penyiraman");
  lcd.setCursor(5, 1);
  lcd.print(">");
  lcd.print(alarmTime.hour);
  lcd.print(" :");
  lcd.print(alarmTime.minute);

  adjustTime("jam");
}

void minuteSetter() {
  Serial.println("Mode mengatur menit");
  lcd.setCursor(0, 0);
  lcd.print("Waktu penyiraman");
  lcd.setCursor(6, 1);
  lcd.print(alarmTime.hour);
  lcd.print(" :");
  lcd.print(alarmTime.minute);
  lcd.print("<");

  adjustTime("menit");
}

void displayTime(DateTime now) {
  lcd.setCursor(0, 0);
  lcd.print("Jam : ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());
}

void displayHumidity(int sensorValue) {
  lcd.setCursor(0, 1);
  lcd.print("Klmbpn : ");
  lcd.print(sensorValue);
  lcd.print(" %");
}

void displayLight(int lightValue) {
  int lightPercentage = (1 - (lightValue / 4095.0)) * 100;
  lcd.setCursor(0, 1);
  lcd.print("Cahaya : ");
  lcd.print(lightPercentage);
  lcd.print(" %");
}

void displayTemperature(float tempC) {
  lcd.setCursor(0, 1);
  lcd.print("Suhu : ");
  lcd.print(tempC);
  lcd.print(" C");
}

void adjustTime(const char* timeType) {
  if (timeSetter.isPressed()) {
    Serial.println("The button 1 is pressed");
    lcd.clear();
    if (timeType == "jam") alarmTime.hour++;
    if (timeType == "menit") alarmTime.minute++;
  }
}

void sendHTTP(DateTime now) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();

    String url = String(serverName) + "?user=YBHt5FSuumfICKgF6WiFCEHy8Lg1";
    http.begin(client, url);

    http.addHeader("Authorization", "Bearer <token>"); // Replace with actual token
    http.addHeader("Accept", "application/json");
    http.addHeader("Content-Type", "application/json");

    int moistureValue = 100 - analogRead(MOISTURE_PIN) * 100 / 4095;
    int lightValue = analogRead(LIGHT_PIN);
    float tempValue = analogRead(TEMP_PIN);
    float milliVolt = tempValue * (ADC_VREF_mV / ADC_RESOLUTION);
    float tempC = milliVolt / 10;
    int lightPercentage = (1 - (lightValue / 4095.0)) * 100;

    // Create timestamp string for the relay activation time
    String relayTimestamp = String(relayActivationTime.year()) + "-" +
                            String(relayActivationTime.month()) + "-" +
                            String(relayActivationTime.day()) + " " +
                            String(relayActivationTime.hour()) + ":" +
                            String(relayActivationTime.minute()) + ":" +
                            String(relayActivationTime.second());

    String jsonBody = "{\"email\":\"sukra@gmail.com\",\"id\":\"iot1\",\"suhu\":\"";
    jsonBody += tempC;
    jsonBody += "\",\"cahaya\":\"";
    jsonBody += lightPercentage;
    jsonBody += "\",\"kelembapan\":\"";
    jsonBody += moistureValue;
    jsonBody += "\",\"relay\":\"";
    jsonBody += relayTimestamp; // Send the timestamp
    jsonBody += "\"}";

    Serial.println("JSON Body: " + jsonBody);

    int httpResponseCode = http.POST(jsonBody);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void sendRelayActivationTime(DateTime now) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();

    String url = String(serverName) + "?user=YBHt5FSuumfICKgF6WiFCEHy8Lg1";
    http.begin(client, url);

    http.addHeader("Authorization", "Bearer <token>");
    http.addHeader("Accept", "application/json");
    http.addHeader("Content-Type", "application/json");

    String jsonBody = "{\"email\":\"sukra@gmail.com\",\"id\":\"iot1\",\"relay\":\"";
    jsonBody += now.year();
    jsonBody += "-";
    jsonBody += now.month();
    jsonBody += "-";
    jsonBody += now.day();
    jsonBody += " ";
    jsonBody += now.hour();
    jsonBody += ":";
    jsonBody += now.minute();
    jsonBody += ":";
    jsonBody += now.second();
    jsonBody += "\"}";

    Serial.println("JSON Body: " + jsonBody);

    int httpResponseCode = http.POST(jsonBody);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
