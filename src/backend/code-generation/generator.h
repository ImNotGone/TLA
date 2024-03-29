#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"
#include <stdio.h>

void Output(char *format, ...);

void SetOutputFile(FILE *file);

void GeneratorProgram(Program *program);

void GeneratorStatementList(StatementList statementList);

void GeneratorStatement(Statement *statement);

void GeneratorAssignment(Assignment *assignment);

void GeneratorFunctionCall(FunctionCall *functionCall);

void GeneratorDeclaration(Declaration *declaration);

void GeneratorWhileStatement(WhileStatement *whileStatement);

void GeneratorForStatement(ForStatement *forStatement);

void GeneratorRangeExpression(RangeExpression *rangeExpression);

void GeneratorIfStatement(IfStatement *ifStatement);

void GeneratorBlock(Block *block);

void GeneratorExpression(Expression *expression);

void GeneratorFactor(Factor *factor);

void GeneratorConstant(Constant *constant);

#endif
