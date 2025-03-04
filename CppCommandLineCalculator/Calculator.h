#pragma once
#include "Operation.h"
#include <memory>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <sstream>
#include <exception>

template <typename T>
class Calculator 
{

private:
    std::unordered_map<char, std::unique_ptr<Operation<T>>> operations; // Store pointers
    T leftOperand = 0;
    T rightOperand = 0;
    char operation = '\0';

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

    T Compute() 
    {
        if (operations.count(operation) == 0)
        {
            std::cout << "Error: Invalid operator " << operation << std::endl;
            return T();
        }

        try 
        {
            return operations.at(operation)->Calculate(leftOperand, rightOperand); // Use `->` for pointers
        }

        catch (const std::exception& e) 
        {
			std::cout << "Error: " << e.what() << std::endl;
			return T();
		}

      
    }

    bool ParseExpression(const std::string& input) 
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
        std::cout << input << " " << tokens.size() << std::endl;


        if (tokens.size() == 3) 
        {
            temp1 = tokens[0];
            tempOperator = tokens[1][0];
            temp2 = tokens[2];
        }

        else {
            bool foundOperator = false;
            for (char c : input) 
            {
                std::cout << c << " " << std::endl;

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
                        std::cout << "Error: Multiple operators detected in input.\n";
                        return false;
                    }

                    tempOperator = c;
                    foundOperator = true;
                }

                else 
                {
                    std::cout << "Error: Invalid character detected: " << c << "\n";
                    return false;
                }
            }
        }

        if (tempOperator == '\0')
        {
            std::cout << "Error: No operator found in input.\n";
            return false;
        }

        if (temp1.empty()) 
        {
            std::cout << "Error: Missing first operand.\n";
            return false;
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
                std::cout << "Error: Missing second operand.\n";
                return false;
            }

            operation = tempOperator;
            return true;
        }

        catch (const std::exception& e) 
        {
            std::cout << "Error: Invalid number format " << e.what();
            return false;
        }
    }

};;
