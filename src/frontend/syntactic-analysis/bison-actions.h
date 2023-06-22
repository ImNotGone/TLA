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
Program * ProgramGrammarAction(StatementList statements);

// Block
Block * BlockGrammarAction(StatementList statements);

//Statements
StatementList StatementListGrammarAction(Statement * statement, StatementList next);

// Statement
// recibe void puntero asi puedo obtener cualquier tipo de statement
// recibo el type para distinguir
Statement * StatementGrammarAction(void * statement, StatementType type);

IfStatement * IfStatementGrammarAction(Expression * cond, Block * if_block, Block * else_block);
ForStatement * ForStatementGrammarAction(char * varname, RangeExpression * range, Block * block);
WhileStatement * WhileStatementGrammarAction(Expression * expression, Block * block);

// Expresión.
Expression * ExpressionGrammarAction(Expression * left, Expression * right, Factor * factor, ExpressionType type);
RangeExpression * RangeExpressionGrammarAction(Expression * exp1, Expression * exp2);

// Factores.
Factor * FactorGrammarAction(Expression * exp, Constant * con, char * varname, FactorType type);
Constant * ConstantGrammarAction(int value);

// Funciones
FunctionCall * FunctionGrammarAction(char * varname, Expression * exp, FunctionCallType type);
FunctionCall * FunctionDeclarationGrammarAction(char *varname, Declaration *declaration, FunctionCallType callType);

// Constantes.
int IntegerConstantGrammarAction(int value);

// Declaraciones
Declaration * DeclarationGrammarAction(char * varname, DeclarationType type);
Declaration * IntDeclarationAndAssignmentGrammarAction(char * varname, Expression * exp, FunctionCall * functionCall);

Assignment * AssignmentGrammarAction(char * var, Expression * exp, FunctionCall * functionCall);

#endif
