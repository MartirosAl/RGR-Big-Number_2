#pragma once
#include "BigNumber.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <map>
#include <set>
#include <stack>
using namespace std;

class TableToken
{
public:

   //ENUMS, STRUCTS, VECTORS//

   enum TokenType
   {
      start = -1,
      LETTER,
      DIGIT,
      ARITHMETIC_OPERATION,
      RELATION,
      SPACE,
      LF,
      SEMI_COLON,
      COMMENT,
      ERROR,
      PUSH,
      POP,
      JMP,
      JI,
      READ,
      WRITE,
      END,
      END_MARKER
   };

   vector<string> TokenTypeString
   {
      "LETTER",
      "DIGIT",
      "ARITHMETIC_OPERATION",
      "RELATION",
      "SPACE",
      "LF",
      "SEMI_COLON",
      "COMMENT",
      "ERROR",
      "PUSH",
      "POP",
      "JMP",
      "JI",
      "READ",
      "WRITE",
      "END",
      "END_MARKER"
   };

   enum States
   {
      A1, A2,
      B1,
      C1,
      D1,
      E1, E2, E3,
      F1, F2, F3,
      G1,
      H1,
      I1, I2,
      J1,
      Stop,
      M1
   };

   enum Relation
   {
      Equal,
      Not,
      Less_then,
      More_then,
      Not_equal,
      Less_or_equal_then,
      More_or_equal_then
   };

   vector<string> RelationString
   {
      "Equal",
      "Not",
      "Less_then",
      "More_then",
      "Not_equal",
      "Less_or_equal_then",
      "More_or_equal_then"
   };

   struct SymbolicToken
   {
      TokenType token_class;
      variant<int, set<int>::iterator, map<string, int>::iterator> value;
      int number_line;
   };

   struct SymbolicTokenforBigNumber
   {
      TokenType token_class;
      variant<BigNumber, set<BigNumber>::iterator, map<string, BigNumber>::iterator> value;
      int number_line;
   };

   struct KeywordDetection
   {
      vector<vector<int>> table_first_vector
      {
         {(int)'e', 0 },
         {(int)'j', 2 },
         {(int)'p', 5 },
         {(int)'r', 10 },
         {(int)'w', 13 }
      };

      vector<vector<int>> table_detection
      {
         {(int)'n', B1b, -1},
         {(int)'d', C1b, -1},
         {(int)'i', E2a, 3},
         {(int)'m', B1b, -1},
         {(int)'p', E2b, -1},
         {(int)'o', B1b, 7},
         {(int)'p', E3a, -1},
         {(int)'u', B1b, -1},
         {(int)'s', B1b, -1},
         {(int)'h', E1a, -1},
         {(int)'e', B1b, -1},
         {(int)'a', B1b, -1},
         {(int)'d', C1c, -1},
         {(int)'r', B1b, -1},
         {(int)'i', B1b, -1},
         {(int)'t', B1b, -1},
         {(int)'e', C1d, -1}
      };

      enum Transitions
      {
         B1b = M1 + 1,
         C1b,
         C1c,
         C1d,
         E1a,
         E2a,
         E2b,
         E3a
      };
   };

   //�������//

   //������� ��������
   set<int> table_constants;

   //������� ����������
   map<string, int> table_variable;

   //������� ������ ��� ������
   vector<SymbolicToken> table_tokens;

   //������� �������� ��� BigNumber
   set<BigNumber> table_constants_BigNumber;

   //������� ����������
   map<string, BigNumber> table_variable_BigNumber;

   //������� ������ BigNumber ��� ������
   vector<SymbolicTokenforBigNumber> table_tokens_BigNumber;

   //������� ��� ����������� �������� ����
   const KeywordDetection table_detection;


   //�������������//

   void JUMP(int& number_token);

   void Interpreter(stack<int>& stack_);

   void JUMPBN(int& number_token);

   void InterpreterBN(stack<BigNumber>& stack_);

   //�������//

   void Print()
   {
      for (int i = 0; i < table_tokens.size(); i++)
      {

         cout << table_tokens[i].number_line << " ";
         cout << TokenTypeString[table_tokens[i].token_class] << " ";
         
         if (table_tokens[i].token_class == TokenType::ARITHMETIC_OPERATION)
         {
            cout << (char)get<0>(table_tokens[i].value) << " ";
         }
         else if (table_tokens[i].token_class == TokenType::RELATION)
         {
            cout << RelationString[get<0>(table_tokens[i].value)] << " ";
         }
         else if (table_tokens[i].token_class == TokenType::COMMENT || table_tokens[i].token_class == TokenType::READ ||
            table_tokens[i].token_class == TokenType::WRITE || table_tokens[i].token_class == TokenType::END ||
            table_tokens[i].token_class == TokenType::END_MARKER || table_tokens[i].token_class == TokenType::ERROR)
            ;//nothing
         else if (table_tokens[i].value.index() == 2)
            cout << get<0>(*get<2>(table_tokens[i].value)) << " ";
         else if (table_tokens[i].value.index() == 1)
            cout << *get<1>(table_tokens[i].value) << " ";
         else if (table_tokens[i].value.index() == 0)
            cout << get<0>(table_tokens[i].value) << " ";

         cout << endl;
      }
   }


