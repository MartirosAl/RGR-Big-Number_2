#pragma once
#include <stack>
#include "LexicalAnalyzer.h"

void Interpreter(stack<int>& stack_)
{
   //Проверка на наличее ERROR
   for (int i = 0; i < table_tokens.size; i++)
   {
      if (table_tokens.array[i].token_class == ERROR)
      {
         cerr << "Incorrect commands are present";
         return;
      }
   }

   for (int i = 0; i < table_tokens.size; i++)
   {
         
   }
}