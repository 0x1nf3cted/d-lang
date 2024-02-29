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
    SYSTEM_CALL_NODE,
    FUNCTION_CALL_ARGUMENT_NODE,
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

typedef enum
{
    GLOBAL_SCOPE,
    FUNCTION_SCOPE,
    FOR_LOOP_SCOPE,
    LOOP_SCOPE
} Scope;

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
    int number_args;
    struct Node **args;

} FunctionCallNode;

typedef struct
{
    char *identifier;
    struct Node *referenced_variable;

} PointerNode;
typedef struct Symbol
{
    char *identifier;
    union
    {
        struct Node *variable;
        struct Node *pointer;
    } data;

    NodeType type;
    Scope scope;
    // You can add more fields as needed
} Symbol;
typedef struct Node
{
    char *label;
    union
    {
        VariableNode variable;
        ValueNode value;
        PointerNode pointer;
        FunctionCallNode function_call;

    } data;
    NodeType type;
    struct Node **children;
    size_t branch_count;
    u_int32_t line;
    u_int32_t start_position;
    u_int32_t end_position;
    Symbol *table;
    size_t nb_var;

} Node;

void print_ast(Node *ast, int level);
void malloc_error(Node *p);
Node *parse(Token **tokens, int token_number, Node *ast, int *cursor);
void print_buffer(int buffer_s, Token **buffer);
Node *parse_system_call(Token **tokens, int token_number, Token **var_tokens, int buffer_size, Node *ast, int *cursor);
ValueNode *parse_function_arg(Token *token);
Node *parse_value(Token **var_tokens, int buffer_size, Node *ast);
#endif // PARSER_H