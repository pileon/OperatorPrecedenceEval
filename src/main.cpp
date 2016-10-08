// This code is placed in the public domain

#include <iostream>
#include <string>
#include <stack>
#include <sstream>         // For input tokenization
#include <unordered_map>   // For operator table
#include <functional>

// GCC complains that the member variables are not initialized by the constructor
// But there's no need to have an explicit constructor as we will initialize the
// member variables inline when creating the instances of the structure
// So, temporarily, disable this complaint (if enabled)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

// This structure describes an operation
struct operation
{
    unsigned precedence;
    std::function<double(double const, double const)> oper;

    // Function to actually evaluate an operation
    double operator()(double const first, double const second)
    {
        return oper(first, second);
    }
};
#pragma GCC diagnostic pop

int main()
{
    // Set up the operator table with its precedences
    // For now we only handle the four basic arithmetic operations
    // This table maps an operator to a corresponding operation structure containing
    // precedence and function to evaluate the operation
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
        char oper = oper_stack.top();
        oper_stack.pop();
        return oper;
    };

    // Pop the value stack and return the value
    auto pop_value = [&value_stack]()
    {
        double value = value_stack.top();
        value_stack.pop();
        return value;
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

        // Perform the operation and push the result
        value_stack.push(operations[oper](first, second));
    };

    // Evaluate *all* operations on the operation stack
    auto evaluate_all = [&]()
    {
        while (!oper_stack.empty())
        {
            evaluate_one();
        }
    };

    // Print a simple prompt
    std::cout << "> ";

    // We read lines, each line is a separate but full expression
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

            // Get the precedence of the operator on top of the operator stack,
            // and the current operator we just read
            unsigned top_precedence = operations[oper_stack.top()].precedence;
            unsigned cur_precedence = operations[oper].precedence;

            if (top_precedence >= cur_precedence)
            {
                // The precedence of the operator on top of the stack was equal to
                // or larger than the precedence of the currently read operator
                // That means we have to evaluate it and get its result before
                // we can push the current operator
                evaluate_one();
            }

            // Push the new operator and value onto the stack
            oper_stack.push(oper);
            value_stack.push(second_value);
        }

        // Evaluate all remaining operations, if there are any
        evaluate_all();

        // Now print it all out
        std::cout << "Result = " << value_stack.top() << '\n';

        // Print the prompt again
        std::cout << "> ";
    }
}
