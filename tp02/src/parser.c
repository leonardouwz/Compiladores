#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// FUNCIONES AUXILIARES
// ============================================================================

Parser* parser_create(Token* tokens, int count) {
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->tokens = tokens;
    p->token_count = count;
    p->current = 0;
    p->has_error = false;
    p->error_count = 0;
    return p;
}

void parser_destroy(Parser* p) {
    free(p);
}

Token* current_token(Parser* p) {
    if (p->current >= p->token_count) {
        return NULL;
    }
    // Saltar tokens NL (saltos de línea)
    while (p->current < p->token_count && p->tokens[p->current].type == TK_NL) {
        p->current++;
    }
    if (p->current >= p->token_count) {
        return NULL;
    }
    return &p->tokens[p->current];
}

Token* peek_token(Parser* p, int offset) {
    int pos = p->current + offset;
    while (pos < p->token_count && p->tokens[pos].type == TK_NL) {
        pos++;
    }
    if (pos >= p->token_count) {
        return NULL;
    }
    return &p->tokens[pos];
}

bool match(Parser* p, TokenType type) {
    Token* tok = current_token(p);
    return tok != NULL && tok->type == type;
}

bool consume(Parser* p, TokenType type, const char* error_msg) {
    Token* tok = current_token(p);
    if (tok == NULL) {
        fprintf(stderr, "Error: Fin inesperado del archivo. %s\n", error_msg);
        p->has_error = true;
        p->error_count++;
        return false;
    }
    if (tok->type != type) {
        fprintf(stderr, "Error sintáctico en línea %d: %s. Se encontró '%s' (%s)\n", 
                tok->line, error_msg, tok->lexeme, token_type_to_string(tok->type));
        p->has_error = true;
        p->error_count++;
        return false;
    }
    p->current++;
    return true;
}

void parser_error(Parser* p, const char* message) {
    Token* tok = current_token(p);
    if (tok != NULL) {
        fprintf(stderr, "Error sintáctico en línea %d: %s (token: '%s')\n", 
                tok->line, message, tok->lexeme);
    } else {
        fprintf(stderr, "Error sintáctico: %s (fin de archivo)\n", message);
    }
    p->has_error = true;
    p->error_count++;
}

void skip_to_sync(Parser* p) {
    // Sincronización: buscar próximo 'end', 'fun', etc.
    while (!is_at_end(p)) {
        Token* tok = current_token(p);
        if (tok->type == TK_END || tok->type == TK_FUN) {
            return;
        }
        p->current++;
    }
}

bool is_at_end(Parser* p) {
    return current_token(p) == NULL;
}

// ============================================================================
// FUNCIÓN PRINCIPAL DE PARSING
// ============================================================================

bool parse(Parser* p) {
    bool result = parse_program(p);
    
    if (!is_at_end(p)) {
        parser_error(p, "Se esperaba fin de archivo");
        return false;
    }
    
    return result && !p->has_error;
}

// ============================================================================
// REGLAS DE LA GRAMÁTICA (NO TERMINALES)
// ============================================================================

// Program -> FunctionList
bool parse_program(Parser* p) {
    return parse_function_list(p);
}

// FunctionList -> Function FunctionList | Function
bool parse_function_list(Parser* p) {
    if (!parse_function(p)) {
        return false;
    }
    
    // Mientras haya más funciones
    while (match(p, TK_FUN)) {
        if (!parse_function(p)) {
            return false;
        }
    }
    
    return true;
}

// Function -> 'fun' ID '(' ParamList ')' ':' Type StmtList 'end'
bool parse_function(Parser* p) {
    if (!consume(p, TK_FUN, "Se esperaba 'fun'")) return false;
    if (!consume(p, TK_ID, "Se esperaba nombre de función")) return false;
    if (!consume(p, TK_LPAREN, "Se esperaba '('")) return false;
    
    // ParamList puede ser epsilon
    if (!match(p, TK_RPAREN)) {
        if (!parse_param_list(p)) return false;
    }
    
    if (!consume(p, TK_RPAREN, "Se esperaba ')'")) return false;
    if (!consume(p, TK_COLON, "Se esperaba ':'")) return false;
    if (!parse_type(p)) return false;
    if (!parse_stmt_list(p)) return false;
    if (!consume(p, TK_END, "Se esperaba 'end'")) return false;
    
    return true;
}

// ParamList -> Param ParamListPrime
bool parse_param_list(Parser* p) {
    if (!parse_param(p)) return false;
    return parse_param_list_prime(p);
}

// ParamListPrime -> ',' Param ParamListPrime | epsilon
bool parse_param_list_prime(Parser* p) {
    if (match(p, TK_COMMA)) {
        p->current++;
        if (!parse_param(p)) return false;
        return parse_param_list_prime(p);
    }
    return true; // epsilon
}

// Param -> ID ':' Type
bool parse_param(Parser* p) {
    if (!consume(p, TK_ID, "Se esperaba nombre de parámetro")) return false;
    if (!consume(p, TK_COLON, "Se esperaba ':'")) return false;
    return parse_type(p);
}

// Type -> ArrayType | 'int' | 'bool' | 'string' | 'char'
bool parse_type(Parser* p) {
    if (match(p, TK_LBRACKET)) {
        return parse_array_type(p);
    } else if (match(p, TK_INT_TYPE) || match(p, TK_BOOL_TYPE) || 
               match(p, TK_STRING_TYPE) || match(p, TK_CHAR_TYPE)) {
        p->current++;
        return true;
    } else {
        parser_error(p, "Se esperaba un tipo");
        return false;
    }
}

// ArrayType -> '[]' Type
bool parse_array_type(Parser* p) {
    if (!consume(p, TK_LBRACKET, "Se esperaba '['")) return false;
    if (!consume(p, TK_RBRACKET, "Se esperaba ']'")) return false;
    return parse_type(p);
}

// StmtList -> Statement StmtListPrime
bool parse_stmt_list(Parser* p) {
    if (!parse_statement(p)) return false;
    return parse_stmt_list_prime(p);
}

// StmtListPrime -> Statement StmtListPrime | epsilon
bool parse_stmt_list_prime(Parser* p) {
    Token* tok = current_token(p);
    if (tok == NULL) return true; // epsilon
    
    // Verificar si es inicio de statement
    if (tok->type == TK_ID || tok->type == TK_IF || tok->type == TK_WHILE || 
        tok->type == TK_RETURN) {
        if (!parse_statement(p)) return false;
        return parse_stmt_list_prime(p);
    }
    
    return true; // epsilon
}

// Statement -> Declaration | Assignment | IfStmt | WhileStmt | ReturnStmt
bool parse_statement(Parser* p) {
    Token* tok = current_token(p);
    Token* next = peek_token(p, 1);
    
    if (tok == NULL) {
        parser_error(p, "Se esperaba una declaración");
        return false;
    }
    
    // Distinguir entre declaración y asignación
    if (tok->type == TK_ID && next != NULL && next->type == TK_COLON) {
        return parse_declaration(p);
    } else if (tok->type == TK_ID) {
        return parse_assignment(p);
    } else if (tok->type == TK_IF) {
        return parse_if_stmt(p);
    } else if (tok->type == TK_WHILE) {
        return parse_while_stmt(p);
    } else if (tok->type == TK_RETURN) {
        return parse_return_stmt(p);
    } else {
        parser_error(p, "Se esperaba una declaración válida");
        return false;
    }
}

// Declaration -> ID ':' Type
bool parse_declaration(Parser* p) {
    if (!consume(p, TK_ID, "Se esperaba identificador")) return false;
    if (!consume(p, TK_COLON, "Se esperaba ':'")) return false;
    return parse_type(p);
}

// Assignment -> ID CallOrArray '=' Expression
bool parse_assignment(Parser* p) {
    if (!consume(p, TK_ID, "Se esperaba identificador")) return false;
    if (!parse_call_or_array(p)) return false;
    if (!consume(p, TK_EQ, "Se esperaba '='")) return false;
    return parse_expression(p);
}

// CallOrArray -> '(' ArgList ')' | '[' Expression ']' | epsilon
bool parse_call_or_array(Parser* p) {
    if (match(p, TK_LPAREN)) {
        p->current++;
        if (!match(p, TK_RPAREN)) {
            if (!parse_arg_list(p)) return false;
        }
        if (!consume(p, TK_RPAREN, "Se esperaba ')'")) return false;
    } else if (match(p, TK_LBRACKET)) {
        p->current++;
        if (!parse_expression(p)) return false;
        if (!consume(p, TK_RBRACKET, "Se esperaba ']'")) return false;
    }
    return true; // epsilon
}

// IfStmt -> 'if' Expression StmtList ElsePart 'end'
bool parse_if_stmt(Parser* p) {
    if (!consume(p, TK_IF, "Se esperaba 'if'")) return false;
    if (!parse_expression(p)) return false;
    if (!parse_stmt_list(p)) return false;
    
    // ElsePart es opcional
    if (match(p, TK_ELSE)) {
        p->current++;
        if (!parse_stmt_list(p)) return false;
    }
    
    if (!consume(p, TK_END, "Se esperaba 'end'")) return false;
    return true;
}

// WhileStmt -> 'while' Expression StmtList 'loop'
bool parse_while_stmt(Parser* p) {
    if (!consume(p, TK_WHILE, "Se esperaba 'while'")) return false;
    if (!parse_expression(p)) return false;
    if (!parse_stmt_list(p)) return false;
    if (!consume(p, TK_LOOP, "Se esperaba 'loop'")) return false;
    return true;
}

// ReturnStmt -> 'return' Expression
bool parse_return_stmt(Parser* p) {
    if (!consume(p, TK_RETURN, "Se esperaba 'return'")) return false;
    return parse_expression(p);
}

// Expression -> AndExpr ExprPrime
bool parse_expression(Parser* p) {
    if (!parse_and_expr(p)) return false;
    return parse_expr_prime(p);
}

// ExprPrime -> 'or' AndExpr ExprPrime | epsilon
bool parse_expr_prime(Parser* p) {
    if (match(p, TK_OR)) {
        p->current++;
        if (!parse_and_expr(p)) return false;
        return parse_expr_prime(p);
    }
    return true; // epsilon
}

// AndExpr -> RelExpr AndExprPrime
bool parse_and_expr(Parser* p) {
    if (!parse_rel_expr(p)) return false;
    return parse_and_expr_prime(p);
}

// AndExprPrime -> 'and' RelExpr AndExprPrime | epsilon
bool parse_and_expr_prime(Parser* p) {
    if (match(p, TK_AND)) {
        p->current++;
        if (!parse_rel_expr(p)) return false;
        return parse_and_expr_prime(p);
    }
    return true; // epsilon
}

// RelExpr -> AddExpr RelExprPrime
bool parse_rel_expr(Parser* p) {
    if (!parse_add_expr(p)) return false;
    return parse_rel_expr_prime(p);
}

// RelExprPrime -> RelOp AddExpr | epsilon
bool parse_rel_expr_prime(Parser* p) {
    if (match(p, TK_LT) || match(p, TK_GT) || match(p, TK_LE) || 
        match(p, TK_GE) || match(p, TK_EQ) || match(p, TK_NE)) {
        p->current++;
        if (!parse_add_expr(p)) return false;
    }
    return true; // epsilon
}

// AddExpr -> Term AddExprPrime
bool parse_add_expr(Parser* p) {
    if (!parse_term(p)) return false;
    return parse_add_expr_prime(p);
}

// AddExprPrime -> ('+' | '-') Term AddExprPrime | epsilon
bool parse_add_expr_prime(Parser* p) {
    if (match(p, TK_PLUS) || match(p, TK_MINUS)) {
        p->current++;
        if (!parse_term(p)) return false;
        return parse_add_expr_prime(p);
    }
    return true; // epsilon
}

// Term -> Factor TermPrime
bool parse_term(Parser* p) {
    if (!parse_factor(p)) return false;
    return parse_term_prime(p);
}

// TermPrime -> ('*' | '/') Factor TermPrime | epsilon
bool parse_term_prime(Parser* p) {
    if (match(p, TK_MULT) || match(p, TK_DIV)) {
        p->current++;
        if (!parse_factor(p)) return false;
        return parse_term_prime(p);
    }
    return true; // epsilon
}

// Factor -> 'not' Factor | '-' Factor | Primary
bool parse_factor(Parser* p) {
    if (match(p, TK_NOT)) {
        p->current++;
        return parse_factor(p);
    } else if (match(p, TK_MINUS)) {
        p->current++;
        return parse_factor(p);
    } else {
        return parse_primary(p);
    }
}

// Primary -> ID CallOrArray | NUMERAL | LITSTRING | 'true' | 'false' 
//          | '(' Expression ')' | 'new' '[' Expression ']' Type
bool parse_primary(Parser* p) {
    Token* tok = current_token(p);
    
    if (tok == NULL) {
        parser_error(p, "Se esperaba una expresión");
        return false;
    }
    
    if (tok->type == TK_ID) {
        p->current++;
        return parse_call_or_array(p);
    } else if (tok->type == TK_NUMERAL || tok->type == TK_LITSTRING ||
               tok->type == TK_TRUE || tok->type == TK_FALSE) {
        p->current++;
        return true;
    } else if (tok->type == TK_LPAREN) {
        p->current++;
        if (!parse_expression(p)) return false;
        return consume(p, TK_RPAREN, "Se esperaba ')'");
    } else if (tok->type == TK_NEW) {
        p->current++;
        if (!consume(p, TK_LBRACKET, "Se esperaba '['")) return false;
        if (!parse_expression(p)) return false;
        if (!consume(p, TK_RBRACKET, "Se esperaba ']'")) return false;
        return parse_type(p);
    } else {
        parser_error(p, "Se esperaba un valor primario");
        return false;
    }
}

// ArgList -> Expression ArgListPrime
bool parse_arg_list(Parser* p) {
    if (!parse_expression(p)) return false;
    return parse_arg_list_prime(p);
}

// ArgListPrime -> ',' Expression ArgListPrime | epsilon
bool parse_arg_list_prime(Parser* p) {
    if (match(p, TK_COMMA)) {
        p->current++;
        if (!parse_expression(p)) return false;
        return parse_arg_list_prime(p);
    }
    return true; // epsilon
}