   void PrintBN()
   {
      for (int i = 0; i < table_tokens_BigNumber.size(); i++)
      {

         cout << table_tokens_BigNumber[i].number_line << " ";
         cout << TokenTypeString[table_tokens_BigNumber[i].token_class] << " ";

         if (table_tokens_BigNumber[i].token_class == TokenType::ARITHMETIC_OPERATION)
         {
            cout << (char)(int)get<0>(table_tokens_BigNumber[i].value) << " ";
         }
         else if (table_tokens_BigNumber[i].token_class == TokenType::RELATION)
         {
            cout << RelationString[(int)get<0>(table_tokens_BigNumber[i].value)] << " ";
         }
         else if (table_tokens_BigNumber[i].token_class == TokenType::COMMENT || table_tokens_BigNumber[i].token_class == TokenType::READ ||
            table_tokens_BigNumber[i].token_class == TokenType::WRITE || table_tokens_BigNumber[i].token_class == TokenType::END ||
            table_tokens_BigNumber[i].token_class == TokenType::END_MARKER || table_tokens_BigNumber[i].token_class == TokenType::ERROR)
            ;//nothing
         else if (table_tokens_BigNumber[i].value.index() == 2)
            cout << get<0>(*get<2>(table_tokens_BigNumber[i].value)) << " ";
         else if (table_tokens_BigNumber[i].value.index() == 1)
            cout << *get<1>(table_tokens_BigNumber[i].value) << " ";
         else if (table_tokens_BigNumber[i].value.index() == 0)
            cout << get<0>(table_tokens_BigNumber[i].value) << " ";

         cout << endl;
      }
   }





   SymbolicToken Transliterator(int character)
   {
      SymbolicToken result;
      result.value = 0;
      if (character >= 'A' && character <= 'Z' || character >= 'a' && character <= 'z')
      {
         result.token_class = TokenType::LETTER;
         result.value = (int)character;
      }
      else if (character >= '0' && character <= '9')
      {
         result.token_class = TokenType::DIGIT;
         result.value = (int)character - '0';
      }
      else if (character == '+' || character == '-' || character == '*' || character == '/' || character == '%')
      {
         result.token_class = TokenType::ARITHMETIC_OPERATION;
         result.value = (int)character;
      }
      else if (character == '<')
      {
         result.token_class = TokenType::RELATION;
         result.value = Less_then;
      }
      else if (character == '>')
      {
         result.token_class = TokenType::RELATION;
         result.value = More_then;
      }
      else if (character == '=')
      {
         result.token_class = TokenType::RELATION;
         result.value = Equal;
      }
      else if (character == '!')
      {
         result.token_class = TokenType::RELATION;
         result.value = Not;
      }
      else if (character == ' ' || character == '\t')
      {
         result.token_class = TokenType::SPACE;
         result.value = (int)character;
      }
      else if (character == ';')
      {
         result.token_class = TokenType::SEMI_COLON;
         result.value = (int)character;
      }
      else if (character == '\n')
      {
         result.token_class = TokenType::LF;
         result.value = (int)character;
      }
      else if (character == EOF)
      {
         result.token_class = TokenType::END;
         result.value = (int)character;
      }
      else
      {
         result.token_class = TokenType::ERROR;
         result.value = (int)character;
      }
      return result;
   }

   SymbolicTokenforBigNumber TransliteratorBN(int character)
   {
      SymbolicTokenforBigNumber result;
      result.value = 0;
      if (character >= 'A' && character <= 'Z' || character >= 'a' && character <= 'z')
      {
         result.token_class = TokenType::LETTER;
         result.value = (int)character;
      }
      else if (character >= '0' && character <= '9')
      {
         result.token_class = TokenType::DIGIT;
         result.value = (int)character - '0';
      }
      else if (character == '+' || character == '-' || character == '*' || character == '/' || character == '%')
      {
         result.token_class = TokenType::ARITHMETIC_OPERATION;
         result.value = (int)character;
      }
      else if (character == '<')
      {
         result.token_class = TokenType::RELATION;
         result.value = Less_then;
      }
      else if (character == '>')
      {
         result.token_class = TokenType::RELATION;
         result.value = More_then;
      }
      else if (character == '=')
      {
         result.token_class = TokenType::RELATION;
         result.value = Equal;
      }
      else if (character == '!')
      {
         result.token_class = TokenType::RELATION;
         result.value = Not;
      }
      else if (character == ' ' || character == '\t')
      {
         result.token_class = TokenType::SPACE;
      }
      else if (character == ';')
      {
         result.token_class = TokenType::SEMI_COLON;
      }
      else if (character == '\n')
      {
         result.token_class = TokenType::LF;
      }
      else if (character == EOF)
      {
         result.token_class = TokenType::END;
      }
      else
      {
         result.token_class = TokenType::ERROR;
      }
      return result;
   }

   int Find_In_Array_table_first_vector(int character)
   {
      KeywordDetection arrays;
      for (int i = 0; i < 5; i++)
      {
         if (arrays.table_first_vector[i][0] == character)
            return arrays.table_first_vector[i][1];
      }
      return -1;
   }
   
