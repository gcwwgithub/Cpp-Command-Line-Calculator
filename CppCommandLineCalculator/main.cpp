#include <iostream>
#include "Calculator.h"



int main() 
{
	std::cout << "Hello World!" << std::endl;

	Calculator<double> calc;

	while (1) {
		std::string expression;
		std::cout << "Enter an expression: ";
		std::getline(std::cin, expression);

		if (calc.ParseExpression(expression))
		{
			double value = calc.Compute();
			std::cout << "Result: " << value << std::endl;
		}
		else
		{
			std::cerr << "Error: Invalid expression.\n";
		}
	}
	

	return 0;
}