#pragma once
#include <stack>
#include "LexicalAnalyzer.h"

void Interpreter(stack<int>& stack_)
{
   //�������� �� ������� ERROR
   for (int i = 0; i < table_tokens.size(); i++)
   {
      if (table_tokens[i].token_class == ERROR)
      {
         cerr << "Incorrect commands are present";
         return;
      }
   }

   for (int i = 0; i < table_tokens.size(); i++)
   {
      ;
   }
}