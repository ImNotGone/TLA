#include "bison-actions.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include "../../backend/semantic-analysis/symbol-table.h"
#include "../../backend/semantic-analysis/tree-utils.h"
#include "../../backend/support/logger.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "bison-grammar.h".
 */

/**
 * Esta función se ejecuta cada vez que se emite un error de sintaxis.
 */
void yyerror(const char *string) {
    LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
    LogError("En ASCII es:");
    LogErrorRaw("\t");
    const int length = strlen(yytext);
    for (int i = 0; i < length; ++i) {
        LogErrorRaw("[%d]", yytext[i]);
    }
    LogErrorRaw("\n\n");
}

/* Vector de variables usadas. */
static struct key *usedSymbols;
static VarType *usedSymbolsExpectedType;
static int usedSymbolsCount = 0;

/* Funciones auxiliares para la construcción del AST. */
static inline VarType SymbolTableDeclareAux(char *varname, DeclarationType type, bool hasValue);

static void AddUsedSymbol(char *varname, VarType expectedType);
static int getExpressionType(Expression *expression);
static int getFunctionCallType(FunctionCall *functionCall);
static void ValidateUsedSymbols();

/**
 * Esta acción se corresponde con el no-terminal que representa el símbolo
 * inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
 * indica que efectivamente el programa de entrada se pudo generar con esta
 * gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
 */
Program *ProgramGrammarAction(StatementList statements) {
    LogDebug("\tProgramGrammarAction");

    ValidateUsedSymbols();

    free(usedSymbols);
    free(usedSymbolsExpectedType);

    /*
     * "state" es una variable global que almacena el estado del compilador,
     * cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
     * es utilizada en la función "main".
     */
    state.succeed = true;
    state.program = createProgram(statements);
    /*
     * Por otro lado, "result" contiene el resultado de aplicar el análisis
     * sintáctico mediante Bison, y almacenar el nood raíz del AST construido
     * en esta variable. Para el ejemplo de la calculadora, no hay AST porque
     * la expresión se computa on-the-fly, y es la razón por la cual esta
     * variable es un simple entero, en lugar de un nodo.
     */
    // state.result = value;
    // return value;
    return state.program;
}

Block *BlockGrammarAction(StatementList statements) {
    LogDebug("\tBlockGrammarAction");
    return createBlock(statements);
}

StatementList StatementListGrammarAction(Statement *statement, StatementList next) {
    LogDebug("\tStatementListGrammarAction");
    return createStatementList(statement, next);
}

Statement *StatementGrammarAction(void *statement, StatementType type) {
    LogDebug("\tStatementGrammarAction for (%d)", type);
    return createStatement(type, statement);
}

IfStatement *IfStatementGrammarAction(Expression *cond, Block *if_block, Block *else_block) {
    // Todo ver impresion de block2 en caso de NULL
    //    LogDebug("\tIfStatementGrammarAction(%d, %d, %d)", exp, block1, block2);
    
    if (getExpressionType(cond) != VAR_BOOL) {
        LogError("Condition must be a boolean expression");
        exit(1);
    }

    IfStatementType type = IF_ELSE_TYPE;
    if (else_block == NULL) {
        type = IF_TYPE;
    }
    return createIfStatement(type, cond, if_block, else_block);
}

ForStatement *ForStatementGrammarAction(char *varname, RangeExpression *range, Block *block) {
    LogDebug("\tForStatementGrammarAction");

    SymbolTableDeclareAux(varname, INT_DECLARATION, true);

    return createForStatement(varname, range, block);
}

WhileStatement *WhileStatementGrammarAction(Expression *cond, Block *block) {
    LogDebug("\tWhileStatementGrammarAction");

    if (getExpressionType(cond) != VAR_BOOL) {
        LogError("Condition must be a boolean expression");
        exit(1);
    }

    return createWhileStatement(cond, block);
}

FunctionCall *FunctionDeclarationGrammarAction(char *varname, Declaration *declaration, FunctionCallType callType) {
    LogDebug("\tFunctionDeclarationGrammarAction");

    struct key key = {.varname = varname};
    if (!symbolTableFind(&key, NULL)) {
        LogError("Variable %s undeclared", varname);
        exit(1);
    }

    if (declaration->type == INT_DECLARATION) {
        LogError("Variable %s is an integer", declaration->varname);
        exit(1);
    }

    return createFunctionCall(callType, varname, NULL, declaration);
}

FunctionCall *FunctionGrammarAction(char *varname, Expression *exp, FunctionCallType type) {
    LogDebug("\tFunctionStatementGrammarAction of type (%d)", type);
    struct key key = {.varname = varname};
    if (!symbolTableFind(&key, NULL)) {
        LogError("Variable %s undeclared", varname);
        exit(1);
    }

    return createFunctionCall(type, varname, exp, NULL);
}

