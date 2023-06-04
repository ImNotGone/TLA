#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;
typedef struct StatementList StatementList;
/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/
typedef struct {
	int value;
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
	INT,
	RBT,
	AVL,
	BST,
} SymbolType;

// TODO: agregar value?
typedef struct {
	SymbolType type;
	char * name;
} Symbol;

typedef enum {
	EXPRESSION,
	CONSTANT,
	SYMBOL
} FactorType;

typedef struct {
	FactorType type;
	Expression * expression;
	Constant constant;
	Symbol symbol;
} Factor;

typedef enum {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	MODULUS,
	AND,
	NOT,
	OR,
	EQUALS,
	NOT_EQUALS,
	LESS_THAN,
	LEES_EQUAL,
	GREATER_THAN,
	GREATER_EQUAL,
	FACTOR
} ExpressionType;

struct Expression {
	ExpressionType type;
	Expression * leftExpression;
	Expression * rightExpression;
	Factor * factor;
};

typedef struct {
	StatementList * statements;
} Block;

typedef enum {
	IF_ELSE,
	IF
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
	Symbol symbol;
	RangeExpression * range;
	Block * block;
} ForStatement;

typedef struct {
	Expression * expression;
	Block * block;
} WhileStatement;

typedef enum {
	PRINT,
	MAX,
	MIN,
	ROOT,
	HEIGHT,
	INSERT,
	REMOVE,
	INORDER,
	POSTORDER,
	PREORDER,
	REDUCE,
	FIND,
	PRESENT,
	ADD_TREE
} FunctionCallType;

typedef enum {
	TREE_DECLARATION,
	INT_DECLARATION,
	// INT_DECLARATION_AND_ASSIGNMENT?
} DeclarationType;

typedef struct {
	DeclarationType type;
	Symbol * symbol;
	// TODO: agregar los campos necesarios ?
} Declaration;

typedef struct {
	FunctionCallType type;
	Symbol * symbol;
	Expression * expression;
	Declaration * declaration;
} FunctionCall;

typedef struct {
	Symbol * symbol;
	Expression * expression;
} Assignment;

typedef enum {
	IF_STATEMENT,
	FOR_STATEMENT,
	WHILE_STATEMENT,
	FUNCTION_CALL,
	DECLARATION,
	ASSIGNMENT,
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

struct StatementList {
	Statement statement;
	StatementList * next;
};

typedef struct {
	StatementList * statements;
} Program;

#endif
