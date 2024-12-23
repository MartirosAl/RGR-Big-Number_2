#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


//Убрал флаг для JMP, JI, PUSH
enum TokenType
{
   start = -1,
   LETTER,
   DIGIT,
   ARITHMETIC_OPERATION,
   RELATIONSHIP,
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
   "RELATIONSHIP",
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

struct SymbolicToken
{
   TokenType token_class;
   int value;
   int number_line;
};

class TableToken
{
public:
   SymbolicToken* array;
   int size;
   int capacity;

   TableToken()
   {
      array = new SymbolicToken[10];
      array[0].value = -1;
      array[0].token_class = start;
      array[0].number_line = -1;
      size = 0;
      capacity = 10;
   }

   TableToken(TableToken& other_)
   {
      size = other_.size;
      capacity = other_.capacity;
      array = new SymbolicToken[other_.capacity];
      for (int i = 0; i < other_.size; i++)
      {
         array[i].value = other_.array[i].value;
         array[i].token_class = other_.array[i].token_class;
         array[i].number_line = other_.array[i].number_line;
      }
   }

   ~TableToken()
   {
      delete[] array;
      size = 0;
      capacity = 0;
   }

   TableToken operator=(TableToken other_)
   {
      if (other_.size == 0)
         throw "Uninitialized variables";

      if (other_.size >= capacity)
      {
         delete[] array;
         capacity = other_.capacity;
         array = new SymbolicToken[other_.capacity];
      }

      size = other_.size;
      for (int i = 0; i < other_.size; i++)
         array[i] = other_.array[i];

      return *this;
   }

   void Print()
   {
      for (int i = 0; i < size; i++)
      {
         cout << TokenTypeString[array[i].token_class] << " " << array[i].value << " " << array[i].number_line << endl;
      }
   }
};

struct TableConst
{
   int* array;
   int size;
   int capacity;
};

struct TableVariable
{
   string* array;
   int size;
   int capacity;
};


struct KeywordDetection
{
   int table_first_vector[5][2]
   {
      {(int)'e', 0 },
      {(int)'j', 2 },
      {(int)'p', 5 },
      {(int)'r', 10 },
      {(int)'w', 13 }
   };

