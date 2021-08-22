/*
   Author : Atanu Kumar Dey
            University of Chittagong
            Department of CSE (19th batch)
            atanukumar251@gmail.com
*/

/**
 * This example shows an implementation of calculator library using serial input and output
 * input like - sqrt((cos(45)/tan(25/5+6*32))*10)
 */



#include <Arduino.h>

#include <Calculator.h>

Calculator calculator; // creating object

char expression[200]; // variable to take expression input
uint8_t exindex = 0;

void mathcalculation()
{
  while (true)
  {
    if (Serial.available())
    {
      delay(60); // waiting sometime to arrive full expression in case it is long
      exindex = 0;
      memset(expression, '\0', sizeof(expression));
      while (Serial.available())
      {
        expression[exindex++] = (char)Serial.read();
      }
      uint32_t times = micros();    // for calulating calculation time

      calculator.getans(expression, exindex);

      Serial.print(F("Total calculation time taken - "));
      Serial.print(micros() - times);
      Serial.println(F(" microsecons"));

      Serial.print(F("Output- "));

      switch (calculator.calculation_data.error)
      {
      case calculator.Syntex_Error:
        Serial.println(F("Syntex Error"));
        break;
      case calculator.Math_Error:
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
