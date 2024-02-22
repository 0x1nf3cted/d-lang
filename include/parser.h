#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "syntax.h"

// Define the Node struct for parsing variables

typedef struct
{
    char *msg;
    char *severity;
    uint32_t line;
    uint32_t position;
    uint32_t error_nb;
} parser_error_t;

typedef struct
{

} TraceBack;

typedef enum
{
    GLOBAL_CONTEXT,
    FOR_CONTEXT,
    FUNCTION_CONTEXT
} Context;

typedef struct
{
    const char *identifier;
    TokenType variable_type;
    struct Node *parent;
    struct Node *value;
    Context var_context;
} variable_node;

typedef struct
{
    const char *identifier;
    TokenType variable_type;
    struct Node *referenced_variable;
    unsigned long address;
    Context ptr_context;

} pointer_node;

struct symbol_tab
{
    struct Node *variable;
    struct Node *pointer;
    size_t sym_count;
};

typedef struct Node
{
    union
    {
        variable_node *variable;
        pointer_node *pointer;
    } data;
    struct Node* children;
    size_t branch_count;
    u_int32_t line;
    u_int32_t start_position;
    u_int32_t end_position;

} Node;

extern Node *parse(Token **tokens, int tokenNumber);

#endif // PARSER_H