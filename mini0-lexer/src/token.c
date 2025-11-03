#include <stdio.h>
#include "token.h"

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TK_IF: return "IF";
        case TK_ELSE: return "ELSE";
        case TK_END: return "END";
        case TK_WHILE: return "WHILE";
        case TK_LOOP: return "LOOP";
        case TK_FUN: return "FUN";
        case TK_RETURN: return "RETURN";
        case TK_NEW: return "NEW";
        case TK_STRING_TYPE: return "STRING_TYPE";
        case TK_INT_TYPE: return "INT_TYPE";
        case TK_CHAR_TYPE: return "CHAR_TYPE";
        case TK_BOOL_TYPE: return "BOOL_TYPE";
        case TK_TRUE: return "TRUE";
        case TK_FALSE: return "FALSE";
        case TK_AND: return "AND";
        case TK_OR: return "OR";
        case TK_NOT: return "NOT";
        case TK_ID: return "ID";
        case TK_NUMERAL: return "NUMERAL";
        case TK_LITSTRING: return "LITSTRING";
        case TK_PLUS: return "PLUS";
        case TK_MINUS: return "MINUS";
        case TK_MULT: return "MULT";
        case TK_DIV: return "DIV";
        case TK_GT: return "GT";
        case TK_LT: return "LT";
        case TK_GE: return "GE";
        case TK_LE: return "LE";
        case TK_EQ: return "EQ";
        case TK_NE: return "NE";
        case TK_LPAREN: return "LPAREN";
        case TK_RPAREN: return "RPAREN";
        case TK_LBRACKET: return "LBRACKET";
        case TK_RBRACKET: return "RBRACKET";
        case TK_COMMA: return "COMMA";
        case TK_COLON: return "COLON";
        case TK_NL: return "NL";
        case TK_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void print_tokens(Token* tokens, int count) {
    printf("========================================\n");
    printf("ANÁLISIS LÉXICO - Mini-0\n");
    printf("========================================\n");
    printf("Total de tokens: %d\n\n", count);
    
    printf("%-6s %-15s %-20s %-15s\n", "LÍNEA", "TIPO", "LEXEMA", "VALOR");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        Token t = tokens[i];
        printf("%-6d %-15s %-20s", t.line, token_type_to_string(t.type), t.lexeme);
        
        if (t.has_numeric_value) {
            printf("%-15ld", t.numeric_value);
        } else if (t.type == TK_LITSTRING) {
            printf("(string procesado)");
        }
        
        printf("\n");
    }
    
    printf("========================================\n");
}