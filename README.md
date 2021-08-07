# Arduino Advance Calculator
***
This is an advance calculator library for Arduino like devices. It uses infix evaluation method and some error handling method to calculate. As input ```calculator.getans()``` function takes an array of character and outputs the ans with a structure of type
```
struct calculatordata {
  uint8_t error;
  double  ans;
};
```
where ```error``` holds error of input array if any and ```ans (double)```  holds the answer.
