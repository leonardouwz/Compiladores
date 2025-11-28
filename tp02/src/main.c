#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"

// Declaraciones externas del lexer
extern FILE* yyin;
extern int yylex(void);
extern Token* tokens;
extern int token_count;
extern int line_number;

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_mini0>\n", argv[0]);
        return 1;
    }
    
    // Abrir archivo
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: No se puede abrir el archivo '%s'\n", argv[1]);
        return 1;
    }
    
    // Análisis léxico
    yyin = file;
    yylex();
    fclose(file);
    
    // Verificar si hubo errores léxicos
    bool lexical_error = false;
    for (int i = 0; i < token_count; i++) {
        if (tokens[i].type == TK_ERROR) {
            fprintf(stderr, "Error léxico en línea %d: caracter inválido '%s'\n", 
                    tokens[i].line, tokens[i].lexeme);
            lexical_error = true;
        }
    }
    
    if (lexical_error) {
        // Liberar memoria
        for (int i = 0; i < token_count; i++) {
            free(tokens[i].lexeme);
        }
        free(tokens);
        return 1;
    }
    
    // Análisis sintáctico
    Parser* parser = parser_create(tokens, token_count);
    bool success = parse(parser);
    
    if (success) {
        printf("✓ Análisis sintáctico exitoso\n");
        printf("Programa válido en Mini-0\n");
    } else {
        fprintf(stderr, "\n✗ Análisis sintáctico fallido\n");
        fprintf(stderr, "Total de errores sintácticos: %d\n", parser->error_count);
    }
    
    // Liberar memoria
    parser_destroy(parser);
    for (int i = 0; i < token_count; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);
    
    return success ? 0 : 1;
}