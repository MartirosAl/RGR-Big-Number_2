#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"


int main()
{


   TableToken tokens;

   tokens.Lexical_Analyzer("input1.txt");

   tokens.Print();

   cout << endl << "Input:" << endl;
   
   stack<variant<int, BigNumber>> stack;

   tokens.Interpreter(stack);



   

   return 0;
}