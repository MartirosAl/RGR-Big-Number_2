#include <iostream>
#include "BigNumber.h"
#include "LexicalAnalyzer.h"


int main()
{
   
   BigNumber m;

   m.Get_Capacity();

   TableToken tokens;

   tokens.Lexical_Analyzer("input6.txt");

   tokens.Print();

   

   return 0;
}