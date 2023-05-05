#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
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
int ProgramGrammarAction(const int value) {
	LogDebug("\tProgramGrammarAction(%d)", value);
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
	state.result = value;
	return value;
}

int BlockGrammarAction(int statementList) {
    LogDebug("\tStGrammarAction(%d)", statementList);
    return statementList;;
}

int StatementListGrammarAction(const int leftValue, const int rightValue) {
    LogDebug("\tStatementListGrammarAction(%d, %d)", leftValue, rightValue);
    return 1;
}

int StatementGrammarAction(const int value){
    LogDebug("\tStatementGrammarAction(%d)", value);
    return 1;
}

int IfStatementGrammarAction(const int exp, const int block1, const int block2) {
    //Todo ver impresion de block2 en caso de NULL
//    LogDebug("\tIfStatementGrammarAction(%d, %d, %d)", exp, block1, block2);
    return 1;
}

int ForStatementGrammarAction(const int symbol, const int range_expression, const int block){
    LogDebug("\tForStatementGrammarAction(%d, %d, %d)", symbol, range_expression, block);
    return 1;
}

int WhileStatementGrammarAction(const int expression, const int block){
    LogDebug("\tWhileStatementGrammarAction(%d, %d)", expression, block);
    return 1;
}

int PrintFunctionGrammarAction(const int symbol){
    LogDebug("\tPrintFunctionGrammarAction(%d)", symbol);
    return 1;
}

int MaxFunctionGrammarAction(const int symbol){
    LogDebug("\tMaxFunctionGrammarAction(%d)", symbol);
    return 1;
}

int MinFunctionGrammarAction(const int symbol){
    LogDebug("\tMinFunctionGrammarAction(%d)", symbol);
    return 1;
}

int RootFunctionGrammarAction(const int symbol){
    LogDebug("\tRootFunctionGrammarAction(%d)", symbol);
    return 1;
}

int HeightFunctionGrammarAction(const int symbol){
    LogDebug("\tHeightFunctionGrammarAction(%d)", symbol);
    return 1;
}

int InorderFunctionGrammarAction(const int symbol){
    LogDebug("\tInorderFunctionGrammarAction(%d)", symbol);
    return 1;
}

int PostorderFunctionGrammarAction(const int symbol){
    LogDebug("\tPostorderFunctionGrammarAction(%d)", symbol);
    return 1;
}

int PreorderFunctionGrammarAction(const int symbol){
    LogDebug("\tPreorderFunctionGrammarAction(%d)", symbol);
    return 1;
}

int InsertFunctionGrammarAction(const int symbol1, const int symbol2){
    LogDebug("\tInsertFunctionGrammarAction(%d, %d)", symbol1, symbol2);
    return 1;
}

int RemoveFunctionGrammarAction(const int symbol1, const int symbol2){
    LogDebug("\tRemoveFunctionGrammarAction(%d, %d)", symbol1, symbol2);
    return 1;
}

int ReduceFunctionGrammarAction(const int symbol1, const int symbol2){
    LogDebug("\tReduceFunctionGrammarAction(%d, %d)", symbol1, symbol2);
    return 1;
}

int FindFunctionGrammarAction(const int symbol1, const int symbol2){
    LogDebug("\tFindFunctionGrammarAction(%d, %d)", symbol1, symbol2);
    return 1;
}

int PresentFunctionGrammarAction(const int symbol1, const int symbol2){
    LogDebug("\tPresentFunctionGrammarAction(%d, %d)", symbol1, symbol2);
    return 1;
}

int DeclarationFunctionGrammarAction(const int symbol1, const int symbol2){
    LogDebug("\tDeclarationFunctionGrammarAction(%d, %d)", symbol1, symbol2);
    return 1;
}

int TreeDeclarationGrammarAction(const int symbol){
    LogDebug("\tTreeDeclarationGrammarAction(%d)", symbol);
    return 1;
}

int IntDeclarationGrammarAction(const int symbol){
    LogDebug("\tIntDeclarationGrammarAction(%d)", symbol);
    return 1;
}

int IntDeclarationAndAssignmentGrammarAction(const int symbol, const int expression){
    LogDebug("\tIntDeclarationAndAssignmentGrammarAction(%d, %d)", symbol, expression);
    return 1;
}

int AssignmentGrammarAction(const int symbol, const int expression) {
    LogDebug("\tAssignmentGrammarAction(%d, %d)", symbol, expression);
    return 1;
}

int RangeExpressionGrammarAction(const int exp1, const int exp2){
    LogDebug("\tRangeExpressionGrammarAction(%d, %d)", exp1, exp2);
    return 1;
}

int ParenthesisFactorGrammarAction(const int value){
    LogDebug("\tParenthesisFactorGrammarAction(%d)", value);
    return 1;
}

int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tAdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Add(leftValue, rightValue);
}

int SubstractionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tSubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Subtract(leftValue, rightValue);
}

int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tMultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Multiply(leftValue, rightValue);
}

int DivisionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tDivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Divide(leftValue, rightValue);
}

int ModulusExpressionGrammarAction(const int leftValue, const int rightValue) {
    LogDebug("\tModulusExpressionGrammarAction(%d, %d)", leftValue, rightValue);
    return Modulus(leftValue, rightValue);
}

int AndExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tAndExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return And(leftValue, rightValue);
    return 1;
}

int OrExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tOrExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return Or(leftValue, rightValue);
    return 1;
}

int NotExpressionGrammarAction(const int value){
    LogDebug("\tNotExpressionGrammarAction(%d)", value);
//    return Not(value);
    return 1;
}

int GreaterThanExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tGreaterThanExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return Greater(leftValue, rightValue);
    return 1;

}

int GreaterThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tGreaterThanOrEqualExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return GreaterOrEqual(leftValue, rightValue);
    return 1;

}

int LessThanExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tLessThanExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return Lesser(leftValue, rightValue);
    return 1;

}

int LessThanOrEqualExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tLessThanOrEqualExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return LesserOrEqual(leftValue, rightValue);
    return 1;

}

int EqualityExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tEqualityExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return Equal(leftValue, rightValue);
    return 1;
}

int InequalityExpressionGrammarAction(const int leftValue, const int rightValue){
    LogDebug("\tInequalityExpressionGrammarAction(%d, %d)", leftValue, rightValue);
//    return NotEqual(leftValue, rightValue);
    return 1;
}

int FactorExpressionGrammarAction(const int value) {
	LogDebug("\tFactorExpressionGrammarAction(%d)", value);
	return value;
}

int ExpressionFactorGrammarAction(const int value) {
	LogDebug("\tExpressionFactorGrammarAction(%d)", value);
	return value;
}

int ConstantFactorGrammarAction(const int value) {
	LogDebug("\tConstantFactorGrammarAction(%d)", value);
	return value;
}

int ConstantGrammarAction(const int value) {
    LogDebug("\tConstantGrammarAction(%d)", value);
    return value;
}

int VariableFactorGrammarAction(const int value) {
    LogDebug("\tVariableFactorGrammarAction(%d)", value);
    return value;
}

int TreeTypeGrammarAction(const int value) {
    LogDebug("\tTreeTypeGrammarAction(%d)", value);
    return value;
}

int IntegerConstantGrammarAction(const int value) {
	LogDebug("\tIntegerConstantGrammarAction(%d)", value);
	return value;
}


