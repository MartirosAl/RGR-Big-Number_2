#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"


int main()
{

   TableToken tokens;

   tokens.Lexical_Analyzer_int("input7.txt");

   tokens.Print();

   stack<int> stack_int;

   tokens.Interpreter(stack_int);
   

   return 0;
}