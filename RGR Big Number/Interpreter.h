#pragma once
#include <stack>
#include "LexicalAnalyzer.h"

void JMP(int& number_token)
{
   for (int x = 0; x < table_tokens.size(); x++)
   {
      if (table_tokens[x].number_line == *get<1>(table_tokens[number_token].value))
         number_token = table_tokens[x].number_line;
   }
}

void Interpreter(stack<int>& stack_)
{
   //Проверка на наличее ERROR
   for (int i = 0; i < table_tokens.size(); i++)
   {
      if (table_tokens[i].token_class == ERROR)
      {
         cerr << "Incorrect commands are present";
         return;
      }
   }


   int counter_stack = 0;
   string name_variable = " ";
   int value_variable = 0;
   int temp_var1 = 0;
   int temp_var2 = 0;

   for (int number_token = 0; number_token < table_tokens.size(); number_token++)
   {
      switch (table_tokens[number_token].token_class)
      {
      case (TokenType::PUSH):

         if (table_tokens[number_token].value.index() == 2)
            stack_.push(get<2>(table_tokens[number_token].value)->second);
         else
            if (table_tokens[number_token].value.index() == 1)
               stack_.push(*get<1>(table_tokens[number_token].value));

         counter_stack++;
         break;

      case (TokenType::POP):
         
         if (counter_stack <= 0)
         {
            cerr << "Stack usage error";
            return;
         }

         name_variable = get<2>(table_tokens[number_token].value)->first;
         value_variable = stack_.top();
         stack_.pop();
         table_variable.erase(get<2>(table_tokens[number_token].value));
         table_variable.insert({ name_variable, value_variable });

         counter_stack--;
         break;
         
      case (TokenType::ARITHMETIC_OPERATION):


         if (counter_stack <= 1)
         {
            cerr << "Stack usage error";
            return;
         }

         temp_var1 = stack_.top();
         stack_.pop();
         temp_var2 = stack_.top();
         stack_.pop();

         switch (get<0>(table_tokens[number_token].value))
         {
         case ('+'):
            stack_.push(temp_var2 + temp_var1);
            break;
         case ('-'):
            stack_.push(temp_var2 - temp_var1);
            break;
         case('*'):
            stack_.push(temp_var2 * temp_var1);
            break;
         case('/'):
            if (temp_var1 == 0)
            {
               cerr << "Arithmetic error";
            }
            stack_.push(temp_var2 / temp_var1);
            break;
         case ('%'):
            stack_.push(temp_var2 % temp_var1);
            break;
         }

         counter_stack--;
         break;

      case(TokenType::RELATION):

         if (counter_stack <= 1)
         {
            cerr << "Stack usage error";
            return;
         }

         temp_var1 = stack_.top();
         stack_.pop();
         temp_var2 = stack_.top();
         stack_.pop();

         switch (get<0>(table_tokens[number_token].value))
         {
         case (Equal):
            stack_.push(temp_var2 == temp_var1);
            break;
         case (Not_equal):
            stack_.push(temp_var2 != temp_var1);
            break;
         case (Less_then):
            stack_.push(temp_var2 < temp_var1);
            break;
         case(More_then):
            stack_.push(temp_var2 > temp_var1);
            break;
         case(Less_or_equal_then):
            stack_.push(temp_var2 <= temp_var1);
            break;
         case (More_or_equal_then):
            stack_.push(temp_var2 >= temp_var1);
            break;
         }

         counter_stack--;
         break;

      case(TokenType::JMP):
      
         JMP(number_token);
         break;
      
      case(TokenType::JI):

         if (counter_stack <= 0)
         {
            cerr << "Stack usage error";
            return;
         }

         temp_var1 = stack_.top();
         stack_.pop();
         if (temp_var1)
            JMP(number_token);

         counter_stack--;
         break;
  
      case(TokenType::READ):

         stack_.push(temp_var1);
         cin >> temp_var1;

         counter_stack++;
         break;

      case(TokenType::WRITE):
         
         if (counter_stack <= 0)
         {
            cerr << "Stack usage error";
            return;
         }

         cout << stack_.top();
         stack_.pop();

         counter_stack--;
         break;

      case(TokenType::END):

         return;

      case(TokenType::END_MARKER):

         return;

      default:
         break;
      };
   }
}