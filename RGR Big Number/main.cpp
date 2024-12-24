#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"


int main()
{
   
   BigNumber m;

   m.Get_Capacity();

   TableToken tokens;

   tokens.Lexical_Analyzer("input3.txt");

   tokens.Print();

   stack<int> stack_int;

   tokens.Interpreter(stack_int);
   

   return 0;
}