/*
   Author : Atanu Kumar Dey
            University of Chittagong
            Department of CSE (19th batch)
            atanukumar251@gmail.com
*/

#include "Calculator.h"

/**
 * \brief does the actual caculation (main loop)
 * \param[in]
 */

int8_t Calculator::evaluate()
{
    if (isOperator(*Expression))
        numbers.push(0.00);

    char data[char_array_size];
    int datacount = 0;

    for (uint8_t i = 0; i < Length; i++)
    {
        char character = *Expression++; // character takes first character from input expression
        char edition = edit(character); // edition takes first return from edit function for making changes or to correct the expression
        if (edition)
        {
            //Serial.println(edition);
            switch (edition)
            {
            case 'e':
                return Syntex_Error;
            case '+':
            case '-':
                operators.pop();
                character = edition;
                break;
            case '*':
            {
                while (!operators.isEmpty() && priority(edition) <= priority(operators.peek()))
                {
                    execute_operation();
                }
                operators.push(edition);
                break;
            }
            case '0':
                numbers.push(0.00);
                break;
            default:
                break;
            }
            if (edition == '1')
            {
                skip = true;
                sign = character;
                edition = '\0';
                continue;
            }
            edition = '\0';
        }
        /**
         * collecting number type characters to data variable
         */
        if ((character >= '0' && character <= '9') || character == '.')
        {
            while ((character >= '0' && character <= '9') || character == '.')
            {
                data[datacount++] = character;
                if (i < Length)
                    character = *Expression++, i++;
                else
                    break;
            }

            Expression--, i--; // decresing the counter for last char that didn't fullfilled the condition

            previous = *(Expression - 1);
            data[datacount] = '\0';

            converted_num = atodouble(data, datacount);   // converting number in data[] array to double

            if (!converted_num.problem)
                numbers.push(converted_num.number);
            else
                return Syntex_Error;
            datacount = 0;
        }
        else if (isOperator(character))
        {

            //1. If current operator has higher precedence than operator on top of the stack,
            //the current operator can be placed in stack
            // 2. else keep popping operator from stack and perform the operation in  numbers stack till
            //either stack is not empty or current operator has higher precedence than operator on top of the stack
            //Serial.println("here1");

            while (!operators.isEmpty() && priority(character) <= priority(operators.peek()))
            {
                //Serial.println("place-2");
                execute_operation();
            }
            //now pushing current operator to stack// also handels the condition that dosen't go through the while loop

            operators.push(character);
        }

        /**
         * collecting alphabetic type characters to data variable that can express a function name. like -- log, sin
         */
        else if (character >= 'a' && character <= 'z')
        {
            while (character >= 'a' && character <= 'z')
            {
                data[datacount++] = character;
                character = *Expression++, i++;
            }
            if(data[datacount-1] == 'g') // for handling logn
            {
                if(character >= '0' && character<='9')
                {
                    char logbase[10];
                    uint8_t count_len = 0;
                    while(character >= '0' && character<='9')
                    {
                        logbase[count_len++] = character;
                        character = *Expression++, i++;
                    }
                    //data[datacount++]=character;
                    //character = *Expression++, i++;
                    converted_num = atodouble(logbase, count_len);   // converting number in data[] array to double

                    if (!converted_num.problem)
                        numbers.push(converted_num.number);
                    else
                        return Syntex_Error;
                }
                else
                {
                    numbers.push(10);
                }
            }
            Expression--, i--; //decresing the counter for last char tha didn't fullfilled the condition
            data[datacount] = '\0';

            function_to_char(data);   // converting function name to a single character

            if (data[0] == '0')
                return Syntex_Error;

            operators.push(data[0]);

            previous = data[0];//*(Expression - 1);	// that keep tracks of previous character that helps to compare with next character and correct errors
            //Serial.println(previous);

            //memset(data, '\0', sizeof(data));
            datacount = 0;
        }
        else if (character == '(')
        {
            bracket++;
            operators.push(character);
        }
        else if (character == ')')
        {
            if (bracket)    // if number of bracket(left) is greater than 0
            {
                while (operators.peek() != '(')
                {
                    if (!execute_operation())
                        return Math_Error;
                }
                // take out "(" left bracket
                operators.pop();
            }
            else
                return Syntex_Error;
        }
    }
    while (!operators.isEmpty())
    {
        if (!execute_operation())
            return Math_Error;
    }
    return 0;
}

/**
 * \brief Checks the priority of mathematical operations
 * \param[in] operation
 * \return values from first range (1-3) for general symbols -1 for bracket and 4 for functions
 */