   bool Is_Keyword(string word)
   {
      int temp = Find_In_Array_table_first_vector(word[0]);

      if (temp == -1 || word.length() == 1)
         return false;

      for (int i = 1; i < word.length(); i++)
      {
         if (table_detection.table_detection[temp][0] != word[i])
         {
            if (table_detection.table_detection[temp][2] != -1 && table_detection.table_detection[table_detection.table_detection[temp][2]][0] == word[i])
            {
               temp = table_detection.table_detection[temp][2];
               continue;
            }
            else
            {
               return false;
            }
         }

         temp++;
         if (table_detection.table_detection[temp][2] != KeywordDetection::B1b && word.length() > temp)
            return false;
      }

      return true;
   }

   //��������� ��������_���������
   void Add_Constant()
   {
      table_constants.emplace(register_number);

      register_value = register_number;

      register_indicator = table_constants.find(register_number);
   }

   //��������� ��������_��������� ��� BigNumber
   void Add_ConstantBN()
   {
      table_constants_BigNumber.emplace(register_big_number);

      register_value_big_number = register_big_number;

      register_indicator_for_Big_Number = table_constants_BigNumber.find(register_big_number);
   }

   //��������� �������_������� �����
   void Create_Token()
   {
      SymbolicToken result;
      
      if (register_type_token == TokenType::RELATION)
         result.value = register_relation;
      else if (register_type_token == TokenType::ARITHMETIC_OPERATION)
         result.value = register_value;
      else if (register_indicator.index() == 1)
      {
         result.value = get<1>(register_indicator);
      }
      else if (register_indicator.index() == 0)
      {
         result.value = get<0>(register_indicator);
      }
      result.token_class = register_type_token;
      result.number_line = number_line;

      table_tokens.push_back(result);

      register_value = -1;
   }

   //��������� �������_������� ��� BigNumber
   void Create_TokenBN()
   {
      SymbolicTokenforBigNumber result;

      if (register_type_token == TokenType::RELATION)
         result.value = register_relation;
      else if (register_type_token == TokenType::ARITHMETIC_OPERATION)
         result.value = register_value;
      else if (register_indicator_for_Big_Number.index() == 1)
      {
         result.value = get<1>(register_indicator_for_Big_Number);
      }
      else if (register_indicator_for_Big_Number.index() == 0)
      {
         result.value = get<0>(register_indicator_for_Big_Number);
      }
      result.token_class = register_type_token;
      result.number_line = number_line;

      table_tokens_BigNumber.push_back(result);

      register_value = -1;
   }

   //��������� ��������� ������
   void Error_Handler()
   {
      register_type_token = TokenType::ERROR;
      Create_Token();
      cerr << "An error was found in the number line " << number_line << endl;
      state = J1;
   }

   //��������� ��������� ������ ��� BigNumber
   void Error_HandlerBN()
   {
      register_type_token = TokenType::ERROR;
      Create_TokenBN();
      cerr << "An error was found in the number line " << number_line << endl;
      state = J1;
   }

   //��������� ��������_����������
   void Add_Variable()
   {
      
      if (Is_Keyword(register_variable))
      {
         Error_Handler();
         return;
      }

      if (table_variable.count(register_variable) == 0)
      {
         table_variable[register_variable] = 0;
      }

      register_indicator = table_variable.find(register_variable);
   }

   //��������� ��������_���������� ��� BigNumber
   void Add_VariableBN()
   {

      if (Is_Keyword(register_variable))
      {
         Error_HandlerBN();
         return;
      }

      if (table_variable_BigNumber.count(register_variable) == 0)
      {
         table_variable_BigNumber[register_variable] = 0;
      }

      register_indicator_for_Big_Number = table_variable_BigNumber.find(register_variable);
   }
   

   //����������//

   //������� ������ ������ ��� �������� ������ �������
   TokenType register_type_token;

   //������� ��������� �������� ��������� �� ������� ��� ��� ������ PUSH � POP
   variant<set<int>::iterator, map<string, int>::iterator> register_indicator;

   //������� ����� ������������ ��� ���������� ��������
   int register_number;

   //������� ��������� ������ ���������� � ������ ������� ���������
   int register_relation;

   //������� ���������� ����������� ��� ����������
   string register_variable;

   //������� ����������� ������ ����� ������� � ������� ����������� ��� ������ �������� ����
   int register_detection;

   //������� �������� ������ �������� ������
   int register_value = -1;

   //����� ������ ������ ����� ������� ������ � ���������
   int number_line = 1;

   //���� ���������
   bool stop = false;

   //���������� ��� M1
   int tempforswitchinM1;

   //������� ����� ������������ ��� ���������� �������� ���� BigNumber
   BigNumber register_big_number;

   //������� ��������� �������� ��������� �� ������� ��� ��� ������ PUSH � POP ��� BigNumber
   variant<set<BigNumber>::iterator, map<string, BigNumber>::iterator> register_indicator_for_Big_Number;

   //������� �������� ������ �������� ������ BigNumber
   BigNumber register_value_big_number;

   int state = A1;

   SymbolicToken token;

   SymbolicTokenforBigNumber tokenBN;


   //�������//

   void A1a()
   {
      Create_Token();
      number_line++;

      state = A1;
   }

   void A1b()
   {
      number_line++;

      state = A1;
   }
   
