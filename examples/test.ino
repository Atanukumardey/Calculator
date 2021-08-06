#include <Arduino.h>

#include <Calculator.h>

char expression[200];
uint8_t exindex = 0;


void mathans()
{
  calculator Calculator;
  while (true) {
    if (Serial.available())
    {
      delay(45);
      exindex = 0;
      memset(expression,'\0',sizeof(expression));
      while (Serial.available())
      {
        expression [exindex++]=(char) Serial.read();
      }
      uint32_t times = micros();
      Calculator.getans(expression, exindex); 
      Serial.println(micros() - times);
      switch (Calculator.datatosend.control)
      {
        case 1:
          Serial.println(F("Syntex Error")); break;
        case 2:
          Serial.println(F("Math Error")); break;
        default:
          Serial.println(Calculator.datatosend.ans); break;
      }
      Serial.println(F("Input Expression: "));
      //delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("Opened"));
  mathans();
}

void loop() {

}
