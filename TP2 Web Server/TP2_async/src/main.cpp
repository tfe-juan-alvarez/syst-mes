#include <arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"

#ifndef STASSID
#define STASSID "bbox-Sophie1"
#define STAPSK  "20150509Sophi"
#endif

#define DHTPIN 12     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;

const int BUTTON = D0;
const int LEDred = D1;
const int LEDblue = D2;


int state_button = 0;
int flag_button = 0;
int count = 0;
const char* ssid = STASSID;
const char* password = STAPSK;
bool flag;
char str_temp[16];
char str_hum[16];
char count_txt[10];
String mybutton;

AsyncWebServer server(80);


void temp() {
    sprintf(str_temp, "%f", t);
}
void humi() {
    sprintf(str_hum, "%f", h);
}


void bp() {
    if (digitalRead(BUTTON) == LOW)
    {
        mybutton = "bouton_actif";
    }
    else
    {
        mybutton = "bouton_non_actif";
    }
}

void Compteur() {
    sprintf(count_txt, "%d", count);
}

void button_Read()
{
    state_button = digitalRead(BUTTON);
    if (state_button == LOW)
    {
        flag_button = 1;
        digitalWrite(LEDblue, HIGH);
    }
    else
    {
        digitalWrite(LEDblue, LOW);
    }
    if (flag_button == 1 && state_button == HIGH)
    {
        count += 1;
        flag_button = 0;
    }
}

void LEDisON() {
    digitalWrite(LEDred, 1);
}
void LEDisOFF() {
    digitalWrite(LEDred, 0);
}

void handleNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
// Replaces placeholder with DHT values
String processor(const String& var) {
    //Serial.println(var);
    if (var == "TEMPERATURE") {
        return String(t);
    }
    else if (var == "HUMIDITY") {
        return String(h);
    }
    return String();
}
void setup() {
    pinMode(LEDred, OUTPUT);     // Initialize the LED pin as an output
    digitalWrite(LEDred, LOW);
    pinMode(LEDblue, OUTPUT);     // Initialize the LED pin as an output
    digitalWrite(LEDblue, LOW);

    pinMode(BUTTON, INPUT);

    Serial.begin(115200);
    dht.begin();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

  if(!LittleFS.begin())
  {
    Serial.println("Erreur LittleFS...");
    return;
  }

  File root = LittleFS.open("/", "r");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/w3.css", "text/css");
    });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(LittleFS, "/script.js", "text/javascript");
    });
    server.on("/Onizuka.jpg", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(LittleFS, "/Onizuka.jpg", "image/jpg");
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      humi();
      request->send(200, "text/plain", str_hum);
    });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      temp();
      request->send(200, "text/plain", str_temp);
    });
    server.on("/LEDisON", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      LEDisON();
      request->send(200,"text/plain", " is on");
    });
    server.on("/LEDisOFF", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      LEDisOFF();
      request->send(200,"text/plain", " is off");
    });
    server.on("/bp", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      bp();
      request->send(200, "text/plain", mybutton);
    });
    server.on("/compteur", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      Compteur();
      request->send(200, "text/plain", count_txt);
    });

    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        // save the last time you updated the DHT values
        previousMillis = currentMillis;
        // Read temperature as Celsius (the default)
        float newH = dht.readTemperature();
        // Read temperature as Fahrenheit (isFahrenheit = true)
        //float newT = dht.readTemperature(true);
        // if temperature read failed, don't change t value
        if (isnan(newH)) {
            Serial.println("Failed to read from DHT sensor!");
        }
        else {
            h = newH;
            Serial.println(t);
        }
        // Read Humidity
        float newT = dht.readHumidity();
        // if humidity read failed, don't change h value 
        if (isnan(newT)) {
            Serial.println("Failed to read from DHT sensor!");
        }
        else {
            t = newT;
            Serial.println(h);
        }
    }
    button_Read();
}