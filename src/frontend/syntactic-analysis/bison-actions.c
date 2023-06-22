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
    return createWhileStatement(cond, block);
}

FunctionCall *FunctionDeclarationGrammarAction(char *varname, Declaration *declaration, FunctionCallType callType) {
    LogDebug("\tFunctionDeclarationGrammarAction");

    struct key key = {.varname = varname};
    if (!symbolTableFind(&key, NULL)) {
        LogError("Variable %s undeclared", varname);
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

Assignment *AssignmentGrammarAction(char *var, Expression *exp) {
    LogDebug("\tAssignmentGrammarAction");

    struct key key = {.varname = var};
    struct value value;
    if (!symbolTableFind(&key, &value)) {
        LogError("Variable %s undeclared", var);
        exit(1);
    }

    if (value.type != VAR_INT) {
        LogError("Variable %s is not an integer", var);
        exit(1);
    }

    value.metadata.hasValue = true;
    symbolTableInsert(&key, &value);

    return createAssignment(var, exp);
}

RangeExpression *RangeExpressionGrammarAction(Expression *exp1, Expression *exp2) {
    LogDebug("\tRangeExpressionGrammarAction");
    return createRangeExpression(exp1, exp2);
}

Expression *ExpressionGrammarAction(Expression *left, Expression *right, Factor *factor, ExpressionType type) {
    LogDebug("\tExpressionGrammarAction of type (%d)", type);
    return createExpression(type, left, right, factor);
}

Factor *FactorGrammarAction(Expression *exp, Constant *con, char *varname, FactorType type) {
    LogDebug("\tFactorGrammarAction of type (%d)", type);

    if (type == VARIABLE_FACTOR) {
        struct key key = {.varname = varname};
        struct value value;

        AddUsedSymbol(varname, VAR_INT);
    }
    return createFactor(type, exp, con, varname);
}

Constant *ConstantGrammarAction(int value) {
    LogDebug("\tConstantGrammarAction");
    return createConstant(value);
}

// TODO: add assignment
static inline VarType SymbolTableDeclareAux(char *varname, DeclarationType type, bool hasValue) {
    VarType varType;
    struct metadata metadata = {.hasValue = hasValue};
    switch (type) {
    case INT_DECLARATION:
        varType = VAR_INT;
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
    // TODO: ver lo de assingment
    return createDeclaration(varType, varname, NULL);
}

Declaration *IntDeclarationAndAssignmentGrammarAction(char *varname, Expression *exp) {
    LogDebug("\tIntDeclarationAndAssignmentGrammarAction");
    // TODO: calcular value?
    VarType varType = SymbolTableDeclareAux(varname, INT_DECLARATION, true);

    Assignment *assignment = createAssignment(varname, exp);
    return createDeclaration(varType, varname, assignment);
}

int IntegerConstantGrammarAction(int value) {
    LogDebug("\tIntegerConstantGrammarAction(%d)", value);
    return value;
}

// Adds to the used symbols array
static void AddUsedSymbol(char *varname, VarType expectedType) {
    struct key key = {.varname = varname};

    if (usedSymbolsCount == 0) {
        usedSymbols = malloc(sizeof(struct key));
        usedSymbolsExpectedType = malloc(sizeof(VarType));
    } else if (usedSymbolsCount % 10 == 0) {
        usedSymbols = realloc(usedSymbols, sizeof(struct key) * (usedSymbolsCount + 10));
        usedSymbolsExpectedType = realloc(usedSymbolsExpectedType, sizeof(VarType) * (usedSymbolsCount + 10));
    }

    usedSymbols[usedSymbolsCount] = key;
    usedSymbolsExpectedType[usedSymbolsCount++] = expectedType;
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
