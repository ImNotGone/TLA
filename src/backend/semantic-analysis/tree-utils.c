#include <stdlib.h>
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

void freeStatementList(StatementList *statementList) {
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
    freeSymbol(assignment->symbol);
    free(assignment);
}

void freeFunctionCall(FunctionCall *functionCall) {
    if (functionCall == NULL) {
        return;
    }

    freeExpression(functionCall->expression);
    freeSymbol(functionCall->symbol);
    freeDeclaration(functionCall->declaration);
    free(functionCall);
}

void freeDeclaration(Declaration *declaration) {
    if (declaration == NULL) {
        return;
    }

    freeSymbol(declaration->symbol);
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

    if (expression->type == FACTOR) {
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
        case SYMBOL_FACTOR:
            freeSymbol(factor->symbol);
            break;
        default:
            break;
    }

    free(factor);
}

void freeSymbol(Symbol *symbol) {
    if (symbol == NULL) {
        return;
    }

    free(symbol->name);
    free(symbol);
}
