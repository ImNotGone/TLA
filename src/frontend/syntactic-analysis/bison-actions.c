#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include "../../backend/semantic-analysis/tree-utils.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>

/**
 * Implementación de "bison-grammar.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
	LogError("En ASCII es:");
	LogErrorRaw("\t");
	const int length = strlen(yytext);
	for (int i = 0; i < length; ++i) {
		LogErrorRaw("[%d]", yytext[i]);
	}
	LogErrorRaw("\n\n");
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program * ProgramGrammarAction(const StatementList statements) {
	LogDebug("\tProgramGrammarAction");
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	//state.result = value;
	//return value;
    return createProgram(statements);
}

Block * BlockGrammarAction(const StatementList statements) {
    LogDebug("\tBlockGrammarAction");
    return createBlock(statements);
}

StatementList StatementListGrammarAction(const Statement * statement, const StatementList next) {
    LogDebug("\tStatementListGrammarAction");
    return createStatementList(statement, next);
}

Statement * StatementGrammarAction(const void * statement, const StatementType type) {
    LogDebug("\tStatementGrammarAction for (%d)", type);
    return createStatement(type, statement);
}

IfStatement * IfStatementGrammarAction(const Expression * cond, const Block * if_block, const Block * else_block) {
    //Todo ver impresion de block2 en caso de NULL
//    LogDebug("\tIfStatementGrammarAction(%d, %d, %d)", exp, block1, block2);
    IfStatementType type = IF_TYPE;
    if (else_block == NULL) {
        type = IF_ELSE_TYPE;
    }
    return createIfStatement(type, cond, if_block, else_block);
}

ForStatement * ForStatementGrammarAction(const char * varname, const RangeExpression * range, const Block * block) {
    LogDebug("\tForStatementGrammarAction");
    // TODO: ver q hacer con variable
    return createForStatement(varname, range, block);
}

WhileStatement * WhileStatementGrammarAction(const Expression * cond, const Block * block) {
    LogDebug("\tWhileStatementGrammarAction");
    return createWhileStatement(cond, block);
}

FunctionCall * FunctionGrammarAction(const char * varname, const Expression * exp, const FunctionCallType type) {
    LogDebug("\tFunctionStatementGrammarAction of type (%d)", type);
    // TODO: ver q hacer con variable
    // TODO: ver q hacer con declaration
    return createFunctionCall(type, varname, exp, NULL);
}

Assignment * AssignmentGrammarAction(const char * var, const Expression * exp) {
    LogDebug("\tAssignmentGrammarAction");
    // TODO: ver q hacer con variable
    return createAssignment(var, exp);
}

RangeExpression * RangeExpressionGrammarAction(const Expression * exp1, const Expression * exp2){
    LogDebug("\tRangeExpressionGrammarAction");
    return createRangeExpression(exp1, exp2);
}

Expression * ExpressionGrammarAction(const Expression * left, const Expression * right, const Factor * factor, const ExpressionType type) {
    LogDebug("\tExpressionGrammarAction of type (%d)", type);
    return createExpression(type, left, right, factor);
}

Factor * FactorGrammarAction(const Expression * exp, const Constant * con, const char * varname, const FactorType type) {
    LogDebug("\tFactorGrammarAction of type (%d)", type);
    // TODO: ver q hacer con variable
    return createFactor(type, exp, con, varname);
}

Constant * ConstantGrammarAction(const int value) {
    LogDebug("\tConstantGrammarAction");
    return createConstant(value);
}

Declaration * DeclarationGrammarAction(const char * varname, const DeclarationType type) {
    LogDebug("\tDeclarationGrammarAction");
    // TODO: ver q hacer con variable
    // TODO: ver lo de assingment
    return createDeclaration(type, varname, NULL);
}

Declaration * IntDeclarationAndAssignmentGrammarAction(const char * varname, const Expression * exp) {
    LogDebug("\tIntDeclarationAndAssignmentGrammarAction");
    // TODO: ver q hacer con variable
    // TODO: ver q hacer con exp
    return createDeclaration(INT_DECLARATION, varname, NULL);
}

int IntegerConstantGrammarAction(const int value) {
	LogDebug("\tIntegerConstantGrammarAction(%d)", value);
	return value;
}

