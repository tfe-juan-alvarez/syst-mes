/* Bibliothèques */

#include <Arduino.h>                    
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"

/* Variables */

WiFiClientSecure gsclient;
String GAS_ID = "AKfycbybjsotK0WczFlCYcgxVW9FIZJPjjyWERmYpc7iVP3F1HJZBrA";    //getactivespreadsheetID 
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";  //génération d'un fingerprint
const char* host = "script.google.com";  // configuration de l'hôte
const int httpsPort = 443;  // configuration du port https

#ifndef STASSID                         //configuration nom du Wi-Fi et de son mot de passe 
#define STASSID "bbox-Sophie1"          // nom du Wi-Fi
#define STAPSK  "20150509Sophi"         // mot de passe du Wi-Fi
#endif
const char* ssid = STASSID;               
const char* password = STAPSK;            

const int BUTTON = D0;          // Lier la Pin D0 à notre bouton         
const int LEDred = D1;          // Lier la Pin D1 à notre LED rouge     
const int LDR = A0;             // Lier la Pin A0 à notre capteur de Luminosité     


float humy;                     // Variable pour l'humidité
float temp;                     // Variable pour la température
int lumi;                       // Variable pour la luminosité
int state_button = 0;           // Variable pour l'état de notre bouton
int flag_button = 0;            // flag du bouton
int count = 0;                  // compteur du nombre d'appui sur le bouton

#define DHTPIN D6     // Lier la Pin D6 à notre capteur DHT     
#define DHTTYPE    DHT11     // Préciser le type de DHT, ici le DHT 11
DHT dht(DHTPIN, DHTTYPE);     // configuration de notre DHT en précisant via nos variables: La Pin et le type de DHT

/* Fonction du Bouton poussoir */

void button_Read()      //cette fonction va servir à ce que pour un appui il y ai un envoi de donnée
{
    state_button = digitalRead(BUTTON);   // Lire l'état du bouton et l'attribuer à notre variable state_button
    if (state_button == LOW)  // comme le bouton est en pull-up on met le flag à 1 quand l'état du bouton est BAS
    {
        flag_button = 1;
    }
    if (flag_button == 1 && state_button == HIGH)         
    {
        count += 1;             //incrémentation du compteur
        flag_button = 0;
    }
}

/* Fonction d'envoi de données */

void sendData(int x, int y, int z)  // prend comme paramètre les données que l'on souhaite envoyer, ici température, humidité et luminosité
{
  Serial.print("connecting to ");
  Serial.println(host);
  gsclient.setInsecure();   
  if (!gsclient.connect("script.google.com", httpsPort))  // on vérifie le port et le fingerprint
  {
    Serial.println("connection failed");
    return;
  }
  if (gsclient.verify(fingerprint, host))   // on vérifie le port et le fingerprint
  {
    Serial.println("certificate matches");
  } 
  else 
  {
    Serial.println("certificate doesn't match");
  }
  // x =  température, y = humidité et z = luminosité
  String string_x     =  String(x, DEC);  // conversion de x en string
  String string_y     =  String(y, DEC);  // conversion de y en string
  String string_z     =  String(z, DEC);  // conversion de z en string

  Serial.println(string_x);
  Serial.println(string_y);
  Serial.println(string_z);

  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_x + "&humidity="+ string_y + "&luminosity="+ string_z;   //création de l'url qui enverra les données sur le google sheet
  Serial.print("requesting URL: "); 
  Serial.println(url); 


  gsclient.print(String("GET ") + url + " HTTP/1.1\r\n" + 
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (gsclient.connected()) {
    String line = gsclient.readStringUntil('\n');
    if (line == "\r") 
    {
      Serial.println("headers received");
      break;
    }
  }
  String line = gsclient.readStringUntil('\n');
  Serial.println(line);
  if (line.startsWith("{\"state\":\"success\"")) 
  {
    Serial.println("esp8266/Arduino CI successfull!");
  } 
  else 
  {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("closing connection");
  }

/* fonction setup */

void setup()
{
  Serial.begin (9600);
  pinMode(LEDred, OUTPUT);     // défini la LED en output
  digitalWrite(LEDred, LOW);   // Eteind la LED
  pinMode(BUTTON, INPUT);      // défini le bouton en input
  dht.begin();                 // Initialise le DHT
  WiFi.mode(WIFI_STA);         // Précise le mode du WiFi
  WiFi.begin(ssid, password);  // Attribue le nom et le mot de passe du wifi
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {     // Tant que la connection au wifi n'est pas établie, en imprime  des points sur le terminal
  delay(1000);
  Serial.print(".");
}
}

/* fonction loop */

void loop()
{
  button_Read();    
  float temp = dht.readTemperature(); //on lit la valeur de la température du DHT et on l'attibue à une variable
  float humy = dht.readHumidity();    //on lit la valeur de l'humidité du DHT et on l'attibue à une variable
  int lumi = analogRead(LDR);         //on lit la valeur de la luminosité du capteur LDR et on l'attibue à une variable

  if (flag_button == 1)  // On envoie nos données quand on appuie sur le bouton et pour le vérifier une LED s'alume brièvement
  {
    sendData(temp,humy,lumi);  
    Serial.println(temp);
    Serial.println(humy);
    Serial.println(lumi);
    digitalWrite(LEDred, HIGH);
    delay(100);
    digitalWrite(LEDred, LOW);
  }
}