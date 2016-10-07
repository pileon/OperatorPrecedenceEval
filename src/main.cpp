// Copyright 2016 Joachim Pileborg
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <string>
#include <stack>
#include <sstream>         // For input tokenization
#include <unordered_map>   // For operator table
#include <functional>

// This structure describes an operation
struct operation
{
    operation()
        : precedence_{0},
          oper_{}
    {}

    operation(unsigned precedence, std::function<double(double const, double const)> oper)
        : precedence_{precedence},
          oper_{oper}
    {}

    unsigned precedence_;
    std::function<double(double const, double const)> oper_;

    double operator()(double const first, double const second)
    {
        return oper_(first, second);
    }
};



int main()
{
    // Set up the operator table with its precedences
    // For now we only handle the four basic arithmetic operations
    std::unordered_map<char, operation> operations = {
        { '+', { 1, [](double const first, double const second) { return first + second; }}},
        { '-', { 1, [](double const first, double const second) { return first - second; }}},
        { '*', { 2, [](double const first, double const second) { return first * second; }}},
        { '/', { 2, [](double const first, double const second) { return first / second; }}}
    };


    std::stack<double> value_stack;  // The value stack
    std::stack<char>   oper_stack;   // The operator stack

    // A few helper functions...

    // Pop the operations stack and return the operation
    auto pop_oper = [&oper_stack]()
    {
        char oper = oper_stack.top();  // Get the top operator
        oper_stack.pop();  // Pop the stack
        return oper;  // Return the operator
    };

    // Pop the value stack and return the value
    auto pop_value = [&value_stack]()
    {
        double value = value_stack.top();  // Get the top value
        value_stack.pop();  // Pop the stack
        return value;  // Return the value
    };

    // Perform a single operation
    auto evaluate_one = [&]()
    {
        // Get the operator and its two operands
        // Note that the order we pop the values is important. For operations
        // such as subtraction there is a difference between `1 - 2` and `2 - 1`.
        // The top value on the stack is always the *second* value pushed on those operations
        char oper = pop_oper();
        double second = pop_value();
        double first = pop_value();

        // Perform the operation
        value_stack.push(operations[oper](first, second));
    };

    auto evaluate_all = [&]()
    {
        while (!oper_stack.empty())
        {
            evaluate_one();
        }
    };

    // We read lines, each line is a separate expression
    std::string input_line;
    while (std::getline(std::cin, input_line))
    {
        // Put the line into an input string stream that we use for the "tokenization"
        // I.e. how we split the input into separate "tokens"
        std::istringstream input{input_line};

        double first_value;   // The first and mandatory single value to read
        double second_value;  // The second value in the optional operator-value pair sequence
        char oper;            // The operator in the optional operator-value pair sequence

        // Read the first value, and push it onto the value stack
        input >> first_value;
        value_stack.push(first_value);

        // Now read the optional sequence of operator-value pairs
        while (input >> oper >> second_value)
        {
            // We have an operator and a second value, do something with it
            if (oper_stack.empty())
            {
                // Operation stack empty, push the operation and the value
                oper_stack.push(oper);
                value_stack.push(second_value);

                continue;  // Continue with the parsing
            }

            // TODO: Continue here!

        }

        // Evaluate all remaining operations
        evaluate_all();

        // Now print it all out
        std::cout << "Result = " << value_stack.top() << '\n';
    }
}