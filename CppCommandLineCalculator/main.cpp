#include <iostream>
#include "Calculator.h"



int main() 
{


	Calculator<double> calc;
	std::cout << "Hello! Welcome to the Calculator. To use the calculator, type in an expression (Example: 18 + 5) \n" << std::endl;
	std::cout << "Operations allowed: Plus(+), Minus(-), Multiplication(*), Division(/), Exponential(^) \n" << std::endl;
	std::cout << "You may type in h to see the previous expression\n" << std::endl;

	while (1) {
		std::string expression;
		std::cout << "Enter the expression: ";
		std::getline(std::cin, expression);


		switch (calc.ParseExpression(expression))
		{
			case ResultType::Success:
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
				std::cout << "Error: Invalid expression.\n";
				break;
			}

		}


	}
	

	return 0;
}