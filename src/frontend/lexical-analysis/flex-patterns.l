%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
digit			    [0-9]
boolean			    (true|false)
endline			    [\n]
whitespace		    [ \f\n\r\t\v]
declarationName     [A-Za-z][A-Za-z0-9]*

%%

"+"                                         { return ADD;}
"-"                                         { return SUB;}
"*"                                         { return MUL;}
"/"                                         { return DIV;}
"%"                                         { return MOD;}

"&"                                         { return AND;}
"|"                                         { return OR;}
"!"                                         { return NOT;}

"=="                                        { return EQ;}
"!="                                        { return NE;}
"<"                                         { return LT;}
"<="                                        { return LE;}
">"                                         { return GT;}
">="                                        { return GE;}

"="                                         { return ASSIGN;}
","                                         { return COMMA;}
"("                                         { return OPEN_PARENTHESIS;}
")"                                         { return CLOSE_PARENTHESIS;}
"{"                                         { return OPEN_CURL_BRACKETS;}
"}"                                         { return CLOSE_CURL_BRACKETS;}
";"                                         { return SEMI_COLON;}

"for"                                       { return FOR;}
"in"                                        { return IN;}
"while"                                     { return WHILE;}
"if"                                        { return IF;}
"else"                                      { return ELSE;}

"int"                                       {yylval.token = INT; return INT;}
"bool"                                      {yylval.token = BOOL; return BOOL;}

"RBT"                                       {yylval.token = RBT; return RBT;}
"AVL"                                       {yylval.token = AVL; return AVL;}
"BST"                                       {yylval.token = BST; return BST;}

"new"                                       {yylval.token = NEW_TREE; return NEW_TREE;}
"print"                                     {yylval.token = PRINT; return PRINT;}
"max"                                       {yylval.token = MAX; return MAX;}
"min"                                       {yylval.token = MIN; return MIN;}
"root"                                      {yylval.token = ROOT; return ROOT;}
"height"                                    {yylval.token = HEIGHT; return HEIGHT;}
"insert"                                    {yylval.token = INSERT; return INSERT;}
"remove"                                    {yylval.token = REMOVE; return REMOVE;}
"inorder"                                   {yylval.token = INORDER; return INORDER;}
"postorder"                                 {yylval.token = POSTORDER; return POSTORDER;}
"preorder"                                  {yylval.token = PREORDER; return PREORDER;}
"reduce"                                    {yylval.token = REDUCE; return REDUCE;}
"find"                                      {yylval.token = FIND; return FIND;}
"present"                                   {yylval.token = PRESENT; return PRESENT;}
"<-"                                        { return ADD_TREE; }

"//".*{endline}                             { ; }
"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }
{boolean}									{ return BooleanPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

{declarationName}							{ return DeclarePatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
