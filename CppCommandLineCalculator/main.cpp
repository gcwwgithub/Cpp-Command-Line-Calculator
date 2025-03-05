#include <iostream>
#include "Calculator.h"

void TestCalculator()
{
    Calculator<double> calc;


    auto test = [&](const std::string& expression, double expectedValue, bool expectedBooleanResult = true) 
    {
        calc.ParseExpression(expression);
        double result = calc.Compute();

        if ((result != expectedValue && expectedBooleanResult) || (!expectedBooleanResult && result == expectedValue))
        {
            std::cout << "Test failed: " << expression << " = " << result << " (expectedValue: " << expectedValue << ")" << std::endl;
        }

        else
        {
            std::cout << "Test passed: " << expression << " = " << result << std::endl;
        }
    };

    test("5 + 3", 8);
    test("5+3", 8);
    test("10 - 4", 6);
    test("10-4", 6);
    test("7 * 2", 14);
    test("7*2", 14);
    test("9 / 3", 3);
    test("9/3", 3);
    test("2 ^ 3", 8);
    test("2^3", 8);
    test("~5", -5);
    test("r 25", 5);
    test("r25", 5);
    test(" 5+3 ", 8);
    test("10   -2", 8);
    test(" 6  *   3 ", 18);
    test(" 9 /3", 3);
    test("5 +", 0, false);
    test("+ 5", 0, false);
    test("5~", 0, false);
    test("5 ~ 5", 0, false);
    test("abc + 3", 0, false);
    test("5 5 +", 0, false);
}

void RunCalculator()
{
    Calculator<double> calc;
    std::cout << "Hello! Welcome to the Calculator. To use the calculator, type in an expression (Example: 18 + 5) \n" << std::endl;
    std::cout << "Operations allowed: Plus(+), Minus(-), Multiplication(*), Division(/), Exponential(^) \n" << std::endl;
    std::cout << "You may also use: Negation(~), Square Root(r). Its used as ~5 or r5 \n" << std::endl;
    std::cout << "You may type in h to see the previous expression\n" << std::endl;

    while (1) {
        std::string expression;
        std::cout << "Enter the expression: ";
        std::getline(std::cin, expression);


        switch (calc.ParseExpression(expression))
        {
        case ResultType::SingleOperandSuccess:
        {
            double value = calc.Compute();
            std::cout << "Result: " << value << std::endl;
            break;
        }

        case ResultType::DoubleOperandSuccess:
        {
            double value = calc.Compute();
            std::cout << "Result: " << value << std::endl;
            break;
        }
        case ResultType::History:
        {
            std::cout << "History: " << calc.RetrievePreviousExpression() << std::endl;
            break;
        }
        case ResultType::Failure:
        {
            std::cout << "Error: Invalid expression." << std::endl;
            break;
        }

        }


    }
}





int main() 
{
    //TestCalculator();
    RunCalculator();
	
	

	return 0;
}