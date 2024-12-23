#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <map>
using namespace std;



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

vector<string> TokenTypeString =
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
   Less,
   More,
   Not_equal,
   Less_or_equal,
   More_or_equal
};

vector<string> RelationString
{
   "Equal",
   "Not",
   "Less",
   "More",
   "Not_equal",
   "Less_or_equal",
   "More_or_equal"
};

struct SymbolicToken
{
   TokenType token_class;
   variant<int, int*, string> value;
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

//Таблица констант
vector<int> table_constants;

//Таблица переменных
vector<string> table_variable;

//Таблица лексем для вывода
vector<SymbolicToken> table_tokens;

//Таблицы для обнаружения ключевых слов
const KeywordDetection table_detection;

class TableToken
{
public:

   void Print()
   {
      for (int i = 0; i < table_tokens.size(); i++)
      {
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
            cout << get<2>(table_tokens[i].value) << " ";
         else if (table_tokens[i].value.index() == 1)
            cout << get<1>(table_tokens[i].value) << " ";//звезда
         else if (table_tokens[i].value.index() == 0)
            cout << get<0>(table_tokens[i].value) << " ";
         cout << table_tokens[i].number_line << endl;
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
         result.value = Less;
      }
      else if (character == '>')
      {
         result.token_class = TokenType::RELATION;
         result.value = More;
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

   int Find_In_Array_TableConst(const int register_number)
   {
      if (table_constants.empty())
         return -1;

      for (int i = 0; i < table_constants.size(); i++)
      {
         if (table_constants[i] == register_number)
            return i;
      }
      return -1;
   }

   bool Find_In_Array_TableVariable(const string register_variable)
   {
      if (table_variable.empty())
         return false;

      for (int i = 0; i < table_variable.size(); i++)
      {
         if (table_variable[i] == register_variable)
            return true;
      }
      return false;
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

   //Процедура ДОБАВИТЬ_КОНСТАНТУ
   void Add_Constant()
   {

      if (Find_In_Array_TableConst(register_number) == -1)
      {
         table_constants.push_back(register_number);
      }
      register_value = register_number;

      register_indicator = &(table_constants[Find_In_Array_TableConst(register_number)]);
   }

   //Процедура СОЗДАТЬ_ЛЕКСЕМУ
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

   //Процедура обработки ошибок
   void Error_Handler()
   {
      register_type_token = TokenType::ERROR;
      Create_Token();
      cerr << "An error was found in the number line " << number_line << endl;
      state = J1;
   }

   //Процедура ДОБАВИТЬ_ПЕРЕМЕННУЮ
   void Add_Variable()
   {
      if (Is_Keyword(register_variable))
      {
         Error_Handler();
         return;
      }

      if (!Find_In_Array_TableVariable(register_variable))
      {
         table_variable.push_back(register_variable);
      }

      register_indicator = register_variable;
   }


   //Регистр класса служит для хранения класса лексемы
   TokenType register_type_token;

   //Регистр указателя содержит указатель на таблицу имён для лексем PUSH и POP
   variant<int*, string> register_indicator;

   //Регистр числа используется для вычисления констант
   int register_number;

   //Регистр отношения хранит информацию о первом символе отношения
   int register_relation;

   //Регистр переменной накапливает имя переменной
   string register_variable;

   //Регистр обнаружения хранит номер позиции в таблице обнаружения для поиска ключевых слов
   int register_detection;

   //Регистр значения хранит значения лексем
   int register_value = -1;

   //Номер строки хранит номер текущей строки в программе
   int number_line = 1;

   //Флаг остановки
   bool stop = false;

   //Переменная для M1
   int tempforswitchinM1;

   int state = A1;

   SymbolicToken token;

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
      if (register_relation == Not)
      {
         Error_Handler();
         state = A2;
         return;
      }
      Create_Token();
      number_line++;

      state = A2;
   }

   void A2f()
   {
      Error_Handler();
      number_line++;

      state = A2;
   }

   void B1a()
   {
      register_detection = Find_In_Array_table_first_vector(get<0>(token.value));
      if (register_detection == -1)
      {
         Error_Handler();
      }

      state = B1;
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
      if (register_relation == Not)
      {
         Error_Handler();
         state = C1;
         return;
      }
      Create_Token();

      state = C1;
   }

   void C1h()
   {
      if (get<0>(token.value) != Equal)
      {
         Error_Handler();
         state = C1;
         return;
      }
      if (register_relation >= Not && register_relation <= More)
         register_relation += 3;//Not + 3 = Not_equal, Less + 3 = Less_or_equal, More + 3 = More_or_equal
      Create_Token();

      state = C1;
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
      if (register_relation == Not)
      {
         Error_Handler();
         state = I2;
         return;
      }
      Create_Token();
      register_type_token = TokenType::COMMENT;

      state = I2;
   }

   void EXIT1()
   {
      register_type_token = TokenType::END_MARKER;
      Create_Token();

      state = Stop;
   }

   void EXIT2()
   {
      if (register_relation == Not)
      {
         Error_Handler();
         state = Stop;
         return;
      }
      Create_Token();
      register_type_token = TokenType::END_MARKER;
      Create_Token();

      state = Stop;
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

   void M1()
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
         state = Stop;
         Error_Handler();
         break;
      }
   }



   vector<SymbolicToken> Lexical_Analyzer(const char* filename)
   {
      ifstream in(filename);
      if (!in)
      {
         cout << "Не удалось открыть файл " << filename << endl;
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
               state = Stop;
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
               state = Stop;
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////B1
         case(B1):

            switch (token.token_class)
            {
            case (TokenType::LETTER)://M1

               M1();
               break;

            case (TokenType::LF)://A2f

               A2f();
               break;

            default:
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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
               state = Stop;
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

            default:
               state = Stop;
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

               EXIT1();
               break;

            default:
               state = Stop;
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

            case (TokenType::END)://EXIT1

               EXIT1();
               break;

            default:
               state = Stop;
               Error_Handler();
               break;
            }
            break;

            ////////////////////////////////////////////////////////////////////////////Stop    
         case(Stop):
            stop = true;
            break;

         default:
            state = Stop;
            Error_Handler();
            break;
         }
      }


      return table_tokens;
   }
}; 