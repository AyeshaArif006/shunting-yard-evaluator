/*
 * ============================================================
 *  WHAT THIS PROGRAM DOES
 *  Step 1 — TOKENIZE : break the input string into small pieces
 *            called "tokens" (numbers, variable names, operators,
 *            and brackets).
 *
 *  Step 2 — VALIDATE : make sure the tokens are in a sensible
 *            order (e.g. no two operators in a row, brackets match).
 *
 *  Step 3 — INFIX to POSTFIX : rearrange the tokens from the
 *            normal human-readable form (infix)  a + b * c
 *            into postfix form (Reverse Polish Notation):  a b c * +
 *            Using the "Shunting-Yard" algorithm.
 *
 *  Step 4 — GET VARIABLE VALUES : ask the user for a number
 *            for each variable found in the expression.
 *
 *  Step 5 — EVALUATE : compute the answer from the postfix
 *            expression using a simple stack.
 *
 *  Step 6 — OUTPUT : print the postfix expression and result.
 * ============================================================
 */

#include <iostream>   
#include <string>    
#include <vector>     
#include <stack>      
#include <map>        
#include <set>        
#include <cctype>     
using namespace std; 

enum TokenType {
    NUMBER,   
    VARIABLE,  
    OPERATOR,  
    OPEN_BR,   
    CLOSE_BR   
};

struct Token {
    TokenType type;
    string  value;  
};

bool isOpenBracket(char c) {
    return (c == '(' || c == '[' || c == '{');
}

bool isCloseBracket(char c) {
    return (c == ')' || c == ']' || c == '}');
}

char matchingOpen(char closeBr) {
    if (closeBr == ')') return '(';
    if (closeBr == ']') return '[';
    return '{';   // closeBr == '}'
}

