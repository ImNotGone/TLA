#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

// Output language is Java

void GeneratorProgram(Program *program) {

    Output("import java.io.IOException;\n");
    Output("public class Main {\n");
    Output("public static void main(String[] args) throws IOException {\n");
    GeneratorStatementList(program->statements);
    Output("}\n");
    Output("}\n");
}

void GeneratorStatementList(StatementList statementList) {
    StatementList current = statementList;
    while (current != NULL) {
        GeneratorStatement(current->statement);
        current = current->next;
    }
}

void GeneratorStatement(Statement *statement) {
    switch (statement->type) {
        case IF_STATEMENT:
            GeneratorIfStatement(statement->ifStatement);
            break;
        case FOR_STATEMENT:
            GeneratorForStatement(statement->forStatement);
            break;
        case WHILE_STATEMENT:
            GeneratorWhileStatement(statement->whileStatement);
            break;
        case FUNCTION_CALL_STATEMENT:
            GeneratorFunctionCall(statement->functionCall);
            break;
        case DECLARATION_STATEMENT:
            GeneratorDeclaration(statement->declaration);
            break;
        case ASSIGNMENT_STATEMENT:
            GeneratorAssignment(statement->assignment);
            break;
    }
}

void GeneratorAssignment(Assignment *assignment) {
    Output("%s = ", assignment->varname);
    if (assignment->expression != NULL) {
        GeneratorExpression(assignment->expression);
    } 

    if (assignment->functionCall != NULL) {
        GeneratorFunctionCall(assignment->functionCall);
    }
}

void GeneratorFunctionCall(FunctionCall *functionCall) {

    if (functionCall->type == ADD_TREE_CALL) {
        GeneratorDeclaration(functionCall->declaration);
        Output("%s.add(%s);\n", functionCall->declaration->varname, functionCall->varname);
        return;
    }

    Output("%s.", functionCall->varname);

    switch (functionCall->type) {
        case PRINT_CALL:
            Output("draw()");
            break;
        case MAX_CALL:
            Output("max()");
            break;
        case MIN_CALL:
            Output("min()");
            break;
        case ROOT_CALL:
            Output("root()");
            break;
        case HEIGHT_CALL:
            Output("height()");
            break;
        case INSERT_CALL:
            Output("insert(");
            GeneratorExpression(functionCall->expression);
            Output(")");
            break;
        case REMOVE_CALL:
            Output("remove(");
            GeneratorExpression(functionCall->expression);
            Output(")");
            break;
        case INORDER_CALL:
            Output("inorder()");
            break;
        case POSTORDER_CALL:
            Output("postorder()");
            break;
        case PREORDER_CALL:
            Output("preorder()");
            break;
        case REDUCE_CALL:
            Output("reduce(");
            GeneratorExpression(functionCall->expression);
            Output(")");
            break;
        case FIND_CALL:
            Output("find(");
            GeneratorExpression(functionCall->expression);
            Output(")");
            break;
        case PRESENT_CALL:
            Output("present(");
            GeneratorExpression(functionCall->expression);
            Output(")");
            break;
        default:
            break;
    }

    Output(";\n");
}

void GeneratorDeclaration(Declaration *declaration) {
    switch (declaration->type) {
        case RBT_DECLARATION:
            Output("Tree %s = new RBT();\n", declaration->varname);
            break;
        case AVL_DECLARATION:
            Output("Tree %s = new AVL();\n", declaration->varname);
            break;
        case BST_DECLARATION:
            Output("Tree %s = new BST();\n", declaration->varname);
            break;
        case INT_DECLARATION:
            Output("int ");
            if (declaration->assignment != NULL) {
                GeneratorAssignment(declaration->assignment);
            } else {
                Output("%s;\n", declaration->varname);
            }
            break;
        default:
            break;
    }
}

void GeneratorWhileStatement(WhileStatement *whileStatement) {
    Output("while (");
    GeneratorExpression(whileStatement->condition);
    Output(")");
    GeneratorBlock(whileStatement->block);
}

void GeneratorForStatement(ForStatement *forStatement) {
    Output("for (int %s = ", forStatement->varname);
    GeneratorExpression(forStatement->range->expressionLeft);
    Output("; %s < ", forStatement->varname);
    GeneratorExpression(forStatement->range->expressionRight);
    Output("; %s++)", forStatement->varname);
    GeneratorBlock(forStatement->block);

}

void GeneratorIfStatement(IfStatement *ifStatement) {
    Output("if (");
    GeneratorExpression(ifStatement->condition);
    Output(")");
    GeneratorBlock(ifStatement->blockIf);

    if (ifStatement->type == IF_ELSE_TYPE) {
        Output("else");
        GeneratorBlock(ifStatement->blockElse);
    }
}

void GeneratorBlock(Block *block) {
    Output("{\n");
    GeneratorStatementList(block->statements);
    Output("}\n");
}

void GeneratorExpression(Expression *expression) {
    Output("(");
    switch (expression->type) {
        case ADDITION_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" + ");
            GeneratorExpression(expression->rightExpression);
            break;
        case SUBTRACTION_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" - ");
            GeneratorExpression(expression->rightExpression);
            break;
        case MULTIPLICATION_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" * ");
            GeneratorExpression(expression->rightExpression);
            break;
        case DIVISION_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" / ");
            GeneratorExpression(expression->rightExpression);
            break;
        case MODULUS_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" % ");
            GeneratorExpression(expression->rightExpression);
            break;
        case AND_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" && ");
            GeneratorExpression(expression->rightExpression);
            break;
        case NOT_EXPRESSION:
            Output("!");
            GeneratorExpression(expression->leftExpression);
            break;
        case OR_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" || ");
            GeneratorExpression(expression->rightExpression);
            break;
        case EQUALS_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" == ");
            GeneratorExpression(expression->rightExpression);
            break;
        case NOT_EQUALS_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" != ");
            GeneratorExpression(expression->rightExpression);
            break;
        case LESS_THAN_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" < ");
            GeneratorExpression(expression->rightExpression);
            break;
        case LEES_EQUAL_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" <= ");
            GeneratorExpression(expression->rightExpression);
            break;
        case GREATER_THAN_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" > ");
            GeneratorExpression(expression->rightExpression);
            break;
        case GREATER_EQUAL_EXPRESSION:
            GeneratorExpression(expression->leftExpression);
            Output(" >= ");
            GeneratorExpression(expression->rightExpression);
            break;
        case FACTOR_EXPRESSION:
            GeneratorFactor(expression->factor);
            break;
        default:
            break;
    }
    Output(")");
}

void GeneratorFactor(Factor *factor) {
    switch (factor->type) {
        case CONSTANT_FACTOR:
            GeneratorConstant(factor->constant);
            break;
        case VARIABLE_FACTOR:
            Output("%s", factor->varname);
            break;
        case EXPRESSION_FACTOR:
            Output("(");
            GeneratorExpression(factor->expression);
            Output(")");
            break;
    }
}

void GeneratorConstant(Constant *constant) {
    Output("%d", constant->value);
}

#include <stdarg.h>
#include <stdio.h>

void Output(char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
