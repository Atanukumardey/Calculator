# Calculator
***
This is an advance character based calculator for Arduino. It uses infix evaluation method and some error handling method to calculate. As input ```calculator.getans()``` function takes an array of character and outputs the ans with a structure of type
```
struct calculatordata {
  uint8_t control;
  double  ans;
};
```
where ```control``` holds error of input array if any and ```ans (double)```  holds the answer.
