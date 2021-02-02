#include <Arduino.h>
#include <AdafruitIO.h>
#include "config.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// pin connected to DH11 data line
#define DHTPIN 4

// pin connected to DH11 data line
#define ANALOG_PIN 35

// analog state
int current = 0;
int last = -1;

// pins to rgb
#define RED_PIN   16
#define GREEN_PIN 17
#define BLUE_PIN  18


// dht type --> dht11
#define DHTTYPE    DHT11     // DHT 11

// Button Pin
#define BUTTON_PIN 5

// LED Pin
#define LED_PIN 2

DHT dht(DHTPIN, DHTTYPE);

float h,t,f;
long send_time,send_time2;
// button state
bool btn_state = false;
bool prv_btn_state = false;

// set up the 'led' feed
AdafruitIO_Feed *led = io.feed("led");

// set up the 'button' feed
AdafruitIO_Feed *button = io.feed("button");

// set up the 'température' feed
AdafruitIO_Feed *temp = io.feed("temp");

// set up the 'humidity' feed
AdafruitIO_Feed *humi = io.feed("humi");


// set up the 'humidity' feed
AdafruitIO_Feed *color = io.feed("color");

// set up RGB feed
AdafruitIO_Feed *RGB_B = io.feed("RGB_B");
AdafruitIO_Feed *RGB_G = io.feed("RGB_G");
AdafruitIO_Feed *RGB_R= io.feed("RGB_R");

// analog feed for adc
AdafruitIO_Feed *lumi = io.feed("lumi");

// this function is called whenever a 'led' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleLed(AdafruitIO_Data *data) {
  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("LED_TURNED_ON");

  digitalWrite(LED_PIN, data->toPinLevel());
}

/*void handlecolor(AdafruitIO_Data *data) 
{
  // print RGB values and hex value
  Serial.println("Received:");
  Serial.print("  - R: ");
  Serial.println(data->toRed());
  Serial.print("  - G: ");
  Serial.println(data->toGreen());
  Serial.print("  - B: ");
  Serial.println(data->toBlue());
  Serial.print("  - HEX: ");
  Serial.println(data->value());

  // invert RGB values for common anode LEDs
  #if defined(ARDUINO_ARCH_ESP32) // ESP32 analogWrite
    ledcWrite(1, 255 - data->toRed());
    ledcWrite(2, 255 - data->toGreen());
    ledcWrite(3, 255 - data->toBlue());
  #else
    analogWrite(RED_PIN, 255 - data->toRed());
    analogWrite(GREEN_PIN, 255 - data->toGreen());
    analogWrite(BLUE_PIN, 255 - data->toBlue());
  #endif

 } */

void handleRGBB(AdafruitIO_Data *data) {
  #if defined(ARDUINO_ARCH_ESP32) // ESP32 analogWrite
    ledcWrite(3,255 - data->toInt());
  #else
    analogWrite(BLUE_PIN, 255 - data->toBlue());
  #endif
}

void handleRGBR(AdafruitIO_Data *data) {
  #if defined(ARDUINO_ARCH_ESP32) // ESP32 analogWrite
    ledcWrite(1,255 - data->toInt());
  #else
    analogWrite(RED_PIN, 255 - data->toRed());
  #endif
}

void handleRGBG(AdafruitIO_Data *data) {
  #if defined(ARDUINO_ARCH_ESP32) // ESP32 analogWrite
    ledcWrite(2,255 - data->toInt());
  #else
    analogWrite(GREEN_PIN, 255 - data->toGreen());
  #endif
}

void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT);

  // set LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  #if defined(ARDUINO_ARCH_ESP32) // ESP32 pinMode
    // assign rgb pins to channels
    ledcAttachPin(RED_PIN,1);
    ledcAttachPin(GREEN_PIN,2);
    ledcAttachPin(BLUE_PIN,3);

    //adcpin
    ledcAttachPin(ANALOG_PIN,4);
    // init. channels
    ledcSetup(1, 1000, 8);
    ledcSetup(2, 1000, 8);
    ledcSetup(3, 1000, 8);
  #else
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
  #endif

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);
  // Initialize device.
  dht.begin();

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  led->onMessage(handleLed);

  //color->onMessage(handlecolor);
  RGB_R->onMessage(handleRGBR);
  RGB_B->onMessage(handleRGBB);
  RGB_G->onMessage(handleRGBG);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  led->get();

}

void loop() {

  io.run();

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
    Serial.print("sending temperature -> "); 
    Serial.println(t);
    temp->save(t);

    // save the btn_state state to the 'button' feed on adafruit io
    Serial.print("sending humidity-> "); 
    Serial.println(h);
    humi->save(h);

    send_time = millis() + 10000;  // set next time you want to do anything
  }
// grab the current state of the photocell
  current = analogRead(ANALOG_PIN);
  current = map(current,0,4095,0,100);

  // return if the value hasn't changed
  if(current != last){
    // save the current state to the analog feed
    Serial.print("sending -> ");
    Serial.println(current);
    // store last photocell state
    last = current;
  }
  if(send_time2 <= millis()){
    lumi->save(current);
    send_time2 = millis() + 60000;
  }
      // grab the btn_state state of the button.
  if(digitalRead(BUTTON_PIN) == LOW)
    btn_state = false;
  else
    btn_state = true;
  // return if the btn state hasn't changed
  if(btn_state == prv_btn_state)
    return;

  // save the btn_state state to the 'button' feed on adafruit io
  Serial.print("sending button -> "); 
  Serial.println(btn_state);
  button->save(btn_state);

  // store last button state
  prv_btn_state = btn_state;
}
