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
