#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"


int main()
{


   TableToken tokens;

   tokens.Lexical_Analyzer("input10.txt");

   tokens.Print();

   cout << endl;
   
   stack<variant<int, BigNumber>> stack;

   tokens.Interpreter(stack);



   

   return 0;
}