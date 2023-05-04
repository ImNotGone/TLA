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
digit			[0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]
declarationName    [A-Za-z][A-Za-z0-9]*

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

int                                         {yylval.token = INT_TYPE; return INT_TYPE;}

RBT                                         {yylval.token = RED_BLACK_TREE_TYPE; return RED_BLACK_TREE_TYPE;}
AVL                                         {yylval.token = AVL_TREE_TYPE; return AVL_TREE_TYPE;}
BST                                         {yylval.token = BST_TREE_TYPE; return BST_TREE_TYPE;}

new                                         {yylval.token = NEW_TREE; return NEW_TREE;}
print                                       {yylval.token = PRINT; return PRINT;}
insert                                      {yylval.token = INSERT; return INSERT;}
remove                                      {yylval.token = REMOVE; return REMOVE;}
inorder                                     {yylval.token = INORDER; return INORDER;}
postorder                                   {yylval.token = POSTORDER; return POSTORDER;}
preorder                                    {yylval.token = PREORDER; return PREORDER;}
reduce                                      {yylval.token = REDUCE; return REDUCE;}
find                                        {yylval.token = FIND; return FIND;}
match                                       {yylval.token = MATCH; return MATCH;}
"<-"                                        { return ADD_TREE; }


"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

{declarationName}								{ return DeclarePatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
