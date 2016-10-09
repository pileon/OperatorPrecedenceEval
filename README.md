# Operator Precedence Evaluator

This is a simple program I wrote to accompany a blog post I wrote (still not
published) on how to make a simple stack-based operator precedence parser and
evaluator.

While I have written parses and expression evaluators before, I have never
written an operator precedence parser, and I wanted to see if I could make
a simple one that could be easy to understand.

The code is well commented, but for the theory behind my parser and evaluator,
please see [this blog post](https://ghost.pileborg.se/2016/10/08/simple-stack-based-operator-precedence-parser/).

---

The license for the actual code is placed in the public domain. The project
files (for example (but not limited to) the `CMakeLists.txt` files) are mostly
under the Apache License 2.0.

Do with the code as you please but don't blame me (the author) if things go
awry.

---

## Future goals

The parser and evaluator was created with two specific goals: First to see
if I could make a simple and educational operator-precedence parser and
evaluator; Second to make it simple and well-commented so that others could
read the program and learn from it.

While the above two goals are good, and have been reached (I think and hope),
I still feel not satisfied. I feel that could take it a few steps further,
to expand it into a more generic expression parser and evaluator. I know
that I have the knowledge and experience for it. Therefore I have added a
couple of new goals:

* It must be able to handle parenthesized expressions.

  For example is should be able to handle expressions like `3 * (4 - 2) / 6`

* Handle "function" calls.
 
  For example one could add functions likes `abs` to get the absolut value
  of a sub-expression, or trigonometrical functions like `sin` or `cos`
  
Incidentally part of the solution to both the above goals is *recursive
parsers*. If a *sub-expression* is found, wether in a parenthesized expression
or as arguments to a function, we start a new recursive parse round until
the end of the sub-expression.

Further in the future one might add variables and initialization and usage
of those in expressions.

While the step from there to making a parser that could handle more generic
statements isn't far, operator precedence parsers are not the right tool
for that generality. If I have the time I might continue from this base
and make a generic recursive-descent parser for a full (but very simple)
programming language that uses this operator-precedence parser as base
for the expression parsing. More of that for another repository (and
another blog-post).
