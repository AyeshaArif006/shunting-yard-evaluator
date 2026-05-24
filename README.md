# shunting-yard-evaluator
A robust C++ Mathematical Expression Parser and Evaluator that tokenizes infix notation, converts it to Postfix (Reverse Polish Notation) using Dijkstra's Shunting Yard algorithm, handles runtime variable bindings and safely computes the result.

# Features

Tokenizes infix expressions into numbers, variables, operators, and brackets.
Supports binary operators: +, -, *, / with correct precedence (*, / > +, -).
Supports all three grouping symbols: (), [], {}.
Accepts variables following C++ identifier rules (start with letter or _, followed by letters/digits/_).
Prompts user for variable values via stderr.
Outputs postfix expression and evaluated result via stdout.
Full error handling with specific exit codes..

# Compilation
g++ -std=c++17 infixtopostfix.cpp -o shunting-yard-evaluator

# The Pipeline Architecture
[ Infix Input ] ──> 1. Tokenize ──> 2. Validate ──> 3. Shunting-Yard (Infix to Postfix)
                                                                │
 [ Terminal Output ] <── 6. Print Output <── 5. Evaluate <── 4. Bind Variables
# Input Format

A single line containing a valid arithmetic infix expression
Operands: integer constants or C++ identifier variables
Operators: +, -, *, /
Grouping: (), [], {}
Whitespace between tokens is optional


# Output Format

stderr: prompts for variable values
stdout: the postfix expression followed by the evaluated numeric result
