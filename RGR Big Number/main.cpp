#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"

int main()
{
   

   TableToken tokens;

   tokens.Lexical_Analyzer("input7.txt");

   tokens.Print();

   return 0;
}