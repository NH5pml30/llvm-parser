# llvm-parser
Simple translator from floating point expressions with variables to LLVM IR

So far only supports variable declarations with an optional assignment `var a = <expr>;` or `var a;`. `<expr>` may contain assignments and basic arithmetic operations on floating point values and variables (addition, subtraction, multiplication, division, unary plus and minus). The last statement (casted to int32) is the return value from the single generated function.
