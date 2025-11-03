#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    // Palabras reservadas
    TK_IF,
    TK_ELSE,
    TK_END,
    TK_WHILE,
    TK_LOOP,
    TK_FUN,
    TK_RETURN,
    TK_NEW,
    TK_STRING_TYPE,
    TK_INT_TYPE,
    TK_CHAR_TYPE,
    TK_BOOL_TYPE,
    TK_TRUE,
    TK_FALSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    
    // Identificadores y literales
    TK_ID,
    TK_NUMERAL,
    TK_LITSTRING,
    
    // Operadores
    TK_PLUS,        // +
    TK_MINUS,       // -
    TK_MULT,        // *
    TK_DIV,         // /
    TK_GT,          // >
    TK_LT,          // <
    TK_GE,          // >=
    TK_LE,          // <=
    TK_EQ,          // =
    TK_NE,          // <>
    
    // Delimitadores
    TK_LPAREN,      // (
    TK_RPAREN,      // )
    TK_LBRACKET,    // [
    TK_RBRACKET,    // ]
    TK_COMMA,       // ,
    TK_COLON,       // :
    
    // Especiales
    TK_NL,          // Salto de línea
    TK_ERROR        // Token de error
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;
    int line;
    int has_numeric_value;
    long numeric_value;
} Token;

const char* token_type_to_string(TokenType type);
void print_tokens(Token* tokens, int count);

#endif