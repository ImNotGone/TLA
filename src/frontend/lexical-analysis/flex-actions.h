#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

// Aca se definen lso IDs de cada token para el scanner Flex que luego se le brindará su funcionalidad con bison
typedef enum TokenID{
    UNKNOWN= 0, //Este valor hace fallar al analizador sintactico
    /* Código de error de Bison, que permite abortar el escaneo de lexemas cuando
     se presente un patrón desconocido. El número "257" coincide con el valor
     que Bison le otorga por defecto, pero además permite que el resto de
     tokens continúen desde el valor "258" lo que permite proteger los IDs
     internos que Bison reserva para crear "tokens literales":*/
//    YYUNDEF = 257,

    DECLARATION,

    // Operadores aritméticos.
//    ADD,
//    SUB,
//    MUL,
//    DIV,
    MOD,

    //Operadores logicos
    AND,
    OR,
    NOT,

    //Operadores de comparacion
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE,

    //Asignación de variables
    ASSIGN,

    //Simbolos de codigo
    COMA,
//    OPEN_PARENTHESIS,
//    CLOSE_PARENTHESIS,
    OPEN_CURL_BRACKETS,
    CLOSE_CURL_BRACKETS,
    SEMI_COLON,

    //Bloques de control
    FOR,
    IN,
    WHILE,
    IF,
    ELSE,

    //Tipos de datos y arboles
    INT_TYPE,
    RED_BLACK_TREE_TYPE,
    AVL_TREE_TYPE,
    BST_TREE_TYPE,

    //funciones
    NEW_TREE,
    PRINT,
    INSERT,
    REMOVE,
    INORDER,
    POSTORDER,
    PREORDER,
    REDUCE,
    FIND,
    MATCH,
    ADD_TREE,
} TokenID;

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */


//Declaracion de nombres de variablez
 token DeclarePatternAction(const char* lexeme, const int length);

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
void EndCommentPatternAction();

// Patrones terminales del lenguaje diseñado.
token AdditionOperatorPatternAction(const char * lexeme);
token CloseParenthesisPatternAction(const char * lexeme);
token DivisionOperatorPatternAction(const char * lexeme);
token IntegerPatternAction(const char * lexeme, const int length);
token MultiplicationOperatorPatternAction(const char * lexeme);
token OpenParenthesisPatternAction(const char * lexeme);
token SubtractionOperatorPatternAction(const char * lexeme);

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

#endif