   void A2a()
   {
      number_line++;

      state = A2;
   }

   void A2b()
   {
      Create_Token();
      number_line++;

      state = A2;
   }

   void A2c()
   {
      Add_Constant();
      Create_Token();
      number_line++;

      state = A2;
   }

   void A2d()
   {
      Add_Variable();
      Create_Token();
      number_line++;

      state = A2;
   }
   

   void A2e()
   {
      state = A2;

      if (register_relation == Not)
      {
         Error_Handler();
         return;
      }
      Create_Token();
      number_line++;

   }

   void A2f()
   {
      Error_Handler();
      number_line++;

      state = A2;
   }

   void B1a()
   {
      state = B1;
      register_detection = Find_In_Array_table_first_vector(get<0>(token.value));
      if (register_detection == -1)
      {
         Error_Handler();
      }
   }

   //B1b

   void C1a()
   {
      register_type_token = TokenType::ARITHMETIC_OPERATION;
      register_value = (int)get<0>(token.value);
      Create_Token();

      state = C1;
   }

   //C1b
   //C1c
   //C1d

   void C1e()
   {
      Add_Constant();
      Create_Token();

      state = C1;
   }

   void C1f()
   {
      Add_Variable();
      Create_Token();

      state = C1;
   }

   void C1g()
   {
      state = C1;
      
      if (register_relation == Not)
      {
         Error_Handler();
         return;
      }
      Create_Token();

   }

   void C1h()
   {
      state = C1;

      if (get<0>(token.value) != Equal)
      {
         Error_Handler();
         return;
      }
      if (register_relation >= Not && register_relation <= More_then)
         register_relation += 3;//Not + 3 = Not_equal, Less + 3 = Less_or_equal, More + 3 = More_or_equal
      Create_Token();
   }

   void D1a()
   {
      register_type_token = TokenType::RELATION;
      register_relation = get<0>(token.value);

      state = D1;
   }

   //E1a
   //E2a
   //E2b
   //E3a

   void G1a()
   {
      register_number = get<0>(token.value);

      state = G1;
   }

   void G1b()
   {
      register_number *= 10;
      register_number += get<0>(token.value);

      state = G1;
   }

   void H1a()
   {
      register_variable = get<0>(token.value);

      state = H1;
   }

   void H1b()
   {
      register_variable.push_back(get<0>(token.value));

      state = H1;
   }

   void I1a()
   {
      register_type_token = TokenType::COMMENT;

      state = I1;
   }

   void I2a()
   {
      register_type_token = TokenType::COMMENT;

      state = I2;
   }

   void I2b()
   {
      Add_Constant();
      Create_Token();
      register_type_token = TokenType::COMMENT;

      state = I2;
   }

   void I2c()
   {
      Add_Variable();
      Create_Token();
      register_type_token = TokenType::COMMENT;

      state = I2;
   }

   void I2d()
   {
      state = I2;
      
      if (register_relation == Not)
      {
         Error_Handler();
         return;
      }
      Create_Token();
      register_type_token = TokenType::COMMENT;

   }

   void EXIT1()
   {
      register_type_token = TokenType::END_MARKER;
      Create_Token();

      state = Stop;
   }

   void EXIT2()
   {
      state = Stop;

      if (register_relation == Not)
      {
         register_type_token = TokenType::ERROR;
      }
      Create_Token();
      register_type_token = TokenType::END_MARKER;
      Create_Token();
   }

   void EXIT3()
   {
      Add_Constant();
      Create_Token();
      register_type_token = TokenType::END_MARKER;
      Create_Token();

      state = Stop;
   }

   void EXIT4()
   {
      Add_Variable();
      Create_Token();
      register_type_token = TokenType::END_MARKER;
      Create_Token();

      state = Stop;
   }

   void EXIT5()
   {
      Create_Token();
      register_type_token = TokenType::END_MARKER;
      Create_Token();

      state = Stop;
   }

   void M_1()
   {
      if (register_detection == -1)
         return;
      tempforswitchinM1 = table_detection.table_detection[register_detection][1];
      if (table_detection.table_detection[register_detection][0] != get<0>(token.value))
      {
         if (table_detection.table_detection[register_detection][2] != -1 && table_detection.table_detection[table_detection.table_detection[register_detection][2]][0] == get<0>(token.value))
         {
            tempforswitchinM1 = table_detection.table_detection[table_detection.table_detection[register_detection][2]][1];
            register_detection = table_detection.table_detection[register_detection][2];
         }
         else
         {
            Error_Handler();
            return;
         }
      }

      switch (tempforswitchinM1)
      {
      case(table_detection.B1b)://B1b

         register_detection++;

         state = B1;
         break;

      case(table_detection.C1b)://C1b

         register_type_token = TokenType::END;
         Create_Token();

         state = C1;
         break;

      case(table_detection.C1c)://C1c

         register_type_token = TokenType::READ;
         Create_Token();

         state = C1;
         break;

      case(table_detection.C1d)://C1d

         register_type_token = TokenType::WRITE;
         Create_Token();

         state = C1;
         break;

      case(table_detection.E1a)://E1a

         register_type_token = TokenType::PUSH;

         state = E1;
         break;

      case(table_detection.E2a)://E2a

         register_type_token = TokenType::JI;

         state = E2;
         break;

      case(table_detection.E2b)://E2b

         register_type_token = TokenType::JMP;

         state = E2;
         break;

      case(table_detection.E3a)://E3a

         register_type_token = TokenType::POP;

         state = E3;
         break;

      default:
         Error_Handler();
         break;
      }
   }


