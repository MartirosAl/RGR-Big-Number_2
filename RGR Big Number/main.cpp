#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"


int main()
{


   TableToken tokens;

   tokens.Lexical_Analyzer("Программа Даши.txt");

   tokens.Print();

   cout << endl << "Input:" << endl;
   
   stack<variant<int, BigNumber>> stack;

   tokens.PrintStackOnEveryStep();

   tokens.Interpreter(stack);



   

   return 0;
}