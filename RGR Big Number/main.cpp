#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"
#include "Interpreter.h"

int main()
{
   Lexical_Analyzer("input8.txt");

   TableToken tokens(table_tokens);

   tokens.Print();

   return 0;
}