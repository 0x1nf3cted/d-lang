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
    NUMBER_VALUE,
    VARIABLE_NODE,
    VARIABLE_ASSIGN_NODE,
    POINTER_ASSIGN_NODE,
    POINTER_NODE,
    INITIALIZE_POINTER_NODE,
    INITIALIZE_VARIABLE_NODE,
} NodeType;

typedef enum
{
    UnsignedInteger_8,
    UnsignedInteger_16,
    UnsignedInteger_32,
    UnsignedInteger_64,
    UnsignedInteger_128,
    SignedInteger_8,
    SignedInteger_16,
    SignedInteger_32,
    SignedInteger_64,
    SignedInteger_128,
    Char_type,
    String_type,
    Boolean_type,
} ValueType;

typedef struct
{
    ValueType value_type;
    char *identifier;

} VariableNode;

typedef struct
{
    ValueType value_type;
    char *value;
} ValueNode;
typedef struct
{
    char *identifier;
    struct Node *referenced_variable;

} PointerNode;

typedef struct Node
{
    char *label;
    union
    {
        VariableNode variable;
        ValueNode value;
        PointerNode pointer;
    } data;
    NodeType type;
    struct Node **children;
    size_t branch_count;
    u_int32_t line;
    u_int32_t start_position;
    u_int32_t end_position;

} Node;

void malloc_error(Node *p);
Node *parse(Token **tokens, int token_number, Node *ast, int *cursor);
void print_buffer(int buffer_s, Token **buffer);
#endif // PARSER_H