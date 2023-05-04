%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
    int block;
    int statement_list;
    int statement;
    int if_statement;
    int for_statement;
    int while_statement;
    int function_call;
    int declaration;
    int assignment;
    int expression;
    int range_expression;
    int factor;
    int constant;
    int tree_type;


    char * varname;
	// Terminales.
	token token;
	int integer;
}

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> ADD SUB MUL DIV MOD
%token <token> AND OR NOT
%token <token> EQ NE LT LE GT GE
%token <token> ASSIGN
%token <token> COMMA
%token <token> OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_CURL_BRACKETS CLOSE_CURL_BRACKETS SEMI_COLON
%token <token> FOR IN WHILE IF ELSE
%token <token> INT_TYPE
%token <token> RED_BLACK_TREE_TYPE AVL_TREE_TYPE BST_TREE_TYPE
%token <token> NEW_TREE PRINT INSERT REMOVE INORDER POSTORDER PREORDER REDUCE FIND MATCH ADD_TREE
%token <token> SYMBOL

%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <block> block
%type <statement_list> statement_list
%type <statement> statement
%type <if_statement> if_statement
%type <for_statement> for_statement
%type <while_statement> while_statement
%type <function_call> function_call
%type <declaration> declaration
%type <assignment> assignment
%type <expression> expression
%type <range_expression> range_expression
%type <factor> factor
%type <constant> constant
%type <tree_type> tree_type

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV
%left MOD
%left AND OR NOT
%left EQ NE
%left LT LE GT GE
%left ASSIGN

// El símbolo inicial de la gramatica.
%start program

%%

program: block                                                                          { $$ = ProgramGrammarAction($1); }
       ;

block: OPEN_CURL_BRACKETS statement_list CLOSE_CURL_BRACKETS                             { $$ = BlockGrammarAction($2); }
     ;

statement_list: statement_list statement                                                  { $$ = StatementListGrammarAction($1, $2); }
              | statement                                                                 { $$ = StatementListGrammarAction(-1, $1); }
              ;

statement: if_statement                                                                  { $$ = StatementGrammarAction($1); }
         | for_statement                                                                 { $$ = StatementGrammarAction($1); }
         | while_statement                                                               { $$ = StatementGrammarAction($1); }
         | function_call SEMI_COLON                                                      { $$ = StatementGrammarAction($1); }
         | declaration SEMI_COLON                                                        { $$ = StatementGrammarAction($1); }
         | assignment SEMI_COLON                                                         { $$ = StatementGrammarAction($1); }
         ;

if_statement: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS block ELSE block          { $$ = IfStatementGrammarAction($3, $5, $7); }
            | IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS block                     { $$ = IfStatementGrammarAction($3, $5, -1); }
            ;

for_statement: FOR SYMBOL IN range_expression block                                      { $$ = ForStatementGrammarAction($2, $4, $5); }
             ;

while_statement: WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS block               { $$ = WhileStatementGrammarAction($3, $5); }
               ;

function_call: PRINT SYMBOL                                                              { $$ = OneParamFunctionGrammarAction($2); }
             | INSERT SYMBOL SYMBOL                                                      { $$ = TwoParamFunctionGrammarAction($2, $3); }
             | REMOVE SYMBOL SYMBOL                                                      { $$ = TwoParamFunctionGrammarAction($2, $3); }
             | INORDER SYMBOL                                                            { $$ = OneParamFunctionGrammarAction($2); }
             | POSTORDER SYMBOL                                                          { $$ = OneParamFunctionGrammarAction($2); }
             | PREORDER SYMBOL                                                           { $$ = OneParamFunctionGrammarAction($2); }
             | REDUCE expression SYMBOL                                                  { $$ = TwoParamFunctionGrammarAction($2, $3); }
             | FIND SYMBOL SYMBOL                                                        { $$ = TwoParamFunctionGrammarAction($2, $3); }
             | MATCH expression SYMBOL                                                   { $$ = TwoParamFunctionGrammarAction($2, $3); }
             ;

declaration: NEW_TREE tree_type SYMBOL                                                   { $$ = TreeDeclarationGrammarAction($1); }
           | INT_TYPE SYMBOL                                                             { $$ = IntDeclarationGrammarAction($1); }
           | INT_TYPE SYMBOL ASSIGN expression                                           { $$ = IntDeclarationAndAssignmentGrammarAction($1, $3); } 
           ;

assignment: SYMBOL ASSIGN expression                                                     { $$ = AssignmentGrammarAction($1, $3); }
          ;

expression: expression ADD expression                                                    { $$ = AdditionExpressionGrammarAction($1, $3); }
          | expression SUB expression                                                    { $$ = SubstractionExpressionGrammarAction($1, $3); }
          | expression MUL expression                                                    { $$ = MultiplicationExpressionGrammarAction($1, $3); }
          | expression DIV expression                                                    { $$ = DivisionExpressionGrammarAction($1, $3); }
          | expression MOD expression                                                    { $$ = ModulusExpressionGrammarAction($1, $3); }
          | expression AND expression                                                    { $$ = AndExpressionGrammarAction($1, $3); }
          | NOT expression                                                               { $$ = NotExpressionGrammarAction($2); }
          | expression OR expression                                                     { $$ = OrExpressionGrammarAction($1, $3); }
          | expression EQ expression                                                     { $$ = EqualityExpressionGrammarAction($1, $3); }
          | expression NE expression                                                     { $$ = InequalityExpressionGrammarAction($1, $3); }
          | expression LT expression                                                     { $$ = LessThanExpressionGrammarAction($1, $3); }
          | expression LE expression                                                     { $$ = LessThanOrEqualExpressionGrammarAction($1, $3); }
          | expression GT expression                                                     { $$ = GreaterThanExpressionGrammarAction($1, $3); }
          | expression GE expression                                                     { $$ = GreaterThanOrEqualExpressionGrammarAction($1, $3); }
          | factor                                                                       { $$ = FactorExpressionGrammarAction($1); }
          ;

range_expression: OPEN_PARENTHESIS expression COMMA expression CLOSE_PARENTHESIS         { $$ = RangeExpressionGrammarAction($2, $4); }

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS                                    { $$ = ParenthesisFactorGrammarAction($2); }
      | constant                                                                         { $$ = ConstantFactorGrammarAction($1); }
      | SYMBOL                                                                           { $$ = VariableFactorGrammarAction($1); }
      ;

constant: INTEGER                                                                        { $$ = ConstantGrammarAction($1); }
        ;

tree_type: RED_BLACK_TREE_TYPE                                                           { $$ = TreeTypeGrammarAction($1); }
         | AVL_TREE_TYPE                                                                 { $$ = TreeTypeGrammarAction($1); }
         | BST_TREE_TYPE                                                                 { $$ = TreeTypeGrammarAction($1); }
         ;
%%