int8_t Calculator::priority(char operation) //better to not use uint8_t to get first nagetive value return
{
    switch (operation)
    {
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

/**
 * \brief Changes the functions name to single character
 * \param[in] data character array of related function name
 * replces data[0] if incoming array matchs any function name
 */

void Calculator::function_to_char(char data[])
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
    else if (!strcmp("sqrt", data))
        data[0] = 'q';
    else if (!strcmp("log", data))
        data[0] = 'l';
    else if (!strcmp("loge", data))
        data[0] = 'e';
    else
        data[0] = '0';  // incase it is not a function defined here
}

/**
 * \brief Does all the arithmetic operatoions and funcational implementation
 * \param[in]
 */

bool Calculator::execute_operation()
{
    //push char_num back to stack

    char operation = operators.pop(); // holds the operation that to be executed
    double first;
    double second;

    if (isOperator(operation))
    {
        first = numbers.pop();
        second = numbers.pop();

        if (skip)
        {
            if (sign == '-')
                first = -first;
            skip = false;
            sign = '\0';
        }
        switch (operation)
        {
        case '+':
            numbers.push(first + second);
            break;
        case '-':
            numbers.push(second - first);
            break;
        case '*':
            numbers.push(first * second);
            break;
        case '/':
            if (first == 0)
            {
                return false;
            }
            else
            {
                numbers.push(second / first);
                break;
            }
        case '^':
            /*Serial.print(first);
              Serial.print(" ");
              Serial.println(second); */
            numbers.push(pow(second, first));
            break;
        default:
            return false;
        }
    }

    else
    {
        first = numbers.pop();
        switch (operation)
        {
        case 's':
            first = (first * PI) / 180;
            numbers.push(sin(first));
            break;
        case 'c':
            first = (first * PI) / 180;
            numbers.push(cos(first));
            break;
        case 't':
            first = (first * PI) / 180;
            numbers.push(tan(first));
            break;
        case 'x':
            first = asin(first);
            numbers.push((first * 180) / PI);
            break;
        case 'y':
            first = acos(first);
            numbers.push((first * 180) / PI);
            break;
        case 'z':
            first = atan(first);
            numbers.push((first * 180) / PI);
            break;
        case 'l':
            second = numbers.pop();
            numbers.push(log(first)/log(second));
            break;
        case 'e':
            numbers.push(log(first));
            break;
        case 'q':
            numbers.push(sqrt(first));
            break;
            break;
        default:
            return false;
        }
    }
    return true;
}

/**
 * \brief Checks if a character is an operator or not
 * \param[in] character to check
 * \return true if character is an operator
 */

bool Calculator::isOperator(char character)
{
    return (character == '+' || character == '-' || character == '/' || character == '*' || character == '^'); // can try for first switch
}

char Calculator::edit(char character)
{
    char temp = '\0';
    if (!previous)
    {
        previous = character;
        return temp;
    }
    else
    {
        if (previous == ')' && character == '(')
            temp = '*';
        else if (previous == '(' && (character == '-' || character == '+'))
            temp = '0';
        else if (previous == '^' && (character == '-' || character == '+'))
            temp = '1';
        else if (previous == '+' && character == '+')
            temp = '+';
        else if (previous == '-' && character == '-')
            temp = '+';
        else if (previous == '+' && character == '-')
            temp = '-';
        else if (previous == '-' && character == '+')
            temp = '-';
        else if (((previous >= '0' && previous <= '9') || (character >= 'a' && character <= 'z')) && character == '(')
            temp = '*';
        else if (previous == ')' && ((character >= '0' && character <= '9') || (character >= 'a' && character <= 'z')))
            temp = '*';
        else if ((previous == '*' || previous == '/') && ((character == '*' || character == '/') || (character == '+' || character == '-'))) //
            temp = 'e';
        else if ((previous == '+' || previous == '-') && (character == '*' || character == '/'))
            temp = 'e';
        else
            temp = '\0';
    }
    previous = character;
    return temp;
}

/**
 * \brief Does any edition into the incoming expression if needed
 * \returns true if edition is successfull
 * \returns false if edition is not successfull
 */

bool Calculator::preedit()
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

        Length--;
        last--;
    }
    if (!Length)
        return false;

    return true;
}

/**
 * \brief Does any edition into the incoming expression if needed
 * \param[in] expressioncomming receives actual expression from user function call
 * \param[in] length_of_expression
 * \returns nothing
 */

void Calculator::getans(char *expressioncomming, uint8_t length_of_expression)
{
    Expression = expressioncomming;

    calculation_data.error = 0;
    calculation_data.ans = 0.00;
    Length = length_of_expression;

    if (preedit())
        calculation_data.error = evaluate();
    else
        calculation_data.error = Syntex_Error;

    if (!calculation_data.error)
        calculation_data.ans = numbers.pop();
    bracket = 0;

    Length = 0;
    Expression = NULL;
    previous = '\0';

    return;
}

/**
 * \brief Converts a character array to double. It can handle inputs with '.' like - ".023" , "1.23", ect
 * \param[in] expressioncomming receives actual expression from user function call
 * \param[in] size
 * \returns a object of atod type
 */

atod Calculator::atodouble(char char_array[], int size)
{
    atod char_num;
    char_num.problem = false;
    char_num.number = 0.00;
    int dotindex = 0;
    double multiplier = 1;
    int i = 0;
    while (char_array[i++] != '.' && i <= size)
    {
    }

    dotindex = i;
    i -= 2;
    //Serial.print("i -- ");
    //Serial.println(i);
    while (i >= 0)
    {
        char_num.number += (char_array[i] - '0') * multiplier;
        multiplier *= 10.00;
        i--;
    }
    multiplier = 1;
    while (dotindex <= size && char_array[dotindex] != '\0')
    {
        if (char_array[dotindex] == '.')
        {
            char_num.problem = true;
            return char_num;
        }
        else
        {
            multiplier /= 10.00;
            char_num.number += (char_array[dotindex++] - '0') * multiplier;
        }
    }
    return char_num;
}

/*notes --

   check precedence problem first
   '(' & ')' can make problem in operators stack
   careful about using (character>='a'&&character<='z') in precedence & isOperator function
   check if the string received correctly
   check for overflow

  todo---

   done //checking for ++.. / +-../-- stufs
   done //fixing "2(4+5)25" in appropiate but editable expressinos

*/
