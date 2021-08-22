# Arduino Advance Calculator
***
This is an advance calculator library for Arduino like devices. It uses infix evaluation method and some error handling method to calculate.
## Calculator.h
 `calculator.getans()` function takes an `char` array as input and outputs the ans with a structure of type

```
struct calculatordata {
  uint8_t error;
  double  ans;
};
```
where `error` holds error of input array if any and `ans`  holds the answer.

which can be accessed by `calculation_data` objcet from `calculator class`.


## Syntex
  All the basic algebric syntex is same except some.<br/>
  |Syntex|Uses|example|
  |------|----|-------|
  |+, -, * , /| General purpose|2+3, (2/3.5*(66+-9.8)) or 29(5+2)(35)|
  |^| For calulating exponent.|2^5 or 2^(25-6).|
  |sqrt| For calculating square root(`√`) of a number.|sqrt(5) or sqrt5|
  |logn| For calculating `log`. Immediate number used after log will be considerd as base.| log2(3) base is `2` or loge(6) base is `e`.|
  |sin(), cos(), tan(), asin(), acos(), atan() | For trigonomitric calculation and input value will be taken as `**DEGREE**`| cos(45) , cos45, sin(45+60). But equation like sin45+60 will bw considered as sin(45)+60.|  

