%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

int line_number = 1;
Token* tokens = NULL;
int token_count = 0;
int token_capacity = 100;

void add_token(TokenType type, const char* lexeme, long num_value, int has_num);
char* process_string(const char* str);
long parse_number(const char* str);
%}

%option noyywrap

DIGIT       [0-9]
HEX_DIGIT   [0-9a-fA-F]
LETTER      [a-zA-Z]
ID          ({LETTER}|_)({LETTER}|{DIGIT}|_)*
DECIMAL     {DIGIT}+
HEXADECIMAL 0x{HEX_DIGIT}+
STRING      \"([^\"\\]|\\[\\nt\"])*\"
WHITESPACE  [ \t\r]+
NEWLINE     \n

%%

"//".*              { /* Comentario de línea - ignorar */ }
"/*"([^*]|\*+[^*/])*\*+"/"  { 
    /* Comentario de bloque */
    for (int i = 0; yytext[i]; i++) {
        if (yytext[i] == '\n') line_number++;
    }
}

"if"                { add_token(TK_IF, yytext, 0, 0); }
"else"              { add_token(TK_ELSE, yytext, 0, 0); }
"end"               { add_token(TK_END, yytext, 0, 0); }
"while"             { add_token(TK_WHILE, yytext, 0, 0); }
"loop"              { add_token(TK_LOOP, yytext, 0, 0); }
"fun"               { add_token(TK_FUN, yytext, 0, 0); }
"return"            { add_token(TK_RETURN, yytext, 0, 0); }
"new"               { add_token(TK_NEW, yytext, 0, 0); }
"string"            { add_token(TK_STRING_TYPE, yytext, 0, 0); }
"int"               { add_token(TK_INT_TYPE, yytext, 0, 0); }
"char"              { add_token(TK_CHAR_TYPE, yytext, 0, 0); }
"bool"              { add_token(TK_BOOL_TYPE, yytext, 0, 0); }
"true"              { add_token(TK_TRUE, yytext, 0, 0); }
"false"             { add_token(TK_FALSE, yytext, 0, 0); }
"and"               { add_token(TK_AND, yytext, 0, 0); }
"or"                { add_token(TK_OR, yytext, 0, 0); }
"not"               { add_token(TK_NOT, yytext, 0, 0); }

{ID}                { add_token(TK_ID, yytext, 0, 0); }

{HEXADECIMAL}       { 
    long value = parse_number(yytext);
    add_token(TK_NUMERAL, yytext, value, 1); 
}

{DECIMAL}           { 
    long value = atol(yytext);
    add_token(TK_NUMERAL, yytext, value, 1); 
}

{STRING}            { 
    char* processed = process_string(yytext);
    add_token(TK_LITSTRING, processed, 0, 0);
    free(processed);
}

">="                { add_token(TK_GE, yytext, 0, 0); }
"<="                { add_token(TK_LE, yytext, 0, 0); }
"<>"                { add_token(TK_NE, yytext, 0, 0); }
"="                 { add_token(TK_EQ, yytext, 0, 0); }
">"                 { add_token(TK_GT, yytext, 0, 0); }
"<"                 { add_token(TK_LT, yytext, 0, 0); }
"+"                 { add_token(TK_PLUS, yytext, 0, 0); }
"-"                 { add_token(TK_MINUS, yytext, 0, 0); }
"*"                 { add_token(TK_MULT, yytext, 0, 0); }
"/"                 { add_token(TK_DIV, yytext, 0, 0); }
"("                 { add_token(TK_LPAREN, yytext, 0, 0); }
")"                 { add_token(TK_RPAREN, yytext, 0, 0); }
"["                 { add_token(TK_LBRACKET, yytext, 0, 0); }
"]"                 { add_token(TK_RBRACKET, yytext, 0, 0); }
","                 { add_token(TK_COMMA, yytext, 0, 0); }
":"                 { add_token(TK_COLON, yytext, 0, 0); }

{NEWLINE}           { 
    add_token(TK_NL, "\\n", 0, 0);
    line_number++; 
}

{WHITESPACE}        { /* Ignorar espacios */ }

.                   { 
    add_token(TK_ERROR, yytext, 0, 0);
}

%%

void add_token(TokenType type, const char* lexeme, long num_value, int has_num) {
    if (tokens == NULL) {
        token_capacity = 100;
        tokens = (Token*)malloc(token_capacity * sizeof(Token));
    }
    
    if (token_count >= token_capacity) {
        token_capacity *= 2;
        tokens = (Token*)realloc(tokens, token_capacity * sizeof(Token));
    }
    
    tokens[token_count].type = type;
    tokens[token_count].lexeme = strdup(lexeme);
    tokens[token_count].line = line_number;
    tokens[token_count].has_numeric_value = has_num;
    if (has_num) {
        tokens[token_count].numeric_value = num_value;
    }
    
    token_count++;
}

char* process_string(const char* str) {
    int len = strlen(str);
    char* result = (char*)malloc(len + 1);
    int j = 0;
    
    // Omitir comillas iniciales y finales
    for (int i = 1; i < len - 1; i++) {
        if (str[i] == '\\' && i + 1 < len - 1) {
            switch (str[i + 1]) {
                case 'n': result[j++] = '\n'; i++; break;
                case 't': result[j++] = '\t'; i++; break;
                case '\\': result[j++] = '\\'; i++; break;
                case '"': result[j++] = '"'; i++; break;
                default: result[j++] = str[i]; break;
            }
        } else {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

long parse_number(const char* str) {
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        return strtol(str, NULL, 16);
    }
    return atol(str);
}