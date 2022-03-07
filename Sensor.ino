/************************** Bibliotecas adicionais ***********************************/
#include "config.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/*************** Mapeamento de Hardware ***************/
#define DATA_PIN 2 //pin do DHT11
#define sens A0 //pin do solo
#define chuva 12 // pin da Chuva

// instacia do DHT11
DHT_Unified dht(DATA_PIN, DHT11);


// Variaveis globais
int solo = 0;


// Ponteiros para as Feeds
AdafruitIO_Feed *temperature = io.feed("Temperatura");
AdafruitIO_Feed *humidity = io.feed("Ar");
AdafruitIO_Feed *feedsolo = io.feed("Solo");
AdafruitIO_Feed *feedchuva = io.feed("Chuva");


void setup() {

  Serial.begin(115200);
  while (! Serial); //aguardando a conexão da Serial
  dht.begin(); // iniciar o DHT

  Serial.print("Connecting to Adafruit IO");
  io.connect(); // conexão com o Adafruit

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());


}

void loop() {
  io.run();

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;
  float fahrenheit = (celsius * 1.8) + 32;

  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");

  // salvar a temperatura
  temperature->save(celsius);

  dht.humidity().getEvent(&event);

  Serial.print("humidity: ");
  Serial.print(event.relative_humidity);
  Serial.println("%");

  // salvar humidade
  humidity->save(event.relative_humidity);

  solo = analogRead(sens);
  Serial.print("Solo: ");
  Serial.print(solo);
  int solo2 = 0;
  if (solo <=300){
    solo2 = 9;
  } else if (solo <= 500){
    solo2 = 7;
  }else if (solo <= 600) {
    solo2 = 5;
  }else if (solo <=800) {
    solo2 = 3;
  }else {
    solo2 = 1;
  }
  feedsolo->save(solo2);

  int chuva2 = 0;
  if (digitalRead(chuva) == 0) {
    chuva2 = 1;
  } else {
    chuva2 = 0;
  }
  Serial.print("Chuva: ");
  Serial.print(chuva2);
  feedchuva->save(chuva2);




  // Espere 10 segundos (10000 millisegundos == 10 segundos)
  delay(10000);

}
