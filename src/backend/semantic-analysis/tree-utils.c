#include <stdlib.h>
#include <assert.h>
#include "tree-utils.h"
#include "abstract-syntax-tree.h"

// =========================== Free functions ===========================

void freeProgram(Program *program) {
    if (program == NULL) {
        return;
    }

    freeStatementList(program->statements);
    free(program);
}

void freeStatementList(StatementList statementList) {
    if (statementList == NULL) {
        return;
    }

    freeStatementList(statementList->next);

    freeStatement(statementList->statement);
    free(statementList);
}

void freeStatement(Statement *statement) {
    if (statement == NULL) {
        return;
    }

    switch (statement->type) {
        case IF_STATEMENT:
            freeIfStatement(statement->ifStatement);
            break;
        case FOR_STATEMENT:
            freeForStatement(statement->forStatement);
            break;
        case WHILE_STATEMENT:
            freeWhileStatement(statement->whileStatement);
            break;
        case FUNCTION_CALL_STATEMENT:
            freeFunctionCall(statement->functionCall);
            break;
        case DECLARATION_STATEMENT:
            freeDeclaration(statement->declaration);
            break;
        case ASSIGNMENT_STATEMENT:
            freeAssignment(statement->assignment);
            break;
        default:
            break;
    }

    free(statement);
}

void freeAssignment(Assignment *assignment) {
    if (assignment == NULL) {
        return;
    }

    freeExpression(assignment->expression);
    freeVariable(assignment->varname);
    freeFunctionCall(assignment->functionCall);
    free(assignment);
}

void freeFunctionCall(FunctionCall *functionCall) {
    if (functionCall == NULL) {
        return;
    }

    freeExpression(functionCall->expression);
    freeVariable(functionCall->varname);
    freeDeclaration(functionCall->declaration);
    free(functionCall);
}

void freeDeclaration(Declaration *declaration) {
    if (declaration == NULL) {
        return;
    }

    freeVariable(declaration->varname);
    free(declaration);
}

void freeWhileStatement(WhileStatement *whileStatement) {
    if (whileStatement == NULL) {
        return;
    }

    freeExpression(whileStatement->condition);
    freeBlock(whileStatement->block);
    free(whileStatement);
}

void freeForStatement(ForStatement *forStatement) {
    if (forStatement == NULL) {
        return;
    }

    freeRangeExpression(forStatement->range);
    freeBlock(forStatement->block);
    free(forStatement);
}

void freeRangeExpression(RangeExpression *rangeExpression) {
    if (rangeExpression == NULL) {
        return;
    }

    freeExpression(rangeExpression->expressionLeft);
    freeExpression(rangeExpression->expressionRight);
    free(rangeExpression);
}

void freeIfStatement(IfStatement *ifStatement) {
    if (ifStatement == NULL) {
        return;
    }

    freeExpression(ifStatement->condition);
    freeBlock(ifStatement->blockIf);

    if (ifStatement->type == IF_ELSE_TYPE) {
        freeBlock(ifStatement->blockElse);
    }
    free(ifStatement);
}

void freeBlock(Block *block) {
    if (block == NULL) {
        return;
    }

    freeStatementList(block->statements);
    free(block);
}

void freeExpression(Expression *expression) {
    if (expression == NULL) {
        return;
    }

    if (expression->type == FACTOR_EXPRESSION) {
        freeFactor(expression->factor);
    } else {
        freeExpression(expression->leftExpression);
        freeExpression(expression->rightExpression);
    }

    free(expression);
}

void freeFactor(Factor *factor) {
    if (factor == NULL) {
        return;
    }

    switch (factor->type) {
        case EXPRESSION_FACTOR:
            freeExpression(factor->expression);
            break;
        case VARIABLE_FACTOR:
            freeVariable(factor->varname);
            break;
        default:
            break;
    }

    free(factor);
}

void freeVariable(char *varname) {
    if (varname == NULL) {
        return;
    }
    free(varname);
}

Program * createProgram(StatementList statements) {
    Program * new = malloc(sizeof(Program));

    new->statements = statements;

    return new;
}

StatementList createStatementList(Statement * statement, StatementList next) {
    StatementList new = malloc(sizeof(struct StatementNode));

    new->statement = statement;
    new->next = next;

    return new;
}

Statement * createStatement(StatementType type, void * statement) {
    Statement * new = calloc(1, sizeof(Statement));

    new->type = type;
    switch (type) {
        case IF_STATEMENT: new->ifStatement = statement; break;
        case FOR_STATEMENT: new->forStatement = statement; break;
        case WHILE_STATEMENT: new->whileStatement = statement; break;
        case ASSIGNMENT_STATEMENT: new->assignment = statement; break;
        case DECLARATION_STATEMENT: new->declaration = statement; break;
        case FUNCTION_CALL_STATEMENT: new->functionCall = statement; break;
        default: assert(0 && "Illegal State"); break;
    }

    return new;
}

Assignment * createAssignment(char * varname, Expression * expression, FunctionCall * functionCall) {
    Assignment * new = malloc(sizeof(Assignment));

    new->varname = varname;
    new->expression = expression;
    new->functionCall = functionCall;

    return new;
}

FunctionCall * createFunctionCall(FunctionCallType type, char * varname, Expression * expression, Declaration * declaration) {
    FunctionCall * new = malloc(sizeof(FunctionCall));

    new->type = type;
    new->varname = varname;
    new->expression = expression;
    new->declaration = declaration;

    return new;
}

Declaration * createDeclaration(VarType type, char * varname, Assignment * assignment) {
    Declaration * new = malloc(sizeof(Declaration));



    new->type = type;
    new->varname = varname;
    new->assignment = assignment;

    return new;
}

WhileStatement * createWhileStatement(Expression * condition, Block * block) {
    WhileStatement * new = malloc(sizeof(WhileStatement));

    new->condition = condition;
    new->block = block;

    return new;
}

ForStatement * createForStatement(char * varname, RangeExpression * range, Block * block) {
    ForStatement * new = malloc(sizeof(ForStatement));

    new->varname = varname;
    new->block = block;
    new->range = range;

    return new;
}

RangeExpression * createRangeExpression(Expression * left, Expression * right) {
    RangeExpression * new = malloc(sizeof(RangeExpression));

    new->expressionLeft = left;
    new->expressionRight = right;

    return new;
}

IfStatement * createIfStatement(IfStatementType type, Expression * condition, Block * blockIf, Block * blockElse) {
    IfStatement * new = malloc(sizeof(IfStatement));

    new->type = type;
    new->condition = condition;
    new->blockIf = blockIf;
    new->blockElse = blockElse;

    return new;
}

Block * createBlock(StatementList statements) {
    Block * new = malloc(sizeof(Block));

    new->statements = statements;

    return new;
}

Expression * createExpression(ExpressionType type, Expression * left, Expression * right, Factor * factor) {
    Expression * new = malloc(sizeof(Expression));

    new->type = type;
    new->leftExpression = left;
    new->rightExpression = right;
    new->factor = factor;

    return new;
}

Factor * createFactor(FactorType type, Expression * exp, Constant * con, char * var) {
    Factor * new = malloc(sizeof(Factor));

    new->type = type;
    new->expression = exp;
    new->constant = con;
    new->varname = var;

    return new;
}

Constant * createConstant(int value) {
    Constant * new = malloc(sizeof(Constant));

    new->value = value;

    return new;
}

