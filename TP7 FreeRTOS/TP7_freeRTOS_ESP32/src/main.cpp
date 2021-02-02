/* bibliothèques */

#include <Arduino.h>

/* Variables */
const int BUTTON = GPIO_NUM_21;   //Pin du bouton
const int LEDred = GPIO_NUM_22;   //Pin de la LED rouge
const int LEDblue = GPIO_NUM_23;    //Pin de la LED bleu

int flag_button;   //flag du bouton
int state_button;   //état du bouton
int count;    //compteur

static QueueHandle_t qh;     //Création de notre file qh

/* Tâche Bouton */

void TaskButton(void *arg)
{
  int bufferSendData;   //variable de la donnée qui sera envoyée
  for(;;)
  {
    state_button = digitalRead(BUTTON);   //lecture de l'état du bouton
    if (state_button == LOW)    // Système avec flag qui permet d'éviter le rebond du bouton
    {
        flag_button = 1;
    }
    if (flag_button == 1 && state_button == HIGH)
    {
        count += 1;   //incrémentation du compteur
        flag_button = 0;
    }
    if (count == 0)
    {
      bufferSendData = 2;   //Valeur envoyée aux autres tâches pour que les 2 LED restent éteintes au démarrage
    }
    else 
    {
      bufferSendData = count % 2;   //Valeur envoyée aux autres tâches (1 si count est impair et 0 si count est pair)
    }
    //(file,  buffer valeur envoyée,  Timeout)
    xQueueSendToBack(qh, &bufferSendData, portMAX_DELAY);   //fonction pour envoyer la valeur dans la file 
  }
}

/* Tâche LED Red */

void TaskLEDRed(void *arg)
{
  int bufferGetDataRed;   //variable de la donnée reçue
  for(;;)
  {
    //(file,  buffer valeur reçue,  Timeout)
    xQueueReceive(qh,&bufferGetDataRed,portMAX_DELAY);  //fonction pour recevoir la valeur de la file 
    if (bufferGetDataRed == 1)    //si la valeur est égale à 1, la LED rouge clignote à une fréquence de 2Hz
    {
      digitalWrite(LEDred,1);   //LED rouge s'allume
      delay(500);               //500ms de délai
      digitalWrite(LEDred,0);   //LED rouge s'éteind
      delay(500);               //500ms de délai
    }
    else
    {
      digitalWrite(LEDred,0);   //LED rouge s'éteind   
    }
  }
}

/* Tâche LED Bleu */

void TaskLEDBlue(void *arg)
{
  int bufferGetDataBlue;    //variable de la donnée reçue
  for(;;)
  {
    //(file,  buffer valeur reçue,  Timeout)
    xQueueReceive(qh,&bufferGetDataBlue,portMAX_DELAY);  //fonction pour recevoir la valeur de la file 
    if (bufferGetDataBlue == 0)    //si la valeur est égale à 0, la LED bleu clignote à une fréquence de 4Hz
    {
      digitalWrite(LEDblue,1);    //LED bleu s'allume
      delay(250);                 //250ms de délai
      digitalWrite(LEDblue,0);    //LED bleu s'éteind
      delay(250);                 //250ms de délai
    }
    else
    {
      digitalWrite(LEDblue,0);    //LED bleu s'éteind
    }
  }
}

/* fonction setup */

void setup() 
{
  pinMode(LEDred, OUTPUT);     // Initialise la LED rouge en output
  digitalWrite(LEDred, LOW);    //Eteind la LED rouge
  pinMode(LEDblue, OUTPUT);     // Initialise la LED Bleu en output
  digitalWrite(LEDblue, LOW);   //Eteind la LED bleu
  pinMode(BUTTON, INPUT);       // Initialise le bouton en input

  //mise à zéro de quelques variables
  flag_button = 0;
  state_button = 0;
  count = 0;

  qh = xQueueCreate(3,8); //paramètrage/configuration de la file qh
  xTaskCreatePinnedToCore(TaskButton,"TaskButton",2048,NULL,1,NULL,1); //(la Tâche, le nom de la tâche, taille, paramètre, priorité, handle, CPU)
  xTaskCreatePinnedToCore(TaskLEDRed,"TaskLEDRed",2048,NULL,1,NULL,1);
  xTaskCreatePinnedToCore(TaskLEDBlue,"TaskLEDBlue",2048,NULL,1,NULL,1);
}
void loop() 
{
  //Nothing else matters
}