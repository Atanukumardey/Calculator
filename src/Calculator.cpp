#include "Calculator.h"

int8_t calculator::evaluate()
{
  if (isOperator(*Expression))
    numbers.push(0.00);
  char data[char_array_size] ;
  int datacount = 0;
  //memset(data, '\0', sizeof(data));
  for (uint8_t i = 0; i < Length; i++)
  {
    char c = *Expression++;
    char edition = edit(c);
    if (edition)
    {
      //Serial.println(edition);
      switch (edition) {
        case 'e':
          return 1;
        case '+':
        case '-':
          operators.pop(); c = edition; break;
        case '*':
          {
            while (!operators.isEmpty() && priority(edition) <= priority(operators.peek())) {
              operation();
            }
            operators.push(edition); break;
          }
        case '0':
          numbers.push(0.00);
          break;
        default:
          break;
      }
      if (edition == '1') {
        skip = true;
        sign = c;
        edition = '\0';
        continue;
      }
      edition = '\0';
    }

    if ((c >= '0' && c <= '9') || c == '.')
    {
      while ((c >= '0' && c <= '9') || c == '.')
      {
        data[datacount++] = c;
        if (i < Length)
          c = *Expression++, i++;
        else break;
      }
      Expression--, i--;// decresing the counter for last char tha didn't fullfilled the condition
      previous = *(Expression - 1);
      data[datacount] = '\0';
      atod num = atodouble(data, datacount);
      if (!num.problem)
        numbers.push(num.number);
      else
        return 1;
      //memset(data, '\0', sizeof(data));
      datacount = 0;
    }
    else if (isOperator(c)) {

      //1. If current operator has higher precedence than operator on top of the stack,
      //the current operator can be placed in stack
      // 2. else keep popping operator from stack and perform the operation in  numbers stack till
      //either stack is not empty or current operator has higher precedence than operator on top of the stack
      //Serial.println("here1");

      while (!operators.isEmpty() && priority(c) <= priority(operators.peek())) {
        //Serial.println("here2");
        operation();
      }
      //now pushing current operator to stack// also handels the condition that dosen't go through the while loop

      operators.push(c);
    }
    else if (c >= 'a' && c <= 'z')
    {
      while (c >= 'a' && c <= 'z')
      {
        data[datacount++] = c;
        c = *Expression++, i++;
      }
      Expression--, i--; //decresing the counter for last char tha didn't fullfilled the condition
      previous = *(Expression - 1);
      data[datacount] = '\0';
      SpecialFunction(data);
      if (data[0] == '0')
        return 1;
      operators.push(data[0]);
      //memset(data, '\0', sizeof(data));
      datacount = 0;
    }
    else if (c == '(') {
      bracket++;
      operators.push(c);
    }
    else if (c == ')') {
      if (bracket) {
        while (operators.peek() != '(') {
          if (!operation())
            return 2;
        }
        // take out "(" left bracket
        operators.pop();
      }
      else return 1;
    }

  }
  while (!operators.isEmpty()) {
    if (!operation())
      return 2;
  }
  return 0;
}

int8_t calculator::priority(char in)  //better to not use uint8_t to get a nagetive value return
{
  switch (in) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    case '^':
      return 3;
    case '(':
      return -1;
  }
  return 4;
}

void calculator::SpecialFunction(char data[])
{
  if (!strcmp("sin", data))
    data[0] = 's';
  else if (!strcmp("cos", data))
    data[0] = 'c';
  else if (!strcmp("tan", data))
    data[0] = 't';
  else if (!strcmp("asin", data))
    data[0] = 'x';
  else if (!strcmp("acos", data))
    data[0] = 'y';
  else if (!strcmp("atan", data))
    data[0] = 'z';
  else if (!strcmp("sqr", data))
    data[0] = 'q';
  else if (!strcmp("log", data))
    data[0] = 'l';
  else if (!strcmp("loge", data))
    data[0] = 'e';
  else data[0] = '0';
}

