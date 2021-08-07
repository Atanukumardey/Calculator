/**
 * This example shows an implementation of calculator library using serial input and output
 * input like - sqr((cos(45)/tan(25/5+6*32))*10)
 */ 



#include <Arduino.h>

#include <Calculator.h>
Calculator calculator;
  // creating object

char expression[200];  // variable to take expression input
uint8_t exindex = 0;

void mathcalculation()
{
  while (true)
  {
    if (Serial.available())
    {
      delay(45); // waiting sometime to arrive full expression in case it is long
      exindex = 0;
      memset(expression, '\0', sizeof(expression));
      
      while (Serial.available())
      {
        expression[exindex++] = (char)Serial.read();
      }
      
      uint32_t times = micros();
      
      calculator.getans(expression, exindex);
      
      Serial.print(F("Total calculation time taken - "));
      Serial.print(micros() - times);
      Serial.println(F(" microsecons"));
      
      Serial.print(F("Output- "));
      switch (calculator.calculation_data.error)
      {
      case 1:
        Serial.println(F("Syntex Error"));
        break;
      case 2:
        Serial.println(F("Math Error"));
        break;
      default:
        Serial.println(calculator.calculation_data.ans);
        break;
      }
      
      Serial.println(F("Input Expression: "));
      //delay(1000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Initiated"));
  mathcalculation();
}

void loop()
{
}
