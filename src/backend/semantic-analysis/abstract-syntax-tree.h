#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER
#include "symbol-table.h"

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;
typedef struct StatementNode * StatementList;
/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/

typedef enum {
    INT_CONSTANT,
    BOOL_CONSTANT,
} ConstantType;

typedef struct {
	int intValue;
    bool boolValue;
    ConstantType type;
} Constant;

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/

typedef enum {
	EXPRESSION_FACTOR,
	CONSTANT_FACTOR,
	VARIABLE_FACTOR
} FactorType;

typedef struct {
	FactorType type;
	Expression * expression;
	Constant * constant;
	char * varname;
} Factor;

typedef enum {
	ADDITION_EXPRESSION,
	SUBTRACTION_EXPRESSION,
	MULTIPLICATION_EXPRESSION,
	DIVISION_EXPRESSION,
	MODULUS_EXPRESSION,
	AND_EXPRESSION,
	NOT_EXPRESSION,
	OR_EXPRESSION,
	EQUALS_EXPRESSION,
	NOT_EQUALS_EXPRESSION,
	LESS_THAN_EXPRESSION,
	LEES_EQUAL_EXPRESSION,
	GREATER_THAN_EXPRESSION,
	GREATER_EQUAL_EXPRESSION,
	FACTOR_EXPRESSION
} ExpressionType;

struct Expression {
	ExpressionType type;
	Expression * leftExpression;
	Expression * rightExpression;
	Factor * factor;
};

typedef struct {
	StatementList statements;
} Block;

typedef enum {
	IF_ELSE_TYPE,
	IF_TYPE,
} IfStatementType;

typedef struct {
	IfStatementType type;
	Expression * condition;
	Block * blockIf;
	Block * blockElse;
} IfStatement;

typedef struct {
	Expression * expressionLeft;
	Expression * expressionRight;
} RangeExpression;

typedef struct {
	char * varname;
	RangeExpression * range;
	Block * block;
} ForStatement;

typedef struct {
	Expression * condition;
	Block * block;
} WhileStatement;

typedef enum {
	PRINT_CALL,
	MAX_CALL,
	MIN_CALL,
	ROOT_CALL,
	HEIGHT_CALL,
	INSERT_CALL,
	REMOVE_CALL,
	INORDER_CALL,
	POSTORDER_CALL,
	PREORDER_CALL,
	REDUCE_EVEN_CALL,
    REDUCE_ODD_CALL,
	FIND_CALL,
	PRESENT_CALL,
	ADD_TREE_CALL
} FunctionCallType;

typedef enum {
    RBT_DECLARATION,
    BST_DECLARATION,
    AVL_DECLARATION,
	INT_DECLARATION,
	// INT_DECLARATION_AND_ASSIGNMENT?
    BOOL_DECLARATION,
} DeclarationType;


typedef struct {
	VarType type;
	char * varname;
    struct Assignment * assignment;
} Declaration;

typedef struct {
	FunctionCallType type;
	char * varname;
	Expression * expression;
	Declaration * declaration;
} FunctionCall;

typedef struct Assignment {
	char * varname;
	Expression * expression;
    FunctionCall * functionCall;
} Assignment;

typedef enum {
	IF_STATEMENT,
	FOR_STATEMENT,
	WHILE_STATEMENT,
	FUNCTION_CALL_STATEMENT,
	DECLARATION_STATEMENT,
	ASSIGNMENT_STATEMENT,
} StatementType;

typedef struct {
	StatementType type;
	IfStatement * ifStatement;
	ForStatement * forStatement;
	WhileStatement * whileStatement;
	FunctionCall * functionCall;
	Declaration * declaration;
	Assignment * assignment;
} Statement;

struct StatementNode {
	Statement * statement;
	StatementList next;
};

typedef struct {
	StatementList statements;
} Program;

#endif
