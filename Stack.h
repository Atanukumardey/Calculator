/*
   Author : Atanu Kumar Dey
            University of Chittagong
            Department of CSE (19th batch)
            atanukumar251@gmail.com
*/

//#ifndef STACK_H
//#define STACK_H

#include <Arduino.h>

/*#ifdef __cplusplus
  extern "C++"{
  #endif*/

template<typename _DATA_TYPE_,uint16_t _size, uint8_t id>
class Stack
{
  private:
    _DATA_TYPE_ arr[_size];       // array to store Stack elements
    //_size                       // maximum capacity of the Stack
    uint16_t top = 0;           // front points to front element in the Stack (if any)

  public:
    Stack();                      // constructor
    ~Stack();                     // destructor
    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;
    bool pop(_DATA_TYPE_ *var);
    _DATA_TYPE_ pop();
    bool push(_DATA_TYPE_ item);
    _DATA_TYPE_ peek();
    int16_t size();
    bool clear();
    bool isEmpty();
    bool isFull();
};

// Constructor to initialize Stack
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
Stack<_DATA_TYPE_, _size, id>::Stack() {}

// Destructor to free memory allocated to the Stack
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
Stack<_DATA_TYPE_, _size, id>::~Stack(){}

// Utility function to remove front element from the Stack
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
bool Stack<_DATA_TYPE_, _size, id>::pop(_DATA_TYPE_ *var)
{
  if (isEmpty())
  {
#ifdef _Stack_DEBUG
   // Serial.println(F("UnderFlow\nProgram Terminated\n"));
#endif
    return false;
  }
  *var = arr[--top];
  return true;
}

template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
_DATA_TYPE_ Stack<_DATA_TYPE_, _size, id>::pop() {
  if(top == 0)
  return 0.00;
  return arr[--top];
}

// Utility function to add an item to the Stack
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
bool Stack<_DATA_TYPE_, _size, id>::push(_DATA_TYPE_ item)
{
  // check for Stack overflow
  if (isFull())
  {
#ifdef _Stack_DEBUG
   // Serial.println(F("OverFlow\nProgram Terminated\n"));
#endif
    return false;
  }
  arr[top++] = item;
  return true;
}

// Utility function to return front element in the Stack
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
_DATA_TYPE_ Stack<_DATA_TYPE_, _size, id>::peek()
{
  if (isEmpty())
  {
#ifdef _Stack_DEBUG
    //Serial.println(F("UnderFlow\nProgram Terminated\n"));
#endif
  }
  return arr[top - 1];
}

// Utility function to return the size of the Stack
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
int16_t Stack<_DATA_TYPE_, _size, id>::size()
{
  return top;
}

// Utility function to check if the Stack is empty or not
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
bool Stack<_DATA_TYPE_, _size, id>::isEmpty()
{
  return (top == 0);
}

// Utility function to check if the Stack is full or not
template<typename _DATA_TYPE_, uint16_t _size, uint8_t id>
bool Stack<_DATA_TYPE_, _size, id>::isFull()
{
  /*Serial.print(top);
  Serial.print("   ");
  Serial.print(_size);
  Serial.print("  id - ");
  Serial.println(id);*/
  return (top == _size);
}
/*#ifdef __cplusplus
  }
  #endif*/

//#endif
