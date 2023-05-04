#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
int ProgramGrammarAction(const int value);

//Statements
int StatementListGrammarAction(const int leftValue, const int rightValue);
int StatementGrammarAction(const int value);
int IfStatementGrammarAction(const int exp, const int block1, const int block2);
int ForStatementGrammarAction(const int symbol, const int range_expression, const int block);
int WhileStatementGrammarAction(const int expression, const int block);

// Expresión.
int AdditionExpressionGrammarAction(const int leftValue, const int rightValue);
int SubstractionExpressionGrammarAction(const int leftValue, const int rightValue);
int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue);
int DivisionExpressionGrammarAction(const int leftValue, const int rightValue);
int FactorExpressionGrammarAction(const int value);
int RangeExpressionGrammarAction(const int exp1, const int exp2);
int ModulusExpressionGrammarAction(const int leftValue, const int rightValue);
int AndExpressionGrammarAction(const int leftValue, const int rightValue);
int OrExpressionGrammarAction(const int leftValue, const int rightValue);
int NotExpressionGrammarAction(const int value);
int GreaterThanExpressionGrammarAction(const int leftValue, const int rightValue);
int GreaterThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue);
int LessThanExpressionGrammarAction(const int leftValue, const int rightValue);
int LessThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue);
int EqualityExpressionGrammarAction(const int leftValue, const int rightValue);
int InequalityExpressionGrammarAction(const int leftValue, const int rightValue);

// Factores.
int ExpressionFactorGrammarAction(const int value);
int ConstantFactorGrammarAction(const int value);
int ParenthesisFactorGrammarAction(const int value);
int VariableFactorGrammarAction(const int value);
int ConstantGrammarAction(const int value);

//Funciones
int OneParamFunctionGrammarAction(const int symbol);
int TwoParamFunctionGrammarAction(const int symbol1, const int symbol2);

// Constantes.
int IntegerConstantGrammarAction(const int value);

//Declaraciones
int TreeDeclarationGrammarAction(const int symbol);
int IntDeclarationGrammarAction(const int symbol);
int IntDeclarationAndAssignmentGrammarAction(const int symbol, const int expression);

int TreeTypeGrammarAction(const int value);

#endif
