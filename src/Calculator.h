/*
   Author : Atanu Kumar Dey
            University of Chittagong
            Department of CSE (19th batch)
            atanukumar251@gmail.com
*/

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include<Arduino.h>
#include<math.h>
#include "../Stack.h"
#include "../config.h"

struct Calculatordata {
  uint8_t error;
  double  ans;
};

struct atod
{
    double number;
    bool problem;
};

class Calculator{

  private:
    uint8_t bracket = 0; //for chacking odd parenthisis
    uint8_t Length = 0;
    char *Expression = NULL;
    char previous = '\0';
    bool skip = false;
    char sign = '\0';

    Stack<double,number_array_size, 1> numbers;
    Stack<char,char_array_size, 2>  operators;

    atod atodouble(char ar[], int size);

    int8_t evaluate();
    int8_t priority(char in);  //better to not use uint8_t to get a nagetive value return
    void function_to_char(char data[]);
    bool operation();
    bool isOperator(char c);
    char edit(char c);
    bool preedit();

  public:
    Calculatordata  calculation_data;
    void getans(char *expressioncomming, uint8_t length_of_expression);
};

#endif