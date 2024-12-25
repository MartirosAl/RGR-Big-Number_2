#pragma once
#include "LexicalAnalyzer.h"


void TableToken::JUMP(int& number_token)
{
   for (int x = 0; x < table_tokens.size(); x++)
   {
      if (table_tokens[x].number_line == *get<1>(table_tokens[number_token].value))
      {
         number_token = x - 1;//Костыль внутри for станет нужным значением
         return;
      }
   }
}

void TableToken::Interpreter(stack<int>& stack_)
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
         {
            temp_var1 = get<2>(table_tokens[number_token].value)->second;
            stack_.push(temp_var1);
         }
         else
            if (table_tokens[number_token].value.index() == 1)
               stack_.push(*get<1>(table_tokens[number_token].value));

         break;

      case (TokenType::POP):
         
         if (stack_.empty())
         {
            cerr << "Stack usage error";
            return;
         }

         name_variable = get<2>(table_tokens[number_token].value)->first;
         value_variable = stack_.top();
         stack_.pop();

         (get<2>(table_tokens[number_token].value)->second) = value_variable;

         break;
         
      case (TokenType::ARITHMETIC_OPERATION):


         if (stack_.size() <= 1)
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
            if (temp_var1 == 0)
            {
               cerr << "Arithmetic error";
            }
            stack_.push(temp_var2 % temp_var1);
            break;
         }

         break;

      case(TokenType::RELATION):

         if (stack_.size() <= 1)
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

         break;

      case(TokenType::JMP):
      
         JUMP(number_token);
         break;
      
      case(TokenType::JI):

         if (stack_.empty())
         {
            cerr << "Stack usage error";
            return;
         }

         temp_var1 = stack_.top();
         stack_.pop();
         if (temp_var1)
            JUMP(number_token);

         break;
  
      case(TokenType::READ):

         cin >> temp_var1;
         stack_.push(temp_var1);
         

         break;

      case(TokenType::WRITE):
         
         if (stack_.empty())
         {
            cerr << "Stack usage error";
            return;
         }

         cout << stack_.top() << endl;
         stack_.pop();

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

void TableToken::JUMPBN(int& number_token)
{
   for (int x = 0; x < table_tokens_BigNumber.size(); x++)
   {
      if (table_tokens_BigNumber[x].number_line == (int)*get<1>(table_tokens_BigNumber[number_token].value))
      {
         number_token = x - 1;//Костыль. number_token внутри for станет нужным значением
         return;
      }
   }
}

void TableToken::InterpreterBN(stack<BigNumber>& stack_)
{
   //Проверка на наличее ERROR
   for (int i = 0; i < table_tokens_BigNumber.size(); i++)
   {
      if (table_tokens_BigNumber[i].token_class == ERROR)
      {
         cerr << "Incorrect commands are present";
         return;
      }
   }


   string name_variable = " ";
   BigNumber value_variable = 0;
   BigNumber temp_var1 = 0;
   BigNumber temp_var2 = 0;
   BigNumber temp_var3 = 0;



   for (int number_token = 0; number_token < table_tokens_BigNumber.size(); number_token++)
   {
      switch (table_tokens_BigNumber[number_token].token_class)
      {
      case (TokenType::PUSH):

         if (table_tokens_BigNumber[number_token].value.index() == 2)
         {
            temp_var1 = get<2>(table_tokens_BigNumber[number_token].value)->second;
            stack_.push(temp_var1);
         }
         else
            if (table_tokens_BigNumber[number_token].value.index() == 1)
               stack_.push(*get<1>(table_tokens_BigNumber[number_token].value));

         break;

      case (TokenType::POP):

         if (stack_.empty())
         {
            cerr << "Stack usage error";
            return;
         }

         name_variable = get<2>(table_tokens_BigNumber[number_token].value)->first;
         value_variable = stack_.top();
         stack_.pop();

         (get<2>(table_tokens_BigNumber[number_token].value)->second) = value_variable;

         break;

      case (TokenType::ARITHMETIC_OPERATION):


         if (stack_.size() <= 1)
         {
            cerr << "Stack usage error";
            return;
         }

         temp_var1 = stack_.top();
         stack_.pop();
         temp_var2 = stack_.top();
         stack_.pop();

         switch ((int)get<0>(table_tokens_BigNumber[number_token].value))
         {
         case ('+'):
            temp_var3 = (temp_var2 + temp_var1);
            break;
         case ('-'):
            temp_var3 = (temp_var2 - temp_var1);
            break;
         case('*'):
            temp_var3 = (temp_var2 * temp_var1);
            break;
         case('/'):
            if ((int)temp_var1 == 0)
            {
               cerr << "Arithmetic error";
            }
            temp_var3 = (temp_var2 / temp_var1);
            break;
         case ('%'):
            if ((int)temp_var1 == 0)
            {
               cerr << "Arithmetic error";
            }
            temp_var3 = (temp_var2 % temp_var1);
            break;
         }

         stack_.push(temp_var3);

         break;

      case(TokenType::RELATION):

         if (stack_.size() <= 1)
         {
            cerr << "Stack usage error";
            return;
         }

         temp_var1 = stack_.top();
         stack_.pop();
         temp_var2 = stack_.top();
         stack_.pop();

         switch ((int)get<0>(table_tokens_BigNumber[number_token].value))
         {
         case (Equal):
            temp_var3 = temp_var2 == temp_var1;
            break;
         case (Not_equal):
            temp_var3 = temp_var2 != temp_var1;
            break;
         case (Less_then):
            temp_var3 = temp_var2 < temp_var1;
            break;
         case(More_then):
            temp_var3 = temp_var2 > temp_var1;
            break;
         case(Less_or_equal_then):
            temp_var3 = (temp_var2 <= temp_var1);
            break;
         case (More_or_equal_then):
            temp_var3 = (temp_var2 >= temp_var1);
            break;
         }
         stack_.push(temp_var3);

         break;

      case(TokenType::JMP):

         JUMPBN(number_token);
         break;

      case(TokenType::JI):

         if (stack_.empty())
         {
            cerr << "Stack usage error";
            return;
         }

         temp_var1 = stack_.top();
         stack_.pop();
         if (temp_var1)
            JUMPBN(number_token);

         break;

      case(TokenType::READ):

         cin >> temp_var1;
         stack_.push(temp_var1);

         break;

      case(TokenType::WRITE):

         if (stack_.empty())
         {
            cerr << "Stack usage error";
            return;
         }

         cout << stack_.top() << endl;
         stack_.pop();

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