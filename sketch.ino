
#include "DHTesp.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Wi-Fi details
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak channel details
const char* writeAPIKey = "YOUR_WRITE_API_KEY";
const char* server = "http://api.thingspeak.com/update";

// ThingSpeak email alert details
const char* alertsAPIKey = "YOUR_ALERTS_API_KEY";
const char* alertsServer = "https://api.thingspeak.com/alerts/send";

// Pins
int tempPin = 15;
int potPin = 34;
int ledPin = 25;
int buzzerPin = 26;
int buttonPin = 27;

// Sensor values
int sensorValue;
int bpm;

float latitude = 12.9716;
float longitude = 77.5946;
float altitude = 920.0;

// SOS management
bool sosActive = false;
bool lastButtonState = HIGH;

// ThingSpeak update interval
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 20000;

// Send SOS email
void sendSOSAlert()
{
    WiFiClientSecure secureClient;

    // For Wokwi testing only
    secureClient.setInsecure();

    HTTPClient alertHttp;

    if (WiFi.status() == WL_CONNECTED)
    {
        alertHttp.begin(secureClient, alertsServer);

        alertHttp.addHeader(
            "ThingSpeak-Alerts-API-Key", alertsAPIKey);

        alertHttp.addHeader(
            "Content-Type", "application/json");

        String message = "Emergency SOS activated! ";
        message += "Soldier needs immediate assistance. ";
        message += "Latitude: " + String(latitude, 4);
        message += ", Longitude: " + String(longitude, 4);

        String jsonData = "{\"subject\":\"SOLDIER SOS EMERGENCY\",";
        jsonData += "\"body\":\"" + message + "\"}";

        Serial.println("Sending SOS email alert...");

        int responseCode = alertHttp.POST(jsonData);

        Serial.print("Email alert response: ");
        Serial.println(responseCode);

        if (responseCode == 202)
        {
            Serial.println("SOS EMAIL REQUEST ACCEPTED!");
        }
        else
        {
            Serial.println("EMAIL ALERT FAILED!");
            Serial.println(alertHttp.getString());
        }

        alertHttp.end();
    }
    else
    {
        Serial.println("WiFi disconnected. Email not sent.");
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);

    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);

    // Initialize DHT22
    dht.setup(tempPin, DHTesp::DHT22);

    // Initialize LCD
    Wire.begin(21, 22);
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("SOLDIER SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");

    Serial.println("SOLDIER HEALTH AND POSITION SYSTEM");

    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password, 6);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting WiFi");

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected!");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print("System Ready");

    delay(2000);
    lcd.clear();
}

void loop()
{
    // Read DHT22
    TempAndHumidity data = dht.getTempAndHumidity();

    // Simulated heart rate
    sensorValue = analogRead(potPin);
    bpm = map(sensorValue, 0, 4095, 60, 100);

    // Detect SOS button press
    bool buttonState = digitalRead(buttonPin);

    if (lastButtonState == HIGH && buttonState == LOW)
    {
        sosActive = !sosActive;

        if (sosActive)
        {
            Serial.println("SOS BUTTON PRESSED!");
            Serial.println("EMERGENCY SOS ACTIVATED!");

            // Send email only when SOS is activated
            sendSOSAlert();
        }
        else
        {
            Serial.println("SOS EMERGENCY RESET!");
        }

        delay(50);
    }

    lastButtonState = buttonState;

    // Health condition
    bool healthAlert = (data.temperature > 38.0 ||
                        bpm < 60 || bpm > 100);

    // Simulated GPS movement
    if (sensorValue < 1024)
    {
        latitude += 0.0001;
    }
    else if (sensorValue < 2048)
    {
        latitude -= 0.0001;
    }
    else if (sensorValue < 3072)
    {
        longitude += 0.0001;
    }
    else
    {
        longitude -= 0.0001;
    }

    // Local alert
    if (sosActive)
    {
        digitalWrite(ledPin, HIGH);
        tone(buzzerPin, 1000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("!!! SOS ALERT !!!");
        lcd.setCursor(0, 1);
        lcd.print("SOLDIER IN DANGER");

        Serial.println("!!! EMERGENCY SOS ALERT !!!");
    }
    else if (healthAlert)
    {
        digitalWrite(ledPin, HIGH);
        tone(buzzerPin, 1000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("!! HEALTH ALERT");
        lcd.setCursor(0, 1);
        lcd.print("Check Soldier");

        Serial.println("HEALTH ALERT");
    }
    else
    {
        digitalWrite(ledPin, LOW);
        noTone(buzzerPin);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HEALTH NORMAL");
        lcd.setCursor(0, 1);
        lcd.print("Soldier Safe");

        Serial.println("HEALTH NORMAL");
    }

    // Print sensor information
    Serial.print("Temperature: ");
    Serial.println(data.temperature);

    Serial.print("Humidity: ");
    Serial.println(data.humidity);

    Serial.print("Heart Rate: ");
    Serial.println(bpm);

    Serial.print("Latitude: ");
    Serial.println(latitude, 4);

    Serial.print("Longitude: ");
    Serial.println(longitude, 4);

    // Upload data to ThingSpeak every 20 seconds
    if (millis() - lastUpdate >= updateInterval)
    {
        lastUpdate = millis();

        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;
            WiFiClient client;

            http.begin(client, server);

            http.addHeader(
                "Content-Type",
                "application/x-www-form-urlencoded");

            int sosStatus = sosActive ? 1 : 0;

            String postData = "api_key=" + String(writeAPIKey);
            postData += "&field1=" + String(data.temperature, 1);
            postData += "&field2=" + String(data.humidity, 1);
            postData += "&field3=" + String(bpm);
            postData += "&field4=" + String(latitude, 4);
            postData += "&field5=" + String(longitude, 4);
            postData += "&field6=" + String(sosStatus);

            Serial.println("Sending data to ThingSpeak...");

            int httpCode = http.POST(postData);

            if (httpCode > 0)
            {
                String response = http.getString();

                Serial.print("ThingSpeak response: ");
                Serial.println(response);

                if (response.toInt() > 0)
                {
                    Serial.println("Data uploaded successfully!");
                }
                else
                {
                    Serial.println("Upload not accepted.");
                    Serial.println("Check API key or update limit.");
                }
            }
            else
            {
                Serial.print("HTTP Error: ");
                Serial.println(httpCode);
            }

            http.end();
        }
        else
        {
            Serial.println("WiFi disconnected!");
        }
    }

    delay(500);
}
