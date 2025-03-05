#pragma once
#include "Operation.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <iostream>
#include <sstream>
#include <exception>
#include <type_traits>

//determines the result of the expression
enum class ResultType 
{
	SingleOperandSuccess,
    DoubleOperandSuccess,
    History,
	Failure
};


template <typename T>
class Calculator 
{

private:

    //stores pointers to the operations
    std::unordered_map<char, std::unique_ptr<Operation<T>>> operations;

    //used to check for valid operators
    std::unordered_set<char> validSingleOperators = {'~', 'r' };
    std::unordered_set<char> validDoubleOperators = { '+', '-', '*', '/', '^' };

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
        operations.emplace('~', std::make_unique<Negation<T>>());
        operations.emplace('r', std::make_unique<SquareRoot<T>>());

        leftOperand = rightOperand = T();
    }

    //Calculates the result of the expression
    T Compute() 
    {
        if (!operations.contains(operation))
        {
            std::cout << "Error: Invalid operator " << operation << std::endl;
            return T();
        }

        try 
        {

            T value = T();

            if (validSingleOperators.contains(operation))
            {
                value = operations.at(operation)->Calculate(leftOperand, 0);
            }
            else
            {
                value = operations.at(operation)->Calculate(leftOperand, rightOperand);
            }

           
            //used to convert the operands to long long if they are integers
            long long leftOperandConverted = static_cast<long long>(leftOperand);
            long long rightOperandConverted = static_cast<long long>(rightOperand);
            long long valueConverted = static_cast<long long>(value);

            std::string leftOperandString = (leftOperand == leftOperandConverted) ? std::to_string(leftOperandConverted) : std::to_string(leftOperand);
            std::string rightOperandString = (rightOperand == rightOperandConverted) ? std::to_string(rightOperandConverted) : std::to_string(rightOperand);
            std::string valueString = (value == valueConverted) ? std::to_string(valueConverted) : std::to_string(value);
            std::string expression;

            //used for the history feature, used to create the expression that will be displayed
            if (validSingleOperators.contains(operation))
            {
                expression = std::string(1, operation) + " " + leftOperandString + " = " + valueString;
            }
            else
            {
                expression = leftOperandString + " " + operation + " " + rightOperandString + " = " + valueString;
            }

            expressionStack.push(expression);
            return value;
        }

        catch (const std::exception& e) 
        {
			std::cout << "Error: " << e.what() << std::endl;
			return T();
		}

      
    }


    //used to convert the string to a number for the calculation portion
    template <typename T>
    T ConvertStringToNumber(const std::string& str)
    {

        //used on integers
        if constexpr (std::is_integral_v<T>)
        {
            size_t idx;
            long long value = std::stoll(str, &idx); 
            if (idx != str.size())
            {
                throw std::runtime_error("Invalid integer format: " + str);
            }
            return static_cast<T>(value);
        }

        //used on floating point numbers
        else if constexpr (std::is_floating_point_v<T>)
        {
            size_t idx;
            double value = std::stod(str, &idx);
            if (idx != str.size())
            {
                throw std::runtime_error("Invalid floating-point format: " + str);
            }
            return static_cast<T>(value);
        }
        else
        {
            throw std::runtime_error("Unsupported number type");
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


        if (tokens.size() == 0)
        {
            std::cout << "Error: No input detected." << std::endl;
            return ResultType::Failure;
        }

        //cheks if the input is "h" or "H" and returns the history
        if (tokens.size() == 1)
        {
            if (tokens[0] == "h" || tokens[0] == "H")
            {
                return ResultType::History;
            }

        }

        // Check for a single-operand operator (e.g., ~5 or ~ 5)
        if (tokens.size() == 2 && validSingleOperators.contains(tokens[0][0]))
        {
            tempOperator = tokens[0][0];
            temp1 = tokens[1];

            if (temp1.empty())
            {
                std::cout << "Error: Missing operand for single-operand operator." << std::endl;
                return ResultType::Failure;
            }

            try
            {
                leftOperand = static_cast<T>(std::stod(temp1));
                operation = tempOperator;
                return ResultType::SingleOperandSuccess;
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: Invalid number format: " << e.what() << std::endl;
                return ResultType::Failure;
            }


           
        }

        if (tokens.size() == 3)
        {
            temp1 = tokens[0];
            tempOperator = tokens[1][0];
            temp2 = tokens[2];
          
        }

        //the user may haved entered the expression without spaces
        else
        {
            bool foundSingleOperator = false;
            bool foundDoubleOperator = false;
            for (char c : input)
            {
                if (std::isspace(c))
                {
					continue;
				}

                if (validSingleOperators.contains(c))
                {
                    //handles 5+~5 or ~~5
                    if (foundSingleOperator || foundDoubleOperator)
                    {
                        std::cout << "Error: Multiple operators detected in input." << std::endl;
                        return ResultType::Failure;
                    }

                    //handles 5~
                    if (!temp1.empty())
                    {
                        std::cout << "Used single operator incorrectly" << std::endl;
                        return ResultType::Failure;
                    }

                    foundSingleOperator = true;
                    tempOperator = c;
                }

                else if (validDoubleOperators.contains(c))
                {
                    //handles 5+~5 or ~~5
                    if (foundSingleOperator || foundDoubleOperator)
                    {
                        std::cout << "Error: Multiple operators detected in input." << std::endl;
                        return ResultType::Failure;
                    }

                    foundDoubleOperator = true;
                    tempOperator = c;
                }

                else if (std::isdigit(c) || c == '.')
                {
                    if (foundSingleOperator)
                    {
                        temp1 += c;
                    }

                    //if operators such as + or - is not found
                    //we can assume we are still at the first number
                    else if (!foundDoubleOperator)
                    {
                        temp1 += c;
                    }

                    else
                    {
                        temp2 += c;
                    }
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

        if (!operations.contains(tempOperator))
        {
            std::cout << "Error: Invalid operator " << operation << std::endl;
            return ResultType::Failure;
        }

        //checks if the operand is missing
        if (temp1.empty() && temp2.empty())
        {
            std::cout << "Error: Missing operands." << std::endl;
            return ResultType::Failure;
        }

        try
        {
            operation = tempOperator;

            if (validSingleOperators.contains(tempOperator))
            {
                leftOperand = ConvertStringToNumber<T>(temp1);

                if (!temp2.empty())
                {
					std::cout << "Error: Invalid input for single-operand operator." << std::endl;
					return ResultType::Failure;
				}

                return ResultType::SingleOperandSuccess;
            }

            else
            {
                leftOperand = ConvertStringToNumber<T>(temp1);

                if (!temp2.empty())
                {
                    rightOperand = ConvertStringToNumber<T>(temp2);
                }

                else
                {
                    std::cout << "Error: Missing second operand." << std::endl;
                    return ResultType::Failure;
                }
                return ResultType::DoubleOperandSuccess;
            }


        }

        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            return ResultType::Failure;
        }
    }


    //used for the history mode
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