   int table_detection[17][3]
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
TableConst table_constants;

//Таблица переменных
TableVariable table_variable;

//Таблица лексем для вывода
TableToken table_tokens;

//Таблицы для обнаружения ключевых слов
const KeywordDetection table_detection;





SymbolicToken Transliterator(int character)
{
   SymbolicToken result;
   result.value = 0;
   if (character >= 'A' && character <= 'Z' || character >= 'a' && character <= 'z')
   {
      result.token_class = TokenType::LETTER;
      result.value = character;
   }
   else if (character >= '0' && character <= '9')
   {
      result.token_class = TokenType::DIGIT;
      result.value = character - '0';
   }
   else if (character == '+' || character == '-' || character == '*' || character == '/' || character == '%')
   {
      result.token_class = TokenType::ARITHMETIC_OPERATION;
      result.value = character;
   }
   else if (character == '<')
   {
      result.token_class = TokenType::RELATIONSHIP;
      result.value = Less;
   }
   else if (character == '>')
   {
      result.token_class = TokenType::RELATIONSHIP;
      result.value = More;
   }
   else if (character == '=')
   {
      result.token_class = TokenType::RELATIONSHIP;
      result.value = Equal;
   }
   else if (character == '!')
   {
      result.token_class = TokenType::RELATIONSHIP;
      result.value = Not;
   }
   else if (character == ' ')
   {
      result.token_class = TokenType::SPACE;
      result.value = character;
   }
   else if (character == ';')
   {
      result.token_class = TokenType::SEMI_COLON;
      result.value = character;
   }
   else if (character == '\n')
   {
      result.token_class = TokenType::LF;
      result.value = character;
   }
   else if (character == EOF)
   {
      result.token_class = TokenType::END;
      result.value = character;
   }
   else
   {
      result.token_class = TokenType::ERROR;
      result.value = character;
   }
   return result;
}

void Expansion_TableConst()
{
   if (table_constants.array == nullptr)
   {
      table_constants.array = new int[100];
      table_constants.size = 0;
      table_constants.capacity = 100;
      return;
   }

   int* temp = new int[2 * table_constants.capacity];
   table_constants.capacity *= 2;

   for (int i = 0; i < table_constants.size; i++)
   {
      temp[i] = table_constants.array[i];
   }

   table_constants.array = temp;
}

void Expansion_TableToken()
{
   if (table_tokens.array == nullptr)
   {
      table_tokens.array = new SymbolicToken[100];
      table_tokens.size = 0;
      table_tokens.capacity = 100;
      return;
   }

   SymbolicToken* temp = new SymbolicToken[2 * table_tokens.capacity];
   table_tokens.capacity *= 2;

   for (int i = 0; i < table_tokens.size; i++)
   {
      temp[i] = table_tokens.array[i];
   }

   table_tokens.array = temp;
}

void Expansion_TableVariable()
{
   if (table_variable.array == nullptr)
   {
      table_variable.array = new string[100];
      table_variable.size = 0;
      table_variable.capacity = 100;
      return;
   }

   string* temp = new string[2 * table_variable.capacity];
   table_variable.capacity *= 2;

   for (int i = 0; i < table_variable.size; i++)
   {
      temp[i] = table_variable.array[i];
   }

   table_variable.array = temp;
}

void Push_Back_TableConst(int register_number)
{
   if (table_constants.size >= table_constants.capacity)
      Expansion_TableConst();
   table_constants.array[table_constants.size] = register_number;
   table_constants.size++;
}

void Push_Back_TableToken(SymbolicToken& token)
{
   if (table_tokens.size >= table_tokens.capacity)
      Expansion_TableToken();
   table_tokens.array[table_tokens.size] = token;
   table_tokens.size++;
}

void Push_Back_TableVariable(string register_variable)
{
   if (table_variable.size >= table_variable.capacity)
      Expansion_TableVariable();
   table_variable.array[table_tokens.size] = register_variable;
   table_variable.size++;
}

int Find_In_Array_TableConst(const int register_number)
{
   if (table_constants.array == nullptr)
      return -1;

   for (int i = 0; i < table_constants.size; i++)
   {
      if (table_constants.array[i] == register_number)
         return i;
   }
   return -1;
}

bool Find_In_Array_TableVariable(const string register_variable)
{
   if (table_variable.array == nullptr)
      return false;

   for (int i = 0; i < table_variable.size; i++)
   {
      if (table_variable.array[i] == register_variable)
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
void Add_Constant(int register_number, void*& register_indicator, int& register_value)
{

   if (table_constants.size >= table_constants.capacity)
   {
      Expansion_TableConst();
      table_constants.array[table_constants.size] = register_number;
      register_indicator = &table_constants.array[table_constants.size];
      register_value = register_number;
      return;
   }

   if (Find_In_Array_TableConst(register_number) == -1)
   {
      Push_Back_TableConst(register_number);
   }
   register_value = register_number;

   register_indicator = &table_constants.array[Find_In_Array_TableConst(register_number)];
}

//Процедура СОЗДАТЬ_ЛЕКСЕМУ
void Create_Token(TokenType& register_type_token, void*& register_indicator, int& register_value, int& register_relationship, int& number_line)
{
   SymbolicToken result;

   if (register_type_token == TokenType::RELATIONSHIP)
      result.value = register_relationship;
   else
      result.value = register_value;
   result.token_class = register_type_token;
   result.number_line = number_line;

   Push_Back_TableToken(result);

   register_value = -1;
}

//Процедура обработки ошибок
void Error_Handler(TokenType& register_type_token, void*& register_indicator, int& register_value, int& register_relationship, int& number_line, int& state)
{
   register_type_token = TokenType::ERROR;
   Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
   cerr << "An error was found in the number line " << number_line << endl;
   state = J1;
}

//Процедура ДОБАВИТЬ_ПЕРЕМЕННУЮ
void Add_Variable(string register_variable, TokenType& register_type_token, void*& register_indicator, int& register_value, int& register_relationship, int& number_line, int& state)
{
   if (Is_Keyword(register_variable))
   {
      Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
      return;
   }

   if (!Find_In_Array_TableVariable(register_variable))
   {
      Push_Back_TableVariable(register_variable);
   }

   register_indicator = &table_variable.array[table_variable.size - 1];
}





TableToken Lexical_Analyzer(const char* filename)
{
   ifstream in(filename);
   if (!in)
   {
      cout << "Не удалось открыть файл " << filename << endl;
      throw "Error";
   }

   table_constants.array = nullptr;

   //Регистр класса служит для хранения класса лексемы
   TokenType register_type_token;

   //Регистр указателя содержит указатель на таблицу имён для лексем PUSH и POP
   void* register_indicator = nullptr;

   //Регистр числа используется для вычисления констант
   int register_number;

   //Регистр отношения хранит информацию о первом символе отношения
   int register_relationship;

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

   while (!stop)
   {
      int character = in.get();
      SymbolicToken token = Transliterator(character);

      switch (state)
      {

         ////////////////////////////////////////////////////////////////////////////A1
      case (A1):
         switch (token.token_class)
         {
         case (TokenType::LETTER)://B1a

            register_detection = Find_In_Array_table_first_vector(token.value);
            if (register_detection == -1)
            {
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            }

            state = B1;
            break;

         case (TokenType::ARITHMETIC_OPERATION)://C1a

            register_type_token = TokenType::ARITHMETIC_OPERATION;
            register_value = token.value;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = C1;
            break;

         case (TokenType::RELATIONSHIP)://D1a

            register_type_token = TokenType::RELATIONSHIP;
            register_relationship = token.value;

            state = D1;
            break;

         case (TokenType::SPACE)://A1

            state = A1;
            break;

         case (TokenType::LF)://A1b

            number_line++;

            state = A1;
            break;

         case (TokenType::SEMI_COLON)://I1a

            register_type_token = TokenType::COMMENT;

            state = I1;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////A2
      case (A2):

         switch (token.token_class)
         {
         case (TokenType::LETTER)://B1a

            register_detection = Find_In_Array_table_first_vector(token.value);
            if (register_detection == -1)
            {
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            }

            state = B1;
            break;

         case (TokenType::ARITHMETIC_OPERATION)://C1a

            register_type_token = TokenType::ARITHMETIC_OPERATION;
            register_value = token.value;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = C1;
            break;

         case (TokenType::RELATIONSHIP)://D1a

            register_type_token = TokenType::RELATIONSHIP;
            register_relationship = token.value;

            state = D1;
            break;

         case (TokenType::SPACE)://A2

            state = A2;
            break;

         case (TokenType::LF)://A2c

            number_line++;

            state = A2;
            break;

         case (TokenType::SEMI_COLON)://I2a

            register_type_token = TokenType::COMMENT;

            state = I2;
            break;

         case (TokenType::END)://EXIT1

            register_type_token = TokenType::END_MARKER;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = Stop;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////B1
      case(B1):

         switch (token.token_class)
         {
         case (TokenType::LETTER)://M1

            tempforswitchinM1 = table_detection.table_detection[register_detection][1];
            if (table_detection.table_detection[register_detection][0] != token.value)
            {
               if (table_detection.table_detection[register_detection][2] != -1 && table_detection.table_detection[table_detection.table_detection[register_detection][2]][0] == token.value)
               {
                  tempforswitchinM1 = table_detection.table_detection[table_detection.table_detection[register_detection][2]][1];
                  register_detection = table_detection.table_detection[register_detection][2];
               }
               else
               {
                  Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
                  break;
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
               Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

               state = C1;
               break;

            case(table_detection.C1c)://C1c

               register_type_token = TokenType::READ;
               Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

               state = C1;
               break;

            case(table_detection.C1d)://C1d

               register_type_token = TokenType::WRITE;
               Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

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
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
               break;
            }
            break;

         case (TokenType::LF)://A2f

            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            number_line++;

            state = A2;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
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

            number_line++;

            state = A2;
            break;

         case (TokenType::SEMI_COLON)://I2a

            register_type_token = TokenType::COMMENT;

            state = I2;
            break;

         case (TokenType::END)://EXIT1

            register_type_token = TokenType::END_MARKER;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = Stop;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////D1
      case (D1):

         switch (token.token_class)
         {
         case (TokenType::RELATIONSHIP)://C1h

            if (token.value != Equal)
            {
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
               state = C1;
               break;
            }
            if (register_relationship >= Not && register_relationship <= More)
               register_relationship += 3;//Not + 3 = Not_equal, Less + 3 = Less_or_equal, More + 3 = More_or_equal
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = C1;
            break;

         case (TokenType::SPACE)://C1g

            if (register_relationship == Not)
            {
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
               state = C1;
               break;
            }
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = C1;
            break;

         case (TokenType::LF)://A2e

            if (register_relationship == Not)
            {
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
               state = A2;
               break;
            }
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            number_line++;

            state = A2;
            break;

         case (TokenType::SEMI_COLON)://I2d

            if (register_relationship == Not)
            {
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
               state = I2;
               break;
            }
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            register_type_token = TokenType::COMMENT;

            state = I2;
            break;

         case (TokenType::END)://EXIT2

            if (register_relationship == Not)
            {
               Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
               state = Stop;
               break;
            }
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            register_type_token = TokenType::END_MARKER;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = Stop;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
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

            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            number_line++;

            state = A2;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
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

            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            number_line++;

            state = A2;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
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

            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            number_line++;

            state = A2;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////F1
      case(F1):

         switch (token.token_class)
         {
         case (TokenType::LETTER)://H1a

            register_variable = token.value;

            state = H1;
            break;

         case (TokenType::DIGIT)://G1a

            register_number = token.value;

            state = G1;
            break;

         case (TokenType::SPACE)://F1

            state = F1;
            break;

         case (TokenType::LF)://A2f

            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            number_line++;

            state = A2;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////F2
      case(F2):

         switch (token.token_class)
         {
         case (TokenType::DIGIT)://G1a

            register_number = token.value;

            state = G1;
            break;

         case (TokenType::SPACE)://F2

            state = F2;
            break;

         case (TokenType::LF)://A2f

            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            number_line++;

            state = A2;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////F3
      case(F3):

         switch (token.token_class)
         {
         case (TokenType::LETTER)://H1a

            register_variable = token.value;

            state = H1;
            break;

         case (TokenType::SPACE)://F3

            state = F3;
            break;

         case (TokenType::LF)://A2f

            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            number_line++;

            state = A2;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////G1
      case(G1):

         switch (token.token_class)
         {
         case (TokenType::DIGIT)://G1b

            register_number *= 10;
            register_number += token.value;

            state = G1;
            break;

         case (TokenType::SPACE)://C1e

            Add_Constant(register_number, register_indicator, register_value);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = C1;
            break;

         case (TokenType::LF)://A2c

            Add_Constant(register_number, register_indicator, register_value);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            number_line++;

            state = A2;
            break;

         case (TokenType::SEMI_COLON)://I2b

            Add_Constant(register_number, register_indicator, register_value);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            register_type_token = TokenType::COMMENT;

            state = I2;
            break;

         case (TokenType::END)://EXIT3

            Add_Constant(register_number, register_indicator, register_value);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            register_type_token = TokenType::END_MARKER;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = Stop;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////H1
      case(H1):

         switch (token.token_class)
         {
         case (TokenType::LETTER)://H1b

            register_variable.push_back(token.value);

            state = H1;
            break;

         case (TokenType::DIGIT)://H1b

            register_variable.push_back(token.value);

            state = H1;
            break;

         case (TokenType::SPACE)://C1f

            Add_Variable(register_variable, register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = C1;
            break;

         case (TokenType::LF)://A2d

            Add_Variable(register_variable, register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            number_line++;

            state = A2;
            break;

         case (TokenType::SEMI_COLON)://I2c

            Add_Variable(register_variable, register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            register_type_token = TokenType::COMMENT;

            state = I2;
            break;

         case (TokenType::END)://EXIT4

            Add_Variable(register_variable, register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            register_type_token = TokenType::END_MARKER;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = Stop;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
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

         case (TokenType::RELATIONSHIP)://I1

            state = I1;
            break;

         case (TokenType::SPACE)://I1

            state = I1;
            break;

         case (TokenType::LF)://A1a

            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            number_line++;

            state = A1;
            break;

         case(TokenType::SEMI_COLON)://I1

            state = I1;
            break;

         case (TokenType::ERROR)://I1

            state = I1;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
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

         case(TokenType::RELATIONSHIP)://I2

            state = I2;
            break;

         case(TokenType::SPACE)://I2

            state = I2;
            break;

         case(TokenType::LF)://A2b

            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);
            number_line++;

            state = A2;
            break;

         case(TokenType::SEMI_COLON)://I2

            state = I2;
            break;

         case(TokenType::ERROR)://I2

            state = I2;
            break;

         case(TokenType::END)://EXIT1

            register_type_token = TokenType::END_MARKER;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = Stop;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
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

         case (TokenType::RELATIONSHIP)://J1

            state = J1;
            break;

         case (TokenType::SPACE)://J1

            state = J1;
            break;

         case (TokenType::LF)://A2a

            number_line++;

            state = A2;
            break;

         case (TokenType::SEMI_COLON)://J1

            state = J1;
            break;

         case (TokenType::ERROR)://J1

            state = J1;
            break;

         case (TokenType::END)://EXIT1

            register_type_token = TokenType::END_MARKER;
            Create_Token(register_type_token, register_indicator, register_value, register_relationship, number_line);

            state = Stop;
            break;

         default:
            state = Stop;
            Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
            break;
         }
         break;

         ////////////////////////////////////////////////////////////////////////////Stop    
      case(Stop):
         stop = true;
         break;

      default:
         state = Stop;
         Error_Handler(register_type_token, register_indicator, register_value, register_relationship, number_line, state);
         break;
      }

   }

   return table_tokens;
};