   void A1aBN()
   {
      Create_TokenBN();
      number_line++;

      state = A1;
   }


   void A2bBN()
   {
      Create_TokenBN();
      number_line++;

      state = A2;
   }

   void A2cBN()
   {
      Add_ConstantBN();
      Create_TokenBN();
      number_line++;

      state = A2;
   }

   void A2dBN()
   {
      Add_VariableBN();
      Create_TokenBN();
      number_line++;

      state = A2;
   }


   void A2eBN()
   {
      state = A2;

      if (register_relation == Not)
      {
         Error_HandlerBN();
         return;
      }
      Create_TokenBN();
      number_line++;

   }

   void A2fBN()
   {
      Error_HandlerBN();
      number_line++;

      state = A2;
   }

   void B1aBN()
   {
      state = B1;
      register_detection = Find_In_Array_table_first_vector((int)get<0>(tokenBN.value));
      if (register_detection == -1)
      {
         Error_HandlerBN();
      }
   }

   //B1bBN

   void C1aBN()
   {
      register_type_token = TokenType::ARITHMETIC_OPERATION;
      register_value = (int)get<0>(tokenBN.value);
      Create_TokenBN();

      state = C1;
   }

   //C1bBN
   //C1cBN
   //C1dBN

   void C1eBN()
   {
      Add_ConstantBN();
      Create_TokenBN();

      state = C1;
   }

   void C1fBN()
   {
      Add_VariableBN();
      Create_TokenBN();

      state = C1;
   }

   void C1gBN()
   {
      state = C1;

      if (register_relation == Not)
      {
         Error_HandlerBN();
         return;
      }
      Create_TokenBN();

   }

   void C1hBN()
   {
      state = C1;

      if ((int)get<0>(tokenBN.value) != (int)Equal)
      {
         Error_HandlerBN();
         return;
      }
      if (register_relation >= Not && register_relation <= More_then)
         register_relation += 3;//Not + 3 = Not_equal, Less + 3 = Less_or_equal, More + 3 = More_or_equal
      Create_TokenBN();
   }

   void D1aBN()
   {
      register_type_token = TokenType::RELATION;
      register_relation = (int)get<0>(tokenBN.value);

      state = D1;
   }

   //E1aBN
   //E2aBN
   //E2bBN
   //E3aBN

   void G1aBN()
   {
      register_big_number = get<0>(tokenBN.value);

      state = G1;
   }

   void G1bBN()
   {
      short ten_s[] = {0, 1};
      BigNumber ten(ten_s, 2);
      register_big_number = (register_big_number * ten) + get<0>(tokenBN.value);

      state = G1;
   }

   void H1aBN()
   {
      register_variable = (int)get<0>(tokenBN.value);

      state = H1;
   }

   void H1bBN()
   {
      register_variable.push_back((int)get<0>(tokenBN.value));

      state = H1;
   }

   void I1aBN()
   {
      register_type_token = TokenType::COMMENT;

      state = I1;
   }

   void I2aBN()
   {
      register_type_token = TokenType::COMMENT;

      state = I2;
   }

   void I2bBN()
   {
      Add_ConstantBN();
      Create_TokenBN();
      register_type_token = TokenType::COMMENT;

      state = I2;
   }

   void I2cBN()
   {
      Add_VariableBN();
      Create_TokenBN();
      register_type_token = TokenType::COMMENT;

      state = I2;
   }

   void I2dBN()
   {
      state = I2;

      if (register_relation == Not)
      {
         Error_HandlerBN();
         return;
      }
      Create_TokenBN();
      register_type_token = TokenType::COMMENT;

   }

   void EXIT1BN()
   {
      register_type_token = TokenType::END_MARKER;
      Create_TokenBN();

      state = Stop;
   }

   void EXIT2BN()
   {
      state = Stop;

      if (register_relation == Not)
      {
         register_type_token = TokenType::ERROR;
      }
      Create_TokenBN();
      register_type_token = TokenType::END_MARKER;
      Create_TokenBN();
   }

   void EXIT3BN()
   {
      Add_ConstantBN();
      Create_TokenBN();
      register_type_token = TokenType::END_MARKER;
      Create_TokenBN();

      state = Stop;
   }

   void EXIT4BN()
   {
      Add_VariableBN();
      Create_TokenBN();
      register_type_token = TokenType::END_MARKER;
      Create_TokenBN();

      state = Stop;
   }

   void EXIT5BN()
   {
      Create_TokenBN();
      register_type_token = TokenType::END_MARKER;
      Create_TokenBN();

      state = Stop;
   }