Assignment *AssignmentGrammarAction(char *var, Expression *exp, FunctionCall *functionCall) {
    LogDebug("\tAssignmentGrammarAction");

    struct key key = {.varname = var};
    struct value value;
    if (!symbolTableFind(&key, &value)) {
        LogError("Variable %s undeclared", var);
        exit(1);
    }

    if (value.type != VAR_INT && value.type != VAR_BOOL) {
        LogError("Variable %s is not an integer nor a boolean", var);
        exit(1);
    }

    if (functionCall != NULL && getFunctionCallType(functionCall) != value.type) {
        LogError("Function return cannot be assigned to %s", var);
        exit(1);
    }

    if (exp != NULL && getExpressionType(exp) != value.type) {
        LogError("Expression cannot be assigned to %s", var);
        exit(1);
    }

    value.metadata.hasValue = true;
    symbolTableInsert(&key, &value);

    return createAssignment(var, exp, functionCall);
}

RangeExpression *RangeExpressionGrammarAction(Expression *exp1, Expression *exp2) {
    LogDebug("\tRangeExpressionGrammarAction");

    if ((exp1 != NULL && getExpressionType(exp1) != VAR_INT) || (exp2 && getExpressionType(exp2) != VAR_INT)) {
        LogError("Parametros invalidos\n");
        exit(1);
    }
    return createRangeExpression(exp1, exp2);
}

Expression *ExpressionGrammarAction(Expression *left, Expression *right, Factor *factor, ExpressionType type) {
    LogDebug("\tExpressionGrammarAction of type (%d)", type);
    
    VarType leftType, rightType;
    if (left != NULL && right != NULL && (leftType = getExpressionType(left)) != (rightType = getExpressionType(right))) {
        LogError("Parametros invalidos\n");
        exit(1);
    }

    VarType expectedFactorType;
    if (type == AND_EXPRESSION || type == OR_EXPRESSION || type == NOT_EXPRESSION) {
        expectedFactorType = VAR_BOOL;
    } else {
        expectedFactorType = VAR_INT;
    }
    
    if (left != NULL && left->type == FACTOR_EXPRESSION && left->factor->type == VARIABLE_FACTOR) {
        AddUsedSymbol(left->factor->varname, expectedFactorType);
    }

    if (right != NULL && right->type == FACTOR_EXPRESSION && right->factor->type == VARIABLE_FACTOR) {
        AddUsedSymbol(right->factor->varname, expectedFactorType);
    }

    return createExpression(type, left, right, factor);
}

Factor *FactorGrammarAction(Expression *exp, Constant *con, char *varname, FactorType type) {
    LogDebug("\tFactorGrammarAction of type (%d)", type);

    
    return createFactor(type, exp, con, varname);
}

Constant *ConstantGrammarAction(int intValue, bool boolValue, ConstantType type) {
    LogDebug("\tConstantGrammarAction");
    return createConstant(intValue, boolValue, type);
}

static inline VarType SymbolTableDeclareAux(char *varname, DeclarationType type, bool hasValue) {
    VarType varType;
    struct metadata metadata = {.hasValue = hasValue};
    switch (type) {
    case INT_DECLARATION:
        varType = VAR_INT;
        break;
    case BOOL_DECLARATION:
        varType = VAR_BOOL;
        break;
    case RBT_DECLARATION:
        varType = VAR_RBT;
        break;
    case AVL_DECLARATION:
        varType = VAR_AVL;
        break;
    case BST_DECLARATION:
        varType = VAR_BST;
        break;
    default:
        assert(0 && "Illegal State");
        break;
    }
    struct key key = {.varname = varname};
    if (symbolTableFind(&key, NULL)) {
        LogError("Redeclaration of var, %s", varname);
        exit(1);
    }
    struct value value = {.type = varType, .metadata = metadata};
    symbolTableInsert(&key, &value);
    return varType;
}

Declaration *DeclarationGrammarAction(char *varname, DeclarationType type) {
    LogDebug("\tDeclarationGrammarAction");
    VarType varType = SymbolTableDeclareAux(varname, type, false);
    return createDeclaration(varType, varname, NULL);
}

Declaration *IntDeclarationAndAssignmentGrammarAction(char *varname, Expression *exp, FunctionCall *functionCall) {
    LogDebug("\tIntDeclarationAndAssignmentGrammarAction");
    VarType varType = SymbolTableDeclareAux(varname, INT_DECLARATION, true);

    if (functionCall != NULL && getFunctionCallType(functionCall) != VAR_INT) {
        LogError("Function return cannot be assigned to %s", varname);
        exit(1);
    }

    if (exp != NULL && getExpressionType(exp) != VAR_INT) {
        LogError("Expression cannot be assigned to %s", varname);
        exit(1);
    }

    Assignment *assignment = createAssignment(varname, exp, functionCall);
    return createDeclaration(varType, varname, assignment);
}