bool calculator::operation()
{
  //push ans back to stack

  char operation = operators.pop();
  if (isOperator(operation))
  {
    double a = numbers.pop();
    double b = numbers.pop();
    if (skip) {
      if (sign == '-')
        a = -a;
      skip = false;
      sign = '\0';
    }
    switch (operation) {
      case '+':
        numbers.push( a + b); break;
      case '-':
        numbers.push( b - a); break;
      case '*':
        numbers.push( a * b); break;
      case '/':
        if (a == 0)
        {
          return false;
        }
        else{
          numbers.push( b / a); break;
        }
      case '^':
        Serial.print(a);
        Serial.print(" ");
        Serial.println(b);
        numbers.push(pow(b, a)); break;
      default:
        return false;
    }
  }

  else {
    double a = numbers.pop();
    switch (operation) {
      case 's':
        a = (a * PI) / 180;
        numbers.push(sin(a)); break;
      case 'c':
        a = (a * PI) / 180;
        numbers.push(cos(a)); break;
      case 't':
        a = (a * PI) / 180;
        numbers.push(tan(a)); break;
      case 'x':
        a = asin(a);
        numbers.push((a * 180) / PI); break;
      case 'y':
        a = acos(a);
        numbers.push((a * 180) / PI); break;
      case 'z':
        a = atan(a);
        numbers.push((a * 180) / PI); break;
      case 'l':
        numbers.push(log10(a)); break;
      case 'e':
        numbers.push(log(a)); break;
      case 'q':
        numbers.push(sqrt(a)); break;
      default :
        return false;
    }
  }
  return true;
}


bool calculator::isOperator(char c) {
  return (c == '+' || c == '-' || c == '/' || c == '*' || c == '^');// can try for a switch
}

char calculator::edit(char c)
{
  char temp = '\0';
  if (!previous) {
    previous = c;
    return temp;
  }
  else {
    if (previous == ')' && c == '(')
      temp = '*';
    else if (previous == '(' && (c == '-' || c == '+'))
      temp = '0';
    else if (previous == '^' && (c == '-' || c == '+'))
      temp = '1';
    else if (previous == '+' && c == '+')
      temp = '+';
    else if (previous == '-' && c == '-')
      temp = '+';
    else if (previous == '+' && c == '-')
      temp = '-';
    else if (previous == '-' && c == '+')
      temp = '-';
    else if (((previous >= '0' && previous <= '9') || (c >= 'a' && c <= 'z')) && c == '(')
      temp = '*';
    else if (previous == ')' && ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')))
      temp = '*';
    else if ((previous == '*' || previous == '/') && ((c == '*' || c == '/') || (c == '+' || c == '-'))) //
      temp = 'e';
    else if ((previous == '+' || previous  == '-') && ( c == '*' || c == '/'))
      temp = 'e';
    else
      temp = '\0';
  }
  previous = c;
  return temp ;
}

bool calculator::preedit()
{
  uint8_t i = 0;
  char *last = (Expression + Length - 1);
  while ((*Expression == '/' || *Expression == '*') && (i < Length))
  {
    Expression++, i++;
  }
  if (i == Length)
    return false;

  while (isOperator(*last) && Length)
  {

    Length--; last--;
  }
  if (!Length)
    return false;

  return true;
}

void calculator::getans(char *expressioncomming, uint8_t length_of_expression)
{
  Expression = expressioncomming;

  datatosend.control = 0;
  datatosend.ans = 0.00;
  Length = length_of_expression;

  if (preedit())
    datatosend.control = evaluate();
  else datatosend.control = 1;

  if (!datatosend.control)
    datatosend.ans = numbers.pop();
  bracket = 0;

  Length = 0;
  Expression = NULL;
  previous = '\0';

  //numbers.~StackArray();  // use when making an local object
  //operators.~StackArray(); // will save some memory

  return ;
}


atod calculator::atodouble(char ar[], int size)
{
    atod ans;
    ans.problem = false;
    ans.number = 0.00;
    int dotindex = 0;
    double multiplier = 1;
    int i=0;
    while(ar[i++]!='.'&&i<=size){}
    
      dotindex = i;
      i-=2;
    //Serial.print("i -- ");
    //Serial.println(i);
    while(i>=0)
    {
        //Serial.println(ar[i]);
        ans.number+=(ar[i]-'0')*multiplier;
        multiplier*=10.00;
        i--;
    }
    multiplier = 1;
    while(dotindex<=size&&ar[dotindex]!='\0')
    {
        if(ar[dotindex]=='.')
        {
            ans.problem = true;
            return ans;
        }
        else
        {
            multiplier/=10.00;
            ans.number+=(ar[dotindex++]-'0')*multiplier;
        }
    }
    return ans;

}


/*notes --

   check precedence problem first
   '(' & ')' can make problem in operators stack
   careful about using (c>='a'&&c<='z') in precedence & isOperator function
   check if the string received correctly
   check for overflow

  todo---

   done //checking for ++.. / +-../-- stufs
   done //fixing "2(4+5)25" in appropiate but editable expressinos

*/