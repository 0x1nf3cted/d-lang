#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// Define the Node struct for parsing variables

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

typedef enum
{
    ROOT,
    VARIABLE_NODE,
    VARIABLE_ASSIGN_NODE,
    POINTER_ASSIGN_NODE,
    POINTER_NODE,
    INITIALIZE_POINTER_NODE,
    INITIALIZE_VARIABLE_NODE,
} NodeType;

typedef struct Node
{
    union
    {
        struct Node *variable;
        struct Node *pointer;
    } data;
    char *value;
    NodeType type;
    struct Node *children;
    size_t branch_count;
    u_int32_t line;
    u_int32_t start_position;
    u_int32_t end_position;

} Node;

void malloc_error(Node *p);
Node *parse(Token **tokens, int token_number, Node *ast);
#endif // PARSER_H