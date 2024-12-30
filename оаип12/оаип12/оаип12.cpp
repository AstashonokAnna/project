#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <map>

using namespace std;


int precedence(const string& op) {
    if (op == "+" || op == "-") return 1; 
    if (op == "*" || op == "/") return 2; 
    if (op == "^") return 3;
    return 0;
}


double applyOperation(double a, double b, const string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) throw invalid_argument("Ошибка: деление на ноль");
        return a / b;
    }
    if (op == "^") return pow(a, b);
    throw invalid_argument("Неизвестная операция: " + op);
}


double applyUnaryOperation(double a, const string& op) {
    if (op == "sin") return sin(a);
    if (op == "cos") return cos(a);
    if (op == "tan") return tan(a);
    if (op == "ln") return log(a);
    throw invalid_argument("Неизвестная унарная операция: " + op);
}


double evaluateExpression(const string& expression) {
    stack<double> values; 
    stack<string> ops;   

    for (size_t i = 0; i < expression.length(); i++) {
      
        if (isspace(expression[i])) continue;

      
        if (isdigit(expression[i]) || expression[i] == '-') {
            double value = 0;
            bool isNegative = false;

            if (expression[i] == '-') {
                isNegative = true; 
                i++;
            }

            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) { 
                if (expression[i] == '.') {
                    double decimalPlace = 1;
                    i++;
                    while (i < expression.length() && isdigit(expression[i])) {
                        decimalPlace /= 10;
                        value += (expression[i] - '0') * decimalPlace;
                        i++;
                    }
                    break; 
                }
                value = value * 10 + (expression[i] - '0');
                i++;
            }

            if (isNegative) value = -value; 
            values.push(value);
            i--; 
        }
       
        else if (expression[i] == '(') {
            ops.push("(");
        }
       
        else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != "(") {
                string op = ops.top(); ops.pop();
                double val = values.top(); values.pop();
                if (op == "sin" || op == "cos" || op == "tan" || op == "ln") {
                    values.push(applyUnaryOperation(val, op));
                }
                else {
                    double val2 = values.top(); values.pop();
                    values.push(applyOperation(val2, val, op));
                }
            }
            ops.pop(); 
        }
        
        else {
            string op(1, expression[i]);
            while (i + 1 < expression.length() && isalpha(expression[i + 1])) {
                op += expression[++i];
            }

            while (!ops.empty() && precedence(op) <= precedence(ops.top())) {
                string topOp = ops.top(); ops.pop();
                double val = values.top(); values.pop();
                if (topOp == "sin" || topOp == "cos" || topOp == "tan" || topOp == "ln") {
                    values.push(applyUnaryOperation(val, topOp));
                }
                else {
                    double val2 = values.top(); values.pop();
                    values.push(applyOperation(val2, val, topOp));
                }
            }
            ops.push(op);
        }
    }

    
    while (!ops.empty()) {
        string op = ops.top(); ops.pop();
        double val = values.top(); values.pop();
        if (op == "sin" || op == "cos" || op == "tan" || op == "ln") {
            values.push(applyUnaryOperation(val, op));
        }
        else {
            double val2 = values.top(); values.pop();
            values.push(applyOperation(val2, val, op));
        }
    }

    return values.top(); 
}

int main() {
    setlocale(LC_ALL, "ru");
    string expression;

    cout << "Консольный калькулятор" << endl;

    while (true) {
        cout << " ";
        getline(cin, expression);

        if (expression == "exit") break;

        try {
            double result = evaluateExpression(expression);
            cout << "Результат: " << result << endl;
        }
        catch (const exception& e) {
            cout << e.what() << endl;
        }
    }

    return 0;
}