   void M_1BN()
   {
      if (register_detection == -1)
         return;
      tempforswitchinM1 = table_detection.table_detection[register_detection][1];
      if (table_detection.table_detection[register_detection][0] != (int)get<0>(tokenBN.value))
      {
         if (table_detection.table_detection[register_detection][2] != -1 && 
            table_detection.table_detection[table_detection.table_detection[register_detection][2]][0] == (int)get<0>(tokenBN.value))
         {
            tempforswitchinM1 = table_detection.table_detection[table_detection.table_detection[register_detection][2]][1];
            register_detection = table_detection.table_detection[register_detection][2];
         }
         else
         {
            Error_HandlerBN();
            return;
         }
      }

      switch (tempforswitchinM1)
      {
      case(table_detection.B1b)://B1b

         register_detection++;

         state = B1;
         break;

      case(table_detection.C1b)://C1b

         register_type_token = TokenType::END;
         Create_TokenBN();

         state = C1;
         break;

      case(table_detection.C1c)://C1c

         register_type_token = TokenType::READ;
         Create_TokenBN();

         state = C1;
         break;

      case(table_detection.C1d)://C1d

         register_type_token = TokenType::WRITE;
         Create_TokenBN();

         state = C1;
         break;

      case(table_detection.E1a)://E1a

         register_type_token = TokenType::PUSH;

         state = E1;
         break;

      case(table_detection.E2a)://E2a

         register_type_token = TokenType::JI;

         state = E2;
         break;

      case(table_detection.E2b)://E2b

         register_type_token = TokenType::JMP;

         state = E2;
         break;

      case(table_detection.E3a)://E3a

         register_type_token = TokenType::POP;

         state = E3;
         break;

      default:
         Error_HandlerBN();
         break;
      }
   }

   //����������� ���������� ��� int//

