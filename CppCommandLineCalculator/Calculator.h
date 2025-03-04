#pragma once
#include "Operation.h"
#include <memory>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <sstream>
#include <exception>


//determines the result of the expression
enum class ResultType 
{
	Success,
    History,
	Failure
};


template <typename T>
class Calculator 
{

private:

    //stores pointers to the operations
    std::unordered_map<char, std::unique_ptr<Operation<T>>> operations;

    T leftOperand = 0;
    T rightOperand = 0;
    char operation = '\0';

    //used for the history feature
    std::stack<std::string> expressionStack;

public:

    Calculator() 
    {
        operations.emplace('+', std::make_unique<Addition<T>>());
        operations.emplace('-', std::make_unique<Subtraction<T>>());
        operations.emplace('*', std::make_unique<Multiplication<T>>());
        operations.emplace('/', std::make_unique<Division<T>>());
        operations.emplace('^', std::make_unique<Exponentiation<T>>());
        leftOperand = rightOperand = T();
    }

    //Calculates the result of the expression
    T Compute() 
    {
        if (operations.count(operation) == 0)
        {
            std::cout << "Error: Invalid operator " << operation << std::endl;
            return T();
        }

        try 
        {

            T value = operations.at(operation)->Calculate(leftOperand, rightOperand);
            
            //converts the result to a long long if it is a whole number
            long long leftOperandConverted = static_cast<long long>(leftOperand);
            long long rightOperandConverted = static_cast<long long>(rightOperand);
            long long valueConverted = static_cast<long long>(value);

            //converts the result to a string
            std::string leftOperandString = (leftOperand == leftOperandConverted) ? std::to_string(leftOperandConverted) : std::to_string(leftOperand);
            std::string rightOperandString = (rightOperand == rightOperandConverted) ? std::to_string(rightOperandConverted) : std::to_string(rightOperand);
            std::string valueString = (value == valueConverted) ? std::to_string(valueConverted) : std::to_string(value);

            std::string expression = leftOperandString + " " + operation + " " + rightOperandString + " = " + valueString;

            //pushes the expression to the stack
            expressionStack.push(expression);
            return value;
        }

        catch (const std::exception& e) 
        {
			std::cout << "Error: " << e.what() << std::endl;
			return T();
		}

      
    }


    //Parses the input and seperates the operands and operator
    ResultType ParseExpression(const std::string& input)
    {
        std::string temp1, temp2;
        char tempOperator = '\0';

        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;

        while (iss >> token) 
        {
            tokens.push_back(token);
        }


        //cheks if the input is "h" or "H" and returns the history
        if(tokens.size() == 1)
		{
			temp1 = tokens[0];
            if (temp1 == "h" || temp1 == "H")
            {
                return ResultType::History;
            }
		
		}


        if (tokens.size() == 3) 
        {
            temp1 = tokens[0];
            tempOperator = tokens[1][0];
            temp2 = tokens[2];
        }

        //the user may haved entered the expression without spaces
        else {
            bool foundOperator = false;
            for (char c : input) 
            {

                //help craft the operands and operator
                if (std::isdigit(c) || c == '.') 
                {
                    if (!foundOperator) 
                    {
                        temp1 += c;
                    }

                    else 
                    {
                        temp2 += c; 
                    }

                }
                else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') 
                {
                    if (foundOperator)
                    {
                        std::cout << "Error: Multiple operators detected in input." << std::endl;
                        return ResultType::Failure;
                    }

                    tempOperator = c;
                    foundOperator = true;
                }

                else 
                {
                    std::cout << "Error: Invalid character detected: " << c << std::endl;
                    return ResultType::Failure;
                }
            }
        }

        //checks if the operator is missing
        if (tempOperator == '\0')
        {
            std::cout << "Error: No operator found in input." << std::endl;
            return ResultType::Failure;
        }

        //checks if the operand is missing
        if (temp1.empty()) 
        {
            std::cout << "Error: Missing first operand." << std::endl;
            return ResultType::Failure;
        }

        try 
        {
            leftOperand = static_cast<T>(std::stod(temp1));

            if (!temp2.empty()) 
            {
                rightOperand = static_cast<T>(std::stod(temp2));
            }

            else 
            {
                std::cout << "Error: Missing second operand." << std::endl;
                return ResultType::Failure;
            }

            operation = tempOperator;
            return ResultType::Success;
        }

        catch (const std::exception& e) 
        {
            std::cout << "Error: Invalid number format " << e.what() << std::endl;
            return ResultType::Failure;
        }
    }

    std::string RetrievePreviousExpression() 
	{
		if (expressionStack.empty()) 
		{
			return "Empty";
		}

		std::string expression = expressionStack.top();
		expressionStack.pop();
		return expression;
	}

};;
