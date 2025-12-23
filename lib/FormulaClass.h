#pragma once

#include <cstring>
#include "StackClass.h"
#include <cctype>
#include <sstream>

const int MaxLength = 255;
using namespace std;

template<class T>
class TFormula
{
private:
  char Formula[MaxLength];
  char PostfixForm[MaxLength];
  int getPriority(char op)
  {
    switch (op)
    {
      case '(':
        return 0;
      case '+':
      case '-':
        return 1;
      case '*':
      case '/':
        return 2;
      default:
        return -1;
    }
  }
public:
  TFormula(char form[]);
  int FormulaChecker(int Brackets[], int size);
  int FormulaConverter();
  T FormulaCalculator();
};

template<class T>
TFormula<T>::TFormula(char form[])
{
  strncpy(Formula, form, MaxLength - 1);
  Formula[MaxLength - 1] = '\0';
  PostfixForm[0] = '\0';
}

template<class T>
int TFormula<T>::FormulaChecker(int Brackets[], int size)
{
  TStack<int> stack;
  int errors = 0;
  int idx = 0;
  for (int i = 0; Formula[i]; ++i)
  {
    if (Formula[i] == '(') stack.push(i + 1);
    else if (Formula[i] == ')')
    {
      if (stack.IsEmpty())
      {
        Brackets[idx++] = 0;
        Brackets[idx++] = i + 1;
        errors++;
      } else
      {
        Brackets[idx++] = stack.pop();
        Brackets[idx++] = i + 1;
      }
    }
  }
  while (!stack.IsEmpty())
  {
    Brackets[idx++] = stack.pop();
    Brackets[idx++] = 0;
    errors++;
  }
  return errors;
}

template<class T>
int TFormula<T>::FormulaConverter()
{
  TStack<char> ops;
  int i = 0, j = 0;
  while (Formula[i])
  {
    if (isdigit(Formula[i]) || Formula[i] == '.')
    {
      while (isdigit(Formula[i]) || Formula[i] == '.')
        PostfixForm[j++] = Formula[i++];
      PostfixForm[j++] = ' ';
      continue;
    }
    if (Formula[i] == '(') ops.push(Formula[i++]);
    else if (Formula[i] == ')')
    {
      while (!ops.IsEmpty() && ops.GetTop() != '(')
        PostfixForm[j++] = ops.pop();
      if (!ops.IsEmpty() && ops.GetTop() == '(') ops.pop();
      i++;
    } else if (strchr("+-*/", Formula[i]))
    {
      while (!ops.IsEmpty() && getPriority(ops.GetTop()) >= getPriority(Formula[i]))
        PostfixForm[j++] = ops.pop();
      ops.push(Formula[i++]);
    } else i++;
  }
  while (!ops.IsEmpty()) PostfixForm[j++] = ops.pop();
  PostfixForm[j] = '\0';
  return 0;
}

template<class T>
T TFormula<T>::FormulaCalculator()
{
  TStack<T> values;
  std::istringstream iss(PostfixForm);
  std::string token;
  while (iss >> token)
  {
    if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
    {
      T num;
      istringstream(token) >> num;
      values.push(num);
    } else
    {
      T b = values.pop();
      T a = values.pop();
      switch (token[0])
      {
        case '+':
          values.push(a + b);
          break;
        case '-':
          values.push(a - b);
          break;
        case '*':
          values.push(a * b);
          break;
        case '/':
          values.push(a / b);
          break;
      }
    }
  }
  return values.pop();
}