   vector<SymbolicToken> Lexical_Analyzer_int(const char* filename)
   {
      ifstream in(filename);
      if (!in)
      {
         cout << "�� ������� ������� ���� " << filename << endl;
         throw "Error";
      }


      while (!stop)
      {
         int character = in.get();
         token = Transliterator(character);

         switch (state)
         {

            ////////////////////////////////////////////////////////////////////////////A1
         case (A1):
            switch (token.token_class)
            {
            case (TokenType::LETTER)://B1a

               B1a();
               break;

            case (TokenType::ARITHMETIC_OPERATION)://C1a

               C1a();
               break;

            case (TokenType::RELATION)://D1a

               D1a();
               break;

            case (TokenType::SPACE)://A1

               state = A1;
               break;

            case (TokenType::LF)://A1b

               A1b();
               break;

            case (TokenType::SEMI_COLON)://I1a

               I1a();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////A2
         case (A2):

            switch (token.token_class)
            {
            case (TokenType::LETTER)://B1a

               B1a();
               break;

            case (TokenType::ARITHMETIC_OPERATION)://C1a

               C1a();
               break;

            case (TokenType::RELATION)://D1a

               D1a();
               break;

            case (TokenType::SPACE)://A2

               state = A2;
               break;

            case (TokenType::LF)://A2a

               A2a();
               break;

            case (TokenType::SEMI_COLON)://I2a

               I2a();
               break;

            case (TokenType::END)://EXIT1

               EXIT1();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////B1
         case(B1):

            switch (token.token_class)
            {
            case (TokenType::LETTER)://M1

               M_1();
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////C1
         case(C1):

            switch (token.token_class)
            {
            case (TokenType::SPACE)://C1

               state = C1;
               break;

            case (TokenType::LF)://A2a

               A2a();
               break;

            case (TokenType::SEMI_COLON)://I2a

               I2a();
               break;

            case (TokenType::END)://EXIT1

               EXIT1();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////D1
         case (D1):

            switch (token.token_class)
            {
            case (TokenType::RELATION)://C1h

               C1h();
               break;

            case (TokenType::SPACE)://C1g

               C1g();
               break;

            case (TokenType::LF)://A2e

               A2e();
               break;

            case (TokenType::SEMI_COLON)://I2d

               I2d();
               break;

            case (TokenType::END)://EXIT2

               EXIT2();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////E1
         case (E1):

            switch (token.token_class)
            {
            case (TokenType::SPACE)://F1

               state = F1;
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////E1
         case (E2):

            switch (token.token_class)
            {
            case (TokenType::SPACE)://F2

               state = F2;
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////E3
         case (E3):

            switch (token.token_class)
            {
            case (TokenType::SPACE)://F3

               state = F3;
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////F1
         case(F1):

            switch (token.token_class)
            {
            case (TokenType::LETTER)://H1a

               H1a();
               break;

            case (TokenType::DIGIT)://G1a

               G1a();
               break;

            case (TokenType::SPACE)://F1

               state = F1;
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////F2
         case(F2):

            switch (token.token_class)
            {
            case (TokenType::DIGIT)://G1a

               G1a();
               break;

            case (TokenType::SPACE)://F2

               state = F2;
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////F3
         case(F3):

            switch (token.token_class)
            {
            case (TokenType::LETTER)://H1a

               H1a();
               break;

            case (TokenType::SPACE)://F3

               state = F3;
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////G1
         case(G1):

            switch (token.token_class)
            {
            case (TokenType::DIGIT)://G1b

               G1b();
               break;

            case (TokenType::SPACE)://C1e

               C1e();
               break;

            case (TokenType::LF)://A2c

               A2c();
               break;

            case (TokenType::SEMI_COLON)://I2b

               I2b();
               break;

            case (TokenType::END)://EXIT3

               EXIT3();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////H1
         case(H1):

            switch (token.token_class)
            {
            case (TokenType::LETTER)://H1b

               H1b();
               break;

            case (TokenType::DIGIT)://H1b

               H1b();
               break;

            case (TokenType::SPACE)://C1f

               C1f();
               break;

            case (TokenType::LF)://A2d

               A2d();
               break;

            case (TokenType::SEMI_COLON)://I2c

               I2c();
               break;

            case (TokenType::END)://EXIT4

               EXIT4();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////I1
         case(I1):

            switch (token.token_class)
            {

            case (TokenType::LETTER)://I1

               state = I1;
               break;

            case (TokenType::DIGIT)://I1

               state = I1;
               break;

            case (TokenType::ARITHMETIC_OPERATION)://I1

               state = I1;
               break;

            case (TokenType::RELATION)://I1

               state = I1;
               break;

            case (TokenType::SPACE)://I1

               state = I1;
               break;

            case (TokenType::LF)://A1a

               A1a();
               break;

            case(TokenType::SEMI_COLON)://I1

               state = I1;
               break;

            case (TokenType::ERROR)://I1

               state = I1;
               break;

            case(TokenType::END)://EXIT1

               EXIT5();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////I1
         case(I2):

            switch (token.token_class)
            {
            case(TokenType::LETTER)://I2

               state = I2;
               break;

            case(TokenType::DIGIT)://I2

               state = I2;
               break;

            case(TokenType::ARITHMETIC_OPERATION)://I2

               state = I2;
               break;

            case(TokenType::RELATION)://I2

               state = I2;
               break;

            case(TokenType::SPACE)://I2

               state = I2;
               break;

            case(TokenType::LF)://A2b

               A2b();
               break;

            case(TokenType::SEMI_COLON)://I2

               state = I2;
               break;

            case(TokenType::ERROR)://I2

               state = I2;
               break;

            case(TokenType::END)://EXIT1

               EXIT5();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////J1
         case(J1):

            switch (token.token_class)
            {
            case (TokenType::LETTER)://J1

               state = J1;
               break;

            case (TokenType::DIGIT)://J1

               state = J1;
               break;

            case (TokenType::ARITHMETIC_OPERATION)://J1

               state = J1;
               break;

            case (TokenType::RELATION)://J1

               state = J1;
               break;

            case (TokenType::SPACE)://J1

               state = J1;
               break;

            case (TokenType::LF)://A2a

               A2a();
               break;

            case (TokenType::SEMI_COLON)://J1

               state = J1;
               break;

            case (TokenType::ERROR)://J1

               state = J1;
               break;

            case (TokenType::END)://EXIT5

               EXIT5();
               break;

            default:
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////Stop    
         case(Stop):
            stop = true;
            break;

         default:
            Error_Handler();
            break;
         }
      }
      return table_tokens;
   }



   //������������� ���������� ��� BigNumber//

   vector<SymbolicTokenforBigNumber> Lexical_Analyzer_Big_Number(const char* filename)
   {
      ifstream in(filename);
      if (!in)
      {
         cout << "�� ������� ������� ���� " << filename << endl;
         throw "Error";
      }


      while (!stop)
      {
         int character = in.get();
         tokenBN = TransliteratorBN(character);

         switch (state)
         {

            ////////////////////////////////////////////////////////////////////////////A1
         case (A1):
            switch (tokenBN.token_class)
            {
            case (TokenType::LETTER)://B1a

               B1aBN();
               break;

            case (TokenType::ARITHMETIC_OPERATION)://C1a

               C1aBN();
               break;

            case (TokenType::RELATION)://D1a

               D1aBN();
               break;

            case (TokenType::SPACE)://A1

               state = A1;
               break;

            case (TokenType::LF)://A1b

               A1b();
               break;

            case (TokenType::SEMI_COLON)://I1a

               I1aBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////A2
         case (A2):

            switch (tokenBN.token_class)
            {
            case (TokenType::LETTER)://B1a

               B1aBN();
               break;

            case (TokenType::ARITHMETIC_OPERATION)://C1a

               C1aBN();
               break;

            case (TokenType::RELATION)://D1a

               D1aBN();
               break;

            case (TokenType::SPACE)://A2

               state = A2;
               break;

            case (TokenType::LF)://A2a

               A2a();
               break;

            case (TokenType::SEMI_COLON)://I2a

               I2aBN();
               break;

            case (TokenType::END)://EXIT1

               EXIT1BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////B1
         case(B1):

            switch (tokenBN.token_class)
            {
            case (TokenType::LETTER)://M1

               M_1BN();
               break;

            case (TokenType::LF)://A2f

               A2fBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////C1
         case(C1):

            switch (tokenBN.token_class)
            {
            case (TokenType::SPACE)://C1

               state = C1;
               break;

            case (TokenType::LF)://A2a

               A2a();
               break;

            case (TokenType::SEMI_COLON)://I2a

               I2aBN();
               break;

            case (TokenType::END)://EXIT1

               EXIT1BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////D1
         case (D1):

            switch (tokenBN.token_class)
            {
            case (TokenType::RELATION)://C1h

               C1hBN();
               break;

            case (TokenType::SPACE)://C1g

               C1gBN();
               break;

            case (TokenType::LF)://A2e

               A2eBN();
               break;

            case (TokenType::SEMI_COLON)://I2d

               I2dBN();
               break;

            case (TokenType::END)://EXIT2

               EXIT2BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////E1
         case (E1):

            switch (tokenBN.token_class)
            {
            case (TokenType::SPACE)://F1

               state = F1;
               break;

            case (TokenType::LF)://A2f

               A2fBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////E1
         case (E2):

            switch (tokenBN.token_class)
            {
            case (TokenType::SPACE)://F2

               state = F2;
               break;

            case (TokenType::LF)://A2f

               A2fBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////E3
         case (E3):

            switch (tokenBN.token_class)
            {
            case (TokenType::SPACE)://F3

               state = F3;
               break;

            case (TokenType::LF)://A2f

               A2fBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////F1
         case(F1):

            switch (tokenBN.token_class)
            {
            case (TokenType::LETTER)://H1a

               H1aBN();
               break;

            case (TokenType::DIGIT)://G1a

               G1aBN();
               break;

            case (TokenType::SPACE)://F1

               state = F1;
               break;

            case (TokenType::LF)://A2f

               A2fBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////F2
         case(F2):

            switch (tokenBN.token_class)
            {
            case (TokenType::DIGIT)://G1a

               G1aBN();
               break;

            case (TokenType::SPACE)://F2

               state = F2;
               break;

            case (TokenType::LF)://A2f

               A2fBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////F3
         case(F3):

            switch (tokenBN.token_class)
            {
            case (TokenType::LETTER)://H1a

               H1aBN();
               break;

            case (TokenType::SPACE)://F3

               state = F3;
               break;

            case (TokenType::LF)://A2f

               A2fBN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////G1
         case(G1):

            switch (tokenBN.token_class)
            {
            case (TokenType::DIGIT)://G1b

               G1bBN();
               break;

            case (TokenType::SPACE)://C1e

               C1eBN();
               break;

            case (TokenType::LF)://A2c

               A2cBN();
               break;

            case (TokenType::SEMI_COLON)://I2b

               I2bBN();
               break;

            case (TokenType::END)://EXIT3

               EXIT3BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////H1
         case(H1):

            switch (tokenBN.token_class)
            {
            case (TokenType::LETTER)://H1b

               H1bBN();
               break;

            case (TokenType::DIGIT)://H1b

               H1bBN();
               break;

            case (TokenType::SPACE)://C1f

               C1fBN();
               break;

            case (TokenType::LF)://A2d

               A2dBN();
               break;

            case (TokenType::SEMI_COLON)://I2c

               I2cBN();
               break;

            case (TokenType::END)://EXIT4

               EXIT4BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////I1
         case(I1):

            switch (tokenBN.token_class)
            {

            case (TokenType::LETTER)://I1

               state = I1;
               break;

            case (TokenType::DIGIT)://I1

               state = I1;
               break;

            case (TokenType::ARITHMETIC_OPERATION)://I1

               state = I1;
               break;

            case (TokenType::RELATION)://I1

               state = I1;
               break;

            case (TokenType::SPACE)://I1

               state = I1;
               break;

            case (TokenType::LF)://A1a

               A1aBN();
               break;

            case(TokenType::SEMI_COLON)://I1

               state = I1;
               break;

            case (TokenType::ERROR)://I1

               state = I1;
               break;

            case(TokenType::END)://EXIT1

               EXIT5BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////I1
         case(I2):

            switch (tokenBN.token_class)
            {
            case(TokenType::LETTER)://I2

               state = I2;
               break;

            case(TokenType::DIGIT)://I2

               state = I2;
               break;

            case(TokenType::ARITHMETIC_OPERATION)://I2

               state = I2;
               break;

            case(TokenType::RELATION)://I2

               state = I2;
               break;

            case(TokenType::SPACE)://I2

               state = I2;
               break;

            case(TokenType::LF)://A2b

               A2bBN();
               break;

            case(TokenType::SEMI_COLON)://I2

               state = I2;
               break;

            case(TokenType::ERROR)://I2

               state = I2;
               break;

            case(TokenType::END)://EXIT1

               EXIT5BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////J1
         case(J1):

            switch (tokenBN.token_class)
            {
            case (TokenType::LETTER)://J1

               state = J1;
               break;

            case (TokenType::DIGIT)://J1

               state = J1;
               break;

            case (TokenType::ARITHMETIC_OPERATION)://J1

               state = J1;
               break;

            case (TokenType::RELATION)://J1

               state = J1;
               break;

            case (TokenType::SPACE)://J1

               state = J1;
               break;

            case (TokenType::LF)://A2a

               A2a();
               break;

            case (TokenType::SEMI_COLON)://J1

               state = J1;
               break;

            case (TokenType::ERROR)://J1

               state = J1;
               break;

            case (TokenType::END)://EXIT5

               EXIT5BN();
               break;

            default:
               Error_HandlerBN();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////Stop    
         case(Stop):
            stop = true;
            break;

         default:
            Error_HandlerBN();
            break;
         }
      }

      return table_tokens_BigNumber;
   }
};