int getPrecedence(string op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

vector<Token> tokenize(string expr) {
    vector<Token> tokens;  
    int i = 0;             
    while (i < (int)expr.size()) {

        char c = expr[i];
        // Skip spaces
        if (isspace(c)) {
            i++;
            continue;
        }
        if (isdigit(c)) {
            string num = "";
            while (i < (int)expr.size() && isdigit(expr[i])) {
                num += expr[i];  
                i++;
            }
            tokens.push_back({NUMBER, num});
            continue;
        }
        if (isalpha(c) || c == '_') {
            string name = "";
            while (i < (int)expr.size() &&
                   (isalnum(expr[i]) || expr[i] == '_')) {
                name += expr[i];
                i++;
            }
            tokens.push_back({VARIABLE, name});
            continue;
        }

        if (c == '+' || c == '-' || c == '*' || c == '/') {
            tokens.push_back({OPERATOR, string(1, c)});
            i++;
            continue;
        }

        if (isOpenBracket(c)) {
            tokens.push_back({OPEN_BR, string(1, c)});
            i++;
            continue;
        }

        if (isCloseBracket(c)) {
            tokens.push_back({CLOSE_BR, string(1, c)});
            i++;
            continue;
        }

        cerr << "Syntax error: unknown character '" << c << "'\n";
        exit(1);
    }

    return tokens;
}

void validateTokens(vector<Token>& tokens) {
    stack<char> bracketStack;
    for (int i = 0; i < (int)tokens.size(); i++) {
        Token tok = tokens[i];

        if (tok.type == OPEN_BR) {
            bracketStack.push(tok.value[0]);  
        }
        else if (tok.type == CLOSE_BR) {
            if (bracketStack.empty()) {
                cerr << "Syntax error: closing bracket '"
                     << tok.value << "' has no matching opening bracket\n";
                exit(1);
            }
            char top = bracketStack.top();
            bracketStack.pop();
            if (top != matchingOpen(tok.value[0])) {
                cerr << "Syntax error: bracket mismatch — '"
                     << top << "' closed by '" << tok.value << "'\n";
                exit(1);
            }
        }
    }
    if (!bracketStack.empty()) {
        cerr << "Syntax error: opening bracket '"
             << bracketStack.top() << "' was never closed\n";
        exit(1);
    }

    bool expectingOperand = true;  

    for (int i = 0; i < (int)tokens.size(); i++) {
        Token tok = tokens[i];

        if (expectingOperand) {
            if (tok.type == NUMBER || tok.type == VARIABLE) {
                expectingOperand = false;  
            }
            else if (tok.type == OPEN_BR) {
            }
            else {
                cerr << "Syntax error: unexpected '" << tok.value
                     << "' — expected a number, variable, or '('\n";
                exit(1);
            }
        }
        else {
            
            if (tok.type == OPERATOR) {
                expectingOperand = true;  
            }
            else if (tok.type == CLOSE_BR) {
                
            }
            else {
                cerr << "Syntax error: unexpected '" << tok.value
                     << "' — expected an operator or ')'\n";
                exit(1);
            }
        }
    }

    if (expectingOperand) {
        cerr << "Syntax error: expression ends with an operator or is empty\n";
        exit(1);
    }
    if (tokens.empty()) {
        cerr << "Syntax error: empty expression\n";
        exit(1);
    }
}
vector<Token> infixToPostfix(vector<Token>& tokens) {
    vector<Token> output;     
    stack<Token>  opStack;    

    for (int i = 0; i < (int)tokens.size(); i++) {
        Token tok = tokens[i];

        if (tok.type == NUMBER || tok.type == VARIABLE) {
            output.push_back(tok);
        }

        else if (tok.type == OPERATOR) {
            while (!opStack.empty() &&
                   opStack.top().type == OPERATOR &&
                   getPrecedence(opStack.top().value) >= getPrecedence(tok.value))
            {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(tok);   
        }
        else if (tok.type == OPEN_BR) {
            opStack.push(tok);
        }
        else if (tok.type == CLOSE_BR) {
            while (!opStack.empty() && opStack.top().type != OPEN_BR) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (opStack.empty()) {
                cerr << "Syntax error: mismatched bracket during conversion\n";
                exit(1);
            }
            opStack.pop();   
        }
    }
    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }

    return output;
}

vector<string> getUniqueVariables(vector<Token>& tokens) {
    vector<string> variables;   
    set<string>    seen;        

    for (int i = 0; i < (int)tokens.size(); i++) {
        if (tokens[i].type == VARIABLE) {
            string name = tokens[i].value;
            if (seen.find(name) == seen.end()) {
                seen.insert(name);
                variables.push_back(name);
            }
        }
    }
    return variables;
}
double evaluatePostfix(vector<Token>& postfix, map<string, double>& varValues) {
    stack<double> evalStack;

    for (int i = 0; i < (int)postfix.size(); i++) {
        Token tok = postfix[i];

        
        if (tok.type == NUMBER) {
            double num = stod(tok.value);   
            evalStack.push(num);
        }

        else if (tok.type == VARIABLE) {
            if (varValues.find(tok.value) == varValues.end()) {
                cerr << "Runtime error: no value found for variable '"
                     << tok.value << "'\n";
                exit(2);
            }
            evalStack.push(varValues[tok.value]);
        }

        else if (tok.type == OPERATOR) {
            if ((int)evalStack.size() < 2) {
                cerr << "Runtime error: not enough numbers for operator '"
                     << tok.value << "'\n";
                exit(2);
            }

            double b = evalStack.top(); evalStack.pop();  
            double a = evalStack.top(); evalStack.pop();  

            double result = 0;

            if      (tok.value == "+") result = a + b;
            else if (tok.value == "-") result = a - b;
            else if (tok.value == "*") result = a * b;
            else if (tok.value == "/") {
                
                if (b == 0) {
                    cerr << "Logical error: division by zero\n";
                    exit(3);
                }
                result = a / b;
            }

            evalStack.push(result);
        }
    }

    if (evalStack.size() != 1) {
        cerr << "Runtime error: something went wrong during evaluation\n";
        exit(2);
    }

    return evalStack.top();
}

string postfixToString(vector<Token>& postfix) {
    string result = "";
    for (int i = 0; i < (int)postfix.size(); i++) {
        if (i > 0) result += " ";
        result += postfix[i].value;
    }
    return result;
}
int main() {
    string expression;
    if (!getline(cin, expression)) {
        cerr << "Syntax error: no input provided\n";
        return 1;
    }
    vector<Token> tokens = tokenize(expression);
    validateTokens(tokens);
    vector<Token> postfix = infixToPostfix(tokens);
    string postfixStr = postfixToString(postfix);

    vector<string> variables = getUniqueVariables(tokens);
    map<string, double> varValues;

    for (int i = 0; i < (int)variables.size(); i++) {
        string varName = variables[i];
        double value;
        cerr << "Enter value for " << varName << ": ";
        if (!(cin >> value)) {
            cerr << "Runtime error: invalid number entered for '"
                 << varName << "'\n";
            return 2;
        }
        varValues[varName] = value;
    }
    double result = evaluatePostfix(postfix, varValues);

    cout << postfixStr << "\n";

    if (result == (long long)result)
        cout << (long long)result << "\n";
    else
        cout << result << "\n";

    return 0;  
}
