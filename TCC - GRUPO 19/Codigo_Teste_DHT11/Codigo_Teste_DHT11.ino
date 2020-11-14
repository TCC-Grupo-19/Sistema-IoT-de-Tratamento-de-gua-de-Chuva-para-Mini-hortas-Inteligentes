#include "DHT.h"
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int val;
int LDRpin = A0;
 
void setup()
{
Serial.begin(115200);
Serial.println("Sensor DHT11");

dht.begin();
}
 
void loop() {
val = analogRead(LDRpin);
float h = dht.readHumidity();
float t = dht.readTemperature();
int luz = analogRead (4);
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
// print the result to Terminal
Serial.print("Umidade do Ar: ");
Serial.print(h);
Serial.println(" %\t");
Serial.print("Temperatura: ");
Serial.print(t);
Serial.println(" °C ");
Serial.print("Luminosidade: ");
Serial.println(val);

delay(2000);
}
