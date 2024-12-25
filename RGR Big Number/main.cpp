#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"


int main()
{


   TableToken tokens;

   tokens.Lexical_Analyzer_int("input7.txt");

   tokens.Print();

   cout << endl;
   
   stack<int> stack_int;

   tokens.Interpreter(stack_int);

   TableToken tokensBN;

   tokensBN.Lexical_Analyzer_Big_Number("input7.txt");

   tokensBN.PrintBN();

   stack<BigNumber> stack_BigNumber;

   tokensBN.InterpreterBN(stack_BigNumber);

   

   return 0;
}