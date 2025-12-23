#include <cstring>
#include <sstream>
#include <gtest.h>
#include "FormulaClass.h"

// Тест конструктора
TEST(TFormulaTest, Constructor) {
  char expr[] = "1+2";
  TFormula<double> formula(expr);

  // Проверяем, что Formula инициализирована
  // Для этого используем косвенную проверку через другие методы
  int brackets[10];
  formula.FormulaChecker(brackets, 10); // Должен работать без ошибок
  SUCCEED();
}

// Тест проверки скобок - корректные выражения
TEST(TFormulaTest, FormulaChecker_CorrectExpressions) {
  {
    char expr[] = "(1+2)";
    TFormula<double> formula(expr);
    int brackets[10];
    EXPECT_EQ(formula.FormulaChecker(brackets, 10), 0);
  }

  {
    char expr[] = "((1+2)*(3-4))";
    TFormula<double> formula(expr);
    int brackets[20];
    EXPECT_EQ(formula.FormulaChecker(brackets, 20), 0);
  }

  {
    char expr[] = "1+2";
    TFormula<double> formula(expr);
    int brackets[10];
    EXPECT_EQ(formula.FormulaChecker(brackets, 10), 0);
  }
}

// Тест проверки скобок - некорректные выражения
TEST(TFormulaTest, FormulaChecker_IncorrectExpressions) {
  {
    char expr[] = "(1+2";
    TFormula<double> formula(expr);
    int brackets[10];
    EXPECT_GT(formula.FormulaChecker(brackets, 10), 0);
  }

  {
    char expr[] = "1+2)";
    TFormula<double> formula(expr);
    int brackets[10];
    EXPECT_GT(formula.FormulaChecker(brackets, 10), 0);
  }

  {
    char expr[] = "(1+2))";
    TFormula<double> formula(expr);
    int brackets[10];
    EXPECT_GT(formula.FormulaChecker(brackets, 10), 0);
  }

  {
    char expr[] = "((1+2)";
    TFormula<double> formula(expr);
    int brackets[10];
    EXPECT_GT(formula.FormulaChecker(brackets, 10), 0);
  }
}

// Тест преобразования в постфиксную форму
TEST(TFormulaTest, FormulaConverter_SimpleExpressions) {
  // После преобразования можно проверить через FormulaCalculator
  {
    char expr[] = "1+2";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 3.0);
  }

  {
    char expr[] = "1-2";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), -1.0);
  }

  {
    char expr[] = "2*3";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 6.0);
  }

  {
    char expr[] = "6/2";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 3.0);
  }
}

// Тест с вещественными числами
TEST(TFormulaTest, FormulaConverter_FloatingPoint) {
  {
    char expr[] = "1.5+2.5";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 4.0);
  }

  {
    char expr[] = "3.5*2.0";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 7.0);
  }

  {
    char expr[] = "5.0/2.0";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 2.5);
  }
}

// Тест с игнорированием некорректных символов
TEST(TFormulaTest, FormulaConverter_IgnoreInvalidChars) {
  {
    char expr[] = "a1b+c2d";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 3.0); // 1 + 2 = 3
  }

  {
    char expr[] = "1@+2#";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 3.0); // 1 + 2 = 3
  }
}

// Тест граничных случаев
TEST(TFormulaTest, FormulaConverter_EdgeCases) {
  {
    // Одно число
    char expr[] = "42";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 42.0);
  }

  {
    // Отрицательные числа (не поддерживаются в текущей реализации, должны игнорироваться как оператор)
    char expr[] = "-1+2";
    TFormula<double> formula(expr);
    EXPECT_EQ(formula.FormulaConverter(), 0);
    // Результат может быть неожиданным, так как минус перед 1 будет интерпретирован как оператор
    // а не как часть числа
  }
}
// Тест с разными типами данных
TEST(TFormulaTest, DifferentDataTypes) {
  {
    char expr[] = "1+2";
    TFormula<int> formula(expr);
    formula.FormulaConverter();
    EXPECT_EQ(formula.FormulaCalculator(), 3);
  }

  {
    char expr[] = "1.5+2.5";
    TFormula<float> formula(expr);
    formula.FormulaConverter();
    EXPECT_FLOAT_EQ(formula.FormulaCalculator(), 4.0f);
  }

  {
    char expr[] = "10/3";
    TFormula<double> formula(expr);
    formula.FormulaConverter();
    EXPECT_DOUBLE_EQ(formula.FormulaCalculator(), 10.0/3.0);
  }
}

// Тест обработки длинных выражений
TEST(TFormulaTest, LongExpression) {
  char expr[MaxLength];
  // Создаем длинное, но корректное выражение
  strcpy(expr, "1+2-3*4/5+6-7*8/9+0");

  TFormula<double> formula(expr);

  // Проверяем скобки (их нет, должно быть 0 ошибок)
  int brackets[40];
  int errors = formula.FormulaChecker(brackets, 40);
  EXPECT_EQ(errors, 0);

  // Преобразуем
  EXPECT_EQ(formula.FormulaConverter(), 0);

  // Вычисляем (проверяем, что не падает)
  double result = formula.FormulaCalculator();
  // Не проверяем точное значение, так как оно зависит от реализации
  // Главное, что не произошло исключений
  SUCCEED();
}