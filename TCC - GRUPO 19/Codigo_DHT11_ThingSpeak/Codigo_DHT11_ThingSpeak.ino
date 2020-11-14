#include "ThingSpeak.h"
#include <WiFi.h>
#include <Wire.h>
#include "DHT.h"
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
 
char ssid[] = "NAME"; // Nome da Rede que será usada
char pass[] = "PASSWORD"; // Senha da Rede que será usada
WiFiClient client;
 
unsigned long myChannelNumber = 000000; //update
const char * myWriteAPIKey = "KEY"; //Key do canal no ThingSpeak 
 
void setup()
{
Serial.begin(115200); //Inicializa
dht.begin(); //Inicializa o sensor
WiFi.mode(WIFI_STA); //Seleciona o modo STA para o WiFi
ThingSpeak.begin(client); // Inicializa o ThingSpeak
 
}
 
void loop() {

// Conectar ou reconectar ao WiFi
if(WiFi.status() != WL_CONNECTED){
Serial.print("Conectando ao SSID: ");
while(WiFi.status() != WL_CONNECTED){
WiFi.begin(ssid, pass); 
Serial.print(".");
delay(5000);
}
Serial.println("\nConnected.");
}

float h = dht.readHumidity();
float t = dht.readTemperature();
Serial.print("Umidade do Ar: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperatura: ");
Serial.print(t);
Serial.println(" °C ");
ThingSpeak.setField(1, t);
ThingSpeak.setField(2, h);
int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
if(x == 200){
Serial.println("Atualização bem sucedida");
}
else{
Serial.println("Problema ao atualizar. HTTP error code " + String(x));
}
 
delay(20000); //
