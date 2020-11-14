#include "ThingSpeak.h"
#include <WiFi.h>
#include <Wire.h>
#include "DHT.h"
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define MAX_ADC_READING           4095 // no ESP32 a leitura máxima é de 4095
#define ADC_REF_VOLTAGE           3.3  //a alimentação é de 3,3V
#define REF_RESISTANCE            9950  // Resistência do resistor do módulo LDR
#define LUX_CALC_SCALAR           517497367 //10 elevado a interceptação em Y
#define LUX_CALC_EXPONENT         -1.885   //inclinação
const int portaLDR = 36;
int sensor_pin = 39;
int value ;
int moisturePercentage;
 
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
  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float ldrLux;
  float ldrLuxN;

  // Obtém o valor digital da leitura analógica  
  ldrRawData = analogRead(portaLDR);

  // Tensão no Resistor
  // Converte a leitura digital para o valor real
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;

  // a tensão através do LDR é 3,3V supply menos a tensão no resistor de 10 kOhms 
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;

  // Resistência do LDR
  // resistência que o LDR possui devido a esta tensão
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;

  // LDR para lux
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

//Thingspeak update e IDE 

float h = dht.readHumidity();
float t = dht.readTemperature();
moisturePercentage = ( 100 - ( (analogRead(sensor_pin) / 4095.00) * 100 ) );

Serial.print("Umidade do Ar: ");
Serial.print(h);
Serial.println(" %\t");
Serial.print("Temperatura: ");
Serial.print(t);
Serial.println(" °C ");
Serial.print("Luminosidade ESP32: ");
Serial.println(ldrRawData);
Serial.print("Umidade do Solo : ");
Serial.print(moisturePercentage);
Serial.println("%");
ThingSpeak.setField(1, t);
ThingSpeak.setField(2 , h);
ThingSpeak.setField(4 , moisturePercentage);

//condições especiais do Lux
if (ldrRawData>=4000)
{
float ldrLuxN = (ldrLux/6);
ThingSpeak.setField(3, ldrLuxN);
Serial.print("Luminosidade Lux: ");Serial.print(ldrLuxN);Serial.println(" Lux ");
}
//NESTE INTERVALO PRECISA MULTIPLICAR A SAÍDA POR 2 PARA COMPENSAR
else if (ldrRawData>=0 && ldrRawData<=3655)
{
float ldrLuxN = (ldrLux*2);
ThingSpeak.setField(3, ldrLuxN);
Serial.print("Luminosidade Lux: ");Serial.print(ldrLuxN);Serial.println(" Lux ");
}
//NESTE INTERVALO PRECISA MULTIPLICAR A SAÍDA POR 1,6 PARA COMPENSAR
else if (ldrRawData>=3550 && ldrRawData<=3620)
{
float ldrLuxN = (ldrLux*1.6);
ThingSpeak.setField(3, ldrLuxN);
Serial.print("Luminosidade Lux: ");Serial.print(ldrLuxN);Serial.println(" Lux ");
}
//NESTE INTERVALO PRECISA MULTIPLICAR A SAÍDA POR 1,2 PARA COMPENSAR
else if (ldrRawData>=3621 && ldrRawData<=3793)  {
float ldrLuxN = (ldrLux*1.2);
ThingSpeak.setField(3, ldrLuxN);
Serial.print("Luminosidade Lux: ");Serial.print(ldrLuxN);Serial.println(" Lux ");
}
//NESTE INTERVALO PRECISA MULTIPLICAR A SAÍDA POR 0,8 PARA COMPENSAR
else if (ldrRawData>=3794 && ldrRawData<=3800)  {
float ldrLuxN = (ldrLux*0.8);
ThingSpeak.setField(3, ldrLuxN);
Serial.print("Luminosidade Lux: ");Serial.print(ldrLuxN);Serial.println(" Lux ");
}
//NESTE INTERVALO PRECISA MULTIPLICAR A SAÍDA POR 0,6 PARA COMPENSAR
else if (ldrRawData>=3801 && ldrRawData<=3999)  {
float ldrLuxN = (ldrLux*0.6);
ThingSpeak.setField(3, ldrLuxN);
Serial.print("Luminosidade Lux: ");Serial.print(ldrLuxN);Serial.println(" Lux ");
}
int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
if(x == 200){
Serial.println("Atualização bem sucedida");
}
else{
Serial.println("Problema ao atualizar. HTTP error code " + String(x));
}
 
delay(20000); // Wait 20 seconds to update the channel again
}
