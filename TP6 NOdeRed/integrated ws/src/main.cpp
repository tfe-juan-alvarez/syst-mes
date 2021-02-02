#include <Arduino.h>

//#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>   //NODERED
// #include <BlynkSimpleEsp32.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Button Pin
#define BUTTON_PIN 5

// LED Pin
#define LED_PIN 13

// pin connected to DH11 data line
#define DHTPIN 4

#define servoPin 18
ESP32PWM pwm;
int freq = 1000;

// pins to rgb
#define RED_PIN   16
#define GREEN_PIN 17
#define BLUE_PIN  26

// dht type --> dht11
#define DHTTYPE    DHT11     // DHT 11

const char* mqtt_server ="192.168.1.23";    //NODERED

#define WIFI_SSID "Nilort 2.0"
#define WIFI_PASS "0483408720"
//#define WIFI_SSID "juanito pepito"
//#define WIFI_PASS "aaaa1230"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

WiFiClient espClient;       //NODERED
PubSubClient client(espClient);      //NODERED
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int count;

DHT dht(DHTPIN, DHTTYPE);

float h,t,f,maxtemp,maxhumi;
char send_buffer_temp [16]; 
char send_buffer_humi [16]; 
long send_time,send_time2;
// button state
bool btn_state = false;
bool prv_btn_state = false;

Servo myservo;  // create servo object to control a servo


void callback(char* topic, byte* payload, unsigned int length) {     //NODERED que faire quand un msg arrive
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[32];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i]=payload[i];
  }
  Serial.println();
  Serial.print(msg);
  Serial.println();
  memset(msg,0,sizeof(msg));
  if ((char)payload[0] == '0') {
    digitalWrite(LED_PIN, LOW);   // Turn the LED off
  } 
  if ((char)payload[0] == '1')
  {
   digitalWrite(LED_PIN, HIGH);  // Turn the LED on
  }
  if ((char)payload[0] == '3')
  {
    digitalWrite(LED_PIN, LOW);   // Turn the LED off
  }
  if ((char)payload[0] == '4')
  {
    digitalWrite(LED_PIN, HIGH);   // Turn the LED on 
  }
  if ((char)payload[0] == '5')
  {
   myservo.write(0);
    ledcWrite(1,0);
    ledcWrite(2,255);
    ledcWrite(3,255);
  }
  if ((char)payload[0] == '6')
  {
    myservo.write(60);
    ledcWrite(1,20);
    ledcWrite(2,220);
    ledcWrite(3,250);
  }
  if ((char)payload[0] == '7')
  {
   myservo.write(120);
    ledcWrite(1,0);
    ledcWrite(2,128);
    ledcWrite(3,255);
  }
  if ((char)payload[0] == '8')
  {
    myservo.write(180);
    ledcWrite(1,255);
    ledcWrite(2,0);
    ledcWrite(3,255);
  }
  else
  {
    Serial.print("Unknown Payload");
  }
}

void reconnect() {     //NODERED mqqt connexion 
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("LIGHT1");
      client.subscribe("LIGHT2");
      client.subscribe("VANNE");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup(){
  send_time=millis();
  send_time2=millis();
  pinMode(BUTTON_PIN, INPUT);

  // set LED pin as an output
  pinMode(LED_PIN, OUTPUT);
 //----------------------------------------------------Serial
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

   //----------------------------------------------------SPIFFS
  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  Serial.println("Successfully Ended");


  #if defined(ARDUINO_ARCH_ESP32) // ESP32 pinMode
    // assign rgb pins to channels
    ledcAttachPin(RED_PIN,1);
    ledcAttachPin(GREEN_PIN,2);
    ledcAttachPin(BLUE_PIN,3);

    // init. channels
    ledcSetup(1, 1000, 8);
    ledcSetup(2, 1000, 8);
    ledcSetup(3, 1000, 8);
  #else
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
  #endif


//  Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	
  //pwm.attachPin(APin, freq, 10); // 1KHz 8 bit

  myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin); // attaches the servo on pin 18 to the servo object
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server,1883);    //NODERED
  client.setCallback(callback);         //NODERED

  //----------------------------------------------------SERVER

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
      // Route for root / web page
    request->send(SPIFFS, "/index.html","text/html");
    //request->send(200, "text/html",index_html);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    //t = dht.readTemperature();
    //Blynk.virtualWrite(V6, t);
    memset(send_buffer_temp,'0',sizeof(send_buffer_temp));
    sprintf(send_buffer_temp,"%f",t);
    request->send(200, "text/plain",send_buffer_temp);
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    //h = dht.readHumidity();
    //Blynk.virtualWrite(V5, h);
    memset(send_buffer_humi,'0',sizeof(send_buffer_humi));
    sprintf(send_buffer_humi,"%f",h);
    request->send(200, "text/plain",send_buffer_humi);
  });

  dht.begin();
  //Blynk.begin(auth, WIFI_SSID, WIFI_PASS, IPAddress(193,190,65,122), 8080);
  // Start server
  server.begin();
}
 
void loop(){
  //Blynk.run();
  if (!client.connected()) {   //NODERED
    reconnect();
  }
  client.loop();      //NODERED
  if(send_time <= millis()){
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    f = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      //return;
    }
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));

      // save the btn_state state to the 'button' feed on adafruit io

    snprintf (msg, MSG_BUFFER_SIZE, "%f", t);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("temp", msg);
    
    // save the btn_state state to the 'button' feed on adafruit io
    snprintf (msg, MSG_BUFFER_SIZE, "%f", h);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("humi", msg);
    
  if(maxtemp<t){
      maxtemp = t;
    }
  if(maxhumi<h){
      maxhumi = h;
    }
    send_time = millis() + 10000;  // set next time you want to do anything
  }
  if(send_time2 <= millis()){
    if(count==0){
      Serial.println("print to dataBase");
      snprintf (msg, MSG_BUFFER_SIZE, "%f", maxtemp);
      client.publish("DBT",msg);
      maxtemp=0;
      }
    else if (count==1)
    {
      Serial.println("print to dataBase");
      snprintf (msg, MSG_BUFFER_SIZE, "%f", maxhumi);
      client.publish("DBH",msg);
      maxhumi=0;
    }
    send_time2 = millis() + 60000;
    count +=1;
    if (count ==2) count=0;
    }
}