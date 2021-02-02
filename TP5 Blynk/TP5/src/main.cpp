/* Bibliothèques */

#include <Arduino.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

/* Variables */

char auth[] = "8kVbd1Bo9CIy25-HdSLoAAJWb3cKdw5K"; //Le Auth Token reçu par mail de Blynk 
char ssid[] = "bbox-Sophie1"; // Nom du WiFi 
char pass[] = "20150509Sophi"; // Mot de passe du WiFi
#define DHTPIN D6   // Pin connectée au DHT
#define DHTTYPE DHT11  //Préciser le type de DHT: ici le DHT11
DHT dht(DHTPIN, DHTTYPE); // Fonction de paramètrage du DHT avec PIN et Type

BlynkTimer timer; //

//Attribution des PIN à nos différents capteurs/actionneurs
const int BUTTON = D0;    
const int RED = D3;
const int GREEN = D4;
const int BLUE = D7;
const int LDR = A0;

long send_time; 

int state_button = 0; // Etat du bouton
int flag_button = 0;  // Flag du bouton
int count = 0;  // Compteur

/* Fonction du Bouton */

void button_Read()
{
    state_button = digitalRead(BUTTON); // Lire l'état du bouton
    if (state_button == LOW)  // Le bouton est en pull*up donc si le bouton est LOW c'est qu'il est activé
    {
        flag_button = 1;    // Le flag va servir à ne pas avoir de résonnance sur le bouton
        Blynk.virtualWrite(V8, 255);    // Envoi de donnée vers blynk pour allumer la LED sur l'application en TAB4
    }
    if (flag_button == 1 && state_button == HIGH) // Si bouton relâché
    {
      count += 1;   // incrémentation compteur
      Serial.println(count);    
      Blynk.virtualWrite(V9, count);    // Envoi de donnée vers blynk pour transmettre la valeur du compteur au value display en TAB4
      Blynk.virtualWrite(V8, 0);    // Envoi de donnée vers blynk pour éteindre la LED sur l'application en TAB4
      flag_button = 0;
    }
}

/* Fonction d'envoi de données */

void sendSensor()
{
  if(send_time <= millis())
  {
    float h = dht.readHumidity(); // Lecture de la valeur de l'humidité sur le DHT
    float t = dht.readTemperature(); // Lecture de la valeur de la température sur le DHT
    int lumi = analogRead(LDR); // Lecture de la valeur de la luminosité sur le DHT
    if (isnan(h) || isnan(t)) {   // gestion d'erreur si on ne reçois pas de données du DHT
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V6, h);  // Envoi de l'humidité vers blynk    sur TAB1 et TAB2
    Blynk.virtualWrite(V5, t);  // Envoi de la température vers blynk   sur TAB1 et TAB2
    Blynk.virtualWrite(V7, lumi); // Envoi de la luminosité vers blynk    sur TAB1
    send_time = millis() + 1000;      
  }
}

/* Fonction setup */

void setup()
{
  Serial.begin(9600);  //initialisation du serial àune fréquence de 9600
  pinMode(BUTTON, INPUT); // Définir le bouton en input
  send_time = millis();
  
  Blynk.begin(auth, ssid, pass, IPAddress(193,190,65,122), 8080);   //Initialisation de blynk     Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  dht.begin();    //Initialisation du DHT
  delay(1000);
}

/* Fonction loop */

void loop()
{
  Blynk.run();  // Lancer Blynk 
  timer.run();   // Lancer Timer
  sendSensor();   // Fonction d'envoi de données
  button_Read();    // Fonction du bouton
}