Declaration *BoolDeclarationAndAssignmentGrammarAction(char *varname, Expression *exp, FunctionCall *functionCall) {
    LogDebug("\tBoolDeclarationAndAssignmentGrammarAction");
    VarType varType = SymbolTableDeclareAux(varname, BOOL_DECLARATION, true);

    if (functionCall != NULL && getFunctionCallType(functionCall) != VAR_BOOL) {
        LogError("Function return cannot be assigned to %s", varname);
        exit(1);
    }

    if (exp != NULL && getExpressionType(exp) != VAR_BOOL) {
        LogError("Expression cannot be assigned to %s", varname);
        exit(1);
    }

    Assignment *assignment = createAssignment(varname, exp, functionCall);
    return createDeclaration(varType, varname, assignment);
}

// Adds to the used symbols array
static void AddUsedSymbol(char *varname, VarType expectedType) {
    struct key key = {.varname = varname};

    for (int i = 0; i < usedSymbolsCount; i++) {
        if (strcmp(usedSymbols[i].varname, varname) == 0) {
            if (usedSymbolsExpectedType[i] != expectedType) {
                LogError("Variable %s conflicting types", varname);
                exit(1);
            }
            return;
        }
    }

    if (usedSymbolsCount == 0) {
        usedSymbols = malloc(sizeof(struct key));
        usedSymbolsExpectedType = malloc(sizeof(VarType));
    } else if (usedSymbolsCount % 10 == 0) {
        usedSymbols = realloc(usedSymbols, sizeof(struct key) * (usedSymbolsCount + 10));
        usedSymbolsExpectedType = realloc(usedSymbolsExpectedType, sizeof(VarType) * (usedSymbolsCount + 10));
    }

    printf("Adding %s\n", varname);

    usedSymbols[usedSymbolsCount] = key;
    usedSymbolsExpectedType[usedSymbolsCount++] = expectedType;

    printf("Added %s\n", usedSymbols[usedSymbolsCount - 1].varname);
}

// Validates that all the symbols used in the program are declared
// and have a value
static void ValidateUsedSymbols() {
    for (int i = 0; i < usedSymbolsCount; i++) {
        struct key key = usedSymbols[i];
        struct value value;
        if (!symbolTableFind(&key, &value)) {
            LogError("Variable %s undeclared", key.varname);
            exit(1);
        }

        if (value.metadata.hasValue == false) {
            LogError("Variable %s has no value", key.varname);
            exit(1);
        }

        if (value.type != usedSymbolsExpectedType[i]) {
            LogError("Variable %s is not of type %d", key.varname, usedSymbolsExpectedType[i]);
            exit(1);
        }
    }
}

static int getExpressionType(Expression *expression) {
    struct key key;
    struct value value;
    switch (expression->type) {
    case ADDITION_EXPRESSION:
    case SUBTRACTION_EXPRESSION:
    case MULTIPLICATION_EXPRESSION:
    case DIVISION_EXPRESSION:
    case MODULUS_EXPRESSION:
        return (getExpressionType(expression->leftExpression) == VAR_INT && getExpressionType(expression->rightExpression) == VAR_INT)
                   ? VAR_INT
                   : -1;
    case AND_EXPRESSION:
    case OR_EXPRESSION:
        return (getExpressionType(expression->leftExpression) == VAR_BOOL && getExpressionType(expression->rightExpression) == VAR_BOOL)
                   ? VAR_BOOL
                   : -1;
    case NOT_EXPRESSION:
        return getExpressionType(expression->leftExpression) == VAR_BOOL ? VAR_BOOL : -1;
    case EQUALS_EXPRESSION:
    case NOT_EQUALS_EXPRESSION:
    case LESS_THAN_EXPRESSION:
    case LEES_EQUAL_EXPRESSION:
    case GREATER_THAN_EXPRESSION:
    case GREATER_EQUAL_EXPRESSION:
        return (getExpressionType(expression->leftExpression) == getExpressionType(expression->rightExpression))
                   ? VAR_BOOL
                   : -1;
    case FACTOR_EXPRESSION:
        switch (expression->factor->type) {
        case CONSTANT_FACTOR:
            return expression->factor->constant->type == INT_CONSTANT ? VAR_INT : VAR_BOOL;
        case EXPRESSION_FACTOR:
            return getExpressionType(expression->factor->expression);
        case VARIABLE_FACTOR:
            key.varname = expression->factor->varname;
            if (!symbolTableFind(&key, &value)) {
                // It is a for loop iterator
                return VAR_INT;
            }
            return value.type;
        default:
            LogError("Invalid factor type");
            exit(1);
        }
    default:
        LogError("Invalid expression type");
        exit(1);
    }
}

static int getFunctionCallType(FunctionCall *functionCall) {
    switch (functionCall->type) {
    case MAX_CALL:
    case MIN_CALL:
    case HEIGHT_CALL:
        return VAR_INT;
    case PRESENT_CALL:
        return VAR_BOOL;
    default:
        return -1;
    }
}
