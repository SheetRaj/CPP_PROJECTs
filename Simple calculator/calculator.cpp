#include <iostream>
#include <stdexcept>
using namespace std;

class Solution
{
public:
    double add(double num1, double num2) const
    {
        return num1 + num2;
    }

    double subtract(double num1, double num2) const
    {
        return num1 - num2;
    }

    double multiply(double num1, double num2) const
    {
        return num1 * num2;
    }

    double divide(double num1, double num2) const
    {
        if (num2 == 0)
        {
            throw invalid_argument("Error: Division by zero is not allowed.");
        }

        return num1 / num2;
    }

    double performOperation(double num1, char operation, double num2) const
    {
        switch (operation)
        {
        case '+':
            return add(num1, num2);
            break;

        case '-':
            return subtract(num1, num2);
            break;

        case '*':
            return num1 * num2;
            break;

        case '/':
            return num1 / num2;
            break;

        default:
            throw invalid_argument(" Error : Invalid operator.");
            break;
        }
    }
};

int main()
{
    Solution sol;
    double num1, num2, result;
    char operation;

    try
    {
        cout << "Simple calculator" << endl;

        cout << "Enter first number: ";
        cin >> num1;

        cout << "Enter an operator (+, -, *, /): ";
        cin >> operation;

        cout << "Enter second number: ";
        cin >> num2;

        result = sol.performOperation(num1, operation, num2);

        cout << "Result: " << result << endl;
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}