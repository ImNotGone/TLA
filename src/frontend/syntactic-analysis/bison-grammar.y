%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
    Program * program;
    StatementList statement_list;
    Statement * statement;
    Assignment * assignment;
    FunctionCall * function_call;
    Declaration * declaration;
    WhileStatement * while_statement;
    ForStatement * for_statement;
    RangeExpression * range_expression;
    IfStatement * if_statement;
    Block * block;
    Expression * expression;
    Factor * factor;
    Constant * constant;

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
%token <token> INT
%token <token> RBT AVL BST
%token <token> NEW_TREE PRINT INSERT REMOVE INORDER POSTORDER PREORDER REDUCE FIND PRESENT ADD_TREE MAX MIN HEIGHT ROOT

%token <varname> VARIABLE
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

program: statement_list[st_list]                                                         { $$ = ProgramGrammarAction($st_list); }
       ;


block: OPEN_CURL_BRACKETS statement_list[st_list] CLOSE_CURL_BRACKETS                    { $$ = BlockGrammarAction($st_list); }
     ;

statement_list: statement[st] statement_list[next]                                       { $$ = StatementListGrammarAction($st, $next); }
              | statement[st]                                                            { $$ = StatementListGrammarAction($st, NULL); }
              ;

statement: if_statement[st]                                                              { $$ = StatementGrammarAction($st, IF_STATEMENT); }
         | for_statement[st]                                                             { $$ = StatementGrammarAction($st, FOR_STATEMENT); }
         | while_statement[st]                                                           { $$ = StatementGrammarAction($st, WHILE_STATEMENT); }
         | function_call[st] SEMI_COLON                                                  { $$ = StatementGrammarAction($st, FUNCTION_CALL_STATEMENT); }
         | declaration[st] SEMI_COLON                                                    { $$ = StatementGrammarAction($st, DECLARATION_STATEMENT); }
         | assignment[st] SEMI_COLON                                                     { $$ = StatementGrammarAction($st, ASSIGNMENT_STATEMENT); }
         ;

if_statement: IF OPEN_PARENTHESIS expression[cond] CLOSE_PARENTHESIS block[if_block] ELSE block[else_block] { $$ = IfStatementGrammarAction($cond, $if_block, $else_block); }
            | IF OPEN_PARENTHESIS expression[cond] CLOSE_PARENTHESIS block[if_block]                        { $$ = IfStatementGrammarAction($cond, $if_block, NULL); }
            ;

for_statement: FOR VARIABLE[var] IN range_expression[range] block                        { $$ = ForStatementGrammarAction($var, $range, $block); }
             ;

while_statement: WHILE OPEN_PARENTHESIS expression[cond] CLOSE_PARENTHESIS block         { $$ = WhileStatementGrammarAction($cond, $block); }
               ;

function_call: PRINT VARIABLE[var]                                                       { $$ = FunctionGrammarAction($var, NULL, PRINT_CALL); }
	         | MAX VARIABLE[var]                                                         { $$ = FunctionGrammarAction($var, NULL, MAX_CALL); }
	         | MIN VARIABLE[var]                                                         { $$ = FunctionGrammarAction($var, NULL, MIN_CALL); }
	         | ROOT VARIABLE[var]                                                        { $$ = FunctionGrammarAction($var, NULL, ROOT_CALL); }
	         | HEIGHT VARIABLE[var]                                                      { $$ = FunctionGrammarAction($var, NULL, HEIGHT_CALL); }
             | INSERT VARIABLE[var] expression[exp]                                      { $$ = FunctionGrammarAction($var, $exp, INSERT_CALL); }
             | REMOVE VARIABLE[var] expression[exp]                                      { $$ = FunctionGrammarAction($var, $exp, REMOVE_CALL); }
             | INORDER VARIABLE[var]                                                     { $$ = FunctionGrammarAction($var, NULL, INORDER_CALL); }
             | POSTORDER VARIABLE[var]                                                   { $$ = FunctionGrammarAction($var, NULL, POSTORDER_CALL); }
             | PREORDER VARIABLE[var]                                                    { $$ = FunctionGrammarAction($var, NULL, PREORDER_CALL); }
             | REDUCE expression[exp] VARIABLE[var]                                      { $$ = FunctionGrammarAction($var, $exp, REDUCE_CALL); }
             | FIND VARIABLE[var] expression[exp]                                        { $$ = FunctionGrammarAction($var, $exp, FIND_CALL); }
             | PRESENT expression[exp] VARIABLE[var]                                     { $$ = FunctionGrammarAction($var, $exp, PRESENT_CALL); }
             | declaration[decl] ADD_TREE VARIABLE[var]                                  { $$ = FunctionDeclarationGrammarAction($var, $decl, ADD_TREE_CALL); } // TODO: revisar
             ;
// CHECK: mover el add_tree aca???
declaration: NEW_TREE BST VARIABLE[var]                                                  { $$ = DeclarationGrammarAction($var, BST_DECLARATION); }
           | NEW_TREE AVL VARIABLE[var]                                                  { $$ = DeclarationGrammarAction($var, AVL_DECLARATION); }
           | NEW_TREE RBT VARIABLE[var]                                                  { $$ = DeclarationGrammarAction($var, RBT_DECLARATION); }
           | INT VARIABLE[var]                                                           { $$ = DeclarationGrammarAction($var, INT_DECLARATION); }
           | INT VARIABLE[var] ASSIGN expression[exp]                                    { $$ = IntDeclarationAndAssignmentGrammarAction($var, $exp); }
           ;

assignment: VARIABLE[var] ASSIGN expression[exp]                                         { $$ = AssignmentGrammarAction($var, $exp); }
          ;

expression: expression[left] ADD expression[right]                                       { $$ = ExpressionGrammarAction($left, $right, NULL,    ADDITION_EXPRESSION); }
          | expression[left] SUB expression[right]                                       { $$ = ExpressionGrammarAction($left, $right, NULL,    SUBTRACTION_EXPRESSION); }
          | expression[left] MUL expression[right]                                       { $$ = ExpressionGrammarAction($left, $right, NULL,    MULTIPLICATION_EXPRESSION); }
          | expression[left] DIV expression[right]                                       { $$ = ExpressionGrammarAction($left, $right, NULL,    DIVISION_EXPRESSION); }
          | expression[left] MOD expression[right]                                       { $$ = ExpressionGrammarAction($left, $right, NULL,    MODULUS_EXPRESSION); }
          | expression[left] AND expression[right]                                       { $$ = ExpressionGrammarAction($left, $right, NULL,    AND_EXPRESSION); }
          | NOT expression[exp]                                                          { $$ = ExpressionGrammarAction($exp,  NULL,   NULL,    NOT_EXPRESSION); }
          | expression[left] OR expression[right]                                        { $$ = ExpressionGrammarAction($left, $right, NULL,    OR_EXPRESSION); }
          | expression[left] EQ expression[right]                                        { $$ = ExpressionGrammarAction($left, $right, NULL,    EQUALS_EXPRESSION); }
          | expression[left] NE expression[right]                                        { $$ = ExpressionGrammarAction($left, $right, NULL,    NOT_EQUALS_EXPRESSION); }
          | expression[left] LT expression[right]                                        { $$ = ExpressionGrammarAction($left, $right, NULL,    LESS_THAN_EXPRESSION); }
          | expression[left] LE expression[right]                                        { $$ = ExpressionGrammarAction($left, $right, NULL,    LEES_EQUAL_EXPRESSION); }
          | expression[left] GT expression[right]                                        { $$ = ExpressionGrammarAction($left, $right, NULL,    GREATER_THAN_EXPRESSION); }
          | expression[left] GE expression[right]                                        { $$ = ExpressionGrammarAction($left, $right, NULL,    GREATER_EQUAL_EXPRESSION); }
          | factor                                                                       { $$ = ExpressionGrammarAction(NULL,  NULL,   $factor, FACTOR_EXPRESSION); }
          ;

range_expression: OPEN_PARENTHESIS expression[left] COMMA expression[right] CLOSE_PARENTHESIS { $$ = RangeExpressionGrammarAction($left, $right); }

factor: OPEN_PARENTHESIS expression[exp] CLOSE_PARENTHESIS                               { $$ = FactorGrammarAction($exp, NULL, NULL, EXPRESSION_FACTOR); }
      | constant[con]                                                                    { $$ = FactorGrammarAction(NULL, $con, NULL, CONSTANT_FACTOR); }
      | VARIABLE[var]                                                                    { $$ = FactorGrammarAction(NULL, NULL, $var, VARIABLE_FACTOR); }
      ;

constant: INTEGER                                                                        { $$ = ConstantGrammarAction($1); }
        ;
%%
