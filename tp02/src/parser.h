#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include <stdbool.h>

// Estructura del parser
typedef struct {
    Token* tokens;
    int token_count;
    int current;
    bool has_error;
    int error_count;
} Parser;

// Funciones principales
Parser* parser_create(Token* tokens, int count);
void parser_destroy(Parser* p);
bool parse(Parser* p);

// Funciones de parsing (NO TERMINALES)
bool parse_program(Parser* p);
bool parse_function_list(Parser* p);
bool parse_function(Parser* p);
bool parse_param_list(Parser* p);
bool parse_param_list_prime(Parser* p);
bool parse_param(Parser* p);
bool parse_type(Parser* p);
bool parse_array_type(Parser* p);
bool parse_stmt_list(Parser* p);
bool parse_stmt_list_prime(Parser* p);
bool parse_statement(Parser* p);
bool parse_declaration(Parser* p);
bool parse_assignment(Parser* p);
bool parse_if_stmt(Parser* p);
bool parse_while_stmt(Parser* p);
bool parse_return_stmt(Parser* p);
bool parse_expression(Parser* p);
bool parse_expr_prime(Parser* p);
bool parse_and_expr(Parser* p);
bool parse_and_expr_prime(Parser* p);
bool parse_rel_expr(Parser* p);
bool parse_rel_expr_prime(Parser* p);
bool parse_rel_op(Parser* p);
bool parse_add_expr(Parser* p);
bool parse_add_expr_prime(Parser* p);
bool parse_term(Parser* p);
bool parse_term_prime(Parser* p);
bool parse_factor(Parser* p);
bool parse_primary(Parser* p);
bool parse_call_or_array(Parser* p);
bool parse_arg_list(Parser* p);
bool parse_arg_list_prime(Parser* p);

// Funciones auxiliares
Token* current_token(Parser* p);
Token* peek_token(Parser* p, int offset);
bool match(Parser* p, TokenType type);
bool consume(Parser* p, TokenType type, const char* error_msg);
void parser_error(Parser* p, const char* message);
void skip_to_sync(Parser* p);
bool is_at_end(Parser* p);

#endif