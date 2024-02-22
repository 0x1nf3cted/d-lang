#ifndef SYNTAX_H
#define SYNTAX_H

#include "parser.h"

bool check_variable_syntax_error(Token **var_tokens, int buffer_size);

bool check_value_syntax_error(Token **var_tokens, int buffer_size);

void get_var_info(struct Node *tree, Token **var_tokens, int buffer_size, struct symbol_tab *tab);

struct Node *get_variable_ref(struct symbol_tab *tab, char *Identifier);


#endif // SYNTAX_H

// dependency error