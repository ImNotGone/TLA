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
Program * ProgramGrammarAction(const StatementList statements);

// Block
Block * BlockGrammarAction(const StatementList statements);

//Statements
StatementList StatementListGrammarAction(const Statement * statement, const StatementList next);

// Statement
// recibe void puntero asi puedo obtener cualquier tipo de statement
// recibo el type para distinguir
Statement * StatementGrammarAction(const void * statement, const StatementType type);

IfStatement * IfStatementGrammarAction(const Expression * cond, const Block * if_block, const Block * else_block);
ForStatement * ForStatementGrammarAction(const char * varname, const RangeExpression * range, const Block * block);
WhileStatement * WhileStatementGrammarAction(const Expression * expression, const Block * block);

// Expresión.
Expression * ExpressionGrammarAction(const Expression * left, const Expression * right, const Factor * factor, const ExpressionType type);
RangeExpression * RangeExpressionGrammarAction(const Expression * exp1, const Expression * exp2);

// Factores.
Factor * FactorGrammarAction(const Expression * exp, const Constant * con, const char * varname, const FactorType type);
Constant * ConstantGrammarAction(const int value);

//Funciones
FunctionCall * FunctionGrammarAction(const char * varname, const Expression * exp, const FunctionCallType type);

// Constantes.
int IntegerConstantGrammarAction(const int value);

//Declaraciones
Declaration * DeclarationGrammarAction(const char * varname, const DeclarationType type);
Declaration * IntDeclarationAndAssignmentGrammarAction(const char * varname, const Expression * exp);

Assignment * AssignmentGrammarAction(const char * var, const Expression * exp);

int TreeTypeGrammarAction(const int value);

#endif
