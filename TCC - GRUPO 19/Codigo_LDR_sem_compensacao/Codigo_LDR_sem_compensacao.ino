#include <Wire.h>
#define MAX_ADC_READING           4095
#define ADC_REF_VOLTAGE           3.3 
#define REF_RESISTANCE            9950  
#define LUX_CALC_SCALAR           51749736 
#define LUX_CALC_EXPONENT         -1.885 

void setup(void) 
{
  Serial.begin(115200);
}
void loop(void) 
{  
  const int portaLDR = GPIO_NUM_35;
  int leitura = analogRead(portaLDR);
  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float ldrLux;
  
  // Conversão de analógico para digital 
  ldrRawData = analogRead(portaLDR);
  
  // Tensão do Resistor
  // Convert the raw digital data back to the voltage that was measured on the analog pin
  resistorVoltage = 1023 / MAX_ADC_READING * ADC_REF_VOLTAGE;

  // Tensão através do LDR que é a tensão de alimentação menos a tensão no resistor de 10kohms
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  
  // LDR_RESISTANCE_CONVERSION
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  
  // LDR LUX
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
 
  Serial.print("LDR Raw Data   : "); Serial.println(ldrRawData);
  Serial.print("LDR Voltage    : "); Serial.print(ldrVoltage); Serial.println(" volts");
  Serial.print("LDR Resistance : "); Serial.print(ldrResistance); Serial.println(" Ohms");
  Serial.print("LDR Illuminance: "); Serial.print(ldrLux); Serial.println(" lux");

  delay(250);
}
