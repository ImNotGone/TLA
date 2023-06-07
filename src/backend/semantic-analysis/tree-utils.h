#ifndef TREE_UTILS_H
#define TREE_UTILS_H

#include "abstract-syntax-tree.h"

// ===================== Free functions ========================

void freeProgram(Program * program);

void freeStatementList(StatementList * statementList);

void freeStatement(Statement * statement);

void freeAssignment(Assignment * assignment);

void freeFunctionCall(FunctionCall * functionCall);

void freeDeclaration(Declaration * declaration);

void freeWhileStatement(WhileStatement * whileStatement);

void freeForStatement(ForStatement * forStatement);

void freeRangeExpression(RangeExpression * rangeExpression);

void freeIfStatement(IfStatement * ifStatement);

void freeBlock(Block * block);

void freeExpression(Expression * expression);

void freeFactor(Factor * factor);

void freeSymbol(Symbol * symbol);

// ===================== Create functions ========================
Program * createProgram(StatementList * statements);

StatementList * createStatementList(Statement statement, StatementList * next);

Statement * createStatement(StatementType type, void * statement);

Assignment * createAssignment(Symbol * symbol, Expression * expression);

FunctionCall * createFunctionCall(FunctionCallType type, Symbol * symbol, Expression * expression, Declaration * declaration);

Declaration * createDeclaration(DeclarationType type, Symbol * symbol);

WhileStatement * createWhileStatement(Expression * condition, Block * block);

ForStatement * createForStatement(Symbol * symbol, RangeExpression * range, Block * block);

RangeExpression * createRangeExpression(Expression * expressionLeft, Expression * expressionRight);

IfStatement * createIfStatement(IfStatementType type, Expression * condition, Block * blockIf, Block * blockElse);

Block * createBlock(StatementList * statements);

Expression * createExpression(ExpressionType type, void * expression);

Factor * createFactor(FactorType type, void * factor);

Symbol * createSymbol(SymbolType type, char * name);

Constant * createConstant(int value);

#endif
