#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
 
// Define the Node struct for parsing variables


typedef struct {
    char *msg;
    uint32_t line;
    uint32_t position;
} parser_error_t;


typedef struct Node
{
    enum
    {
        INITIALIZED_VAR_NODE,   // set a: u8 = 10;
        UNINITIALIZED_VAR_NODE, // set a: u8;
        POINTER_ASSIGN_NODE,    // x = &a;
        POINTER_NODE,           // set y: *u8;
        DEREF_POINTER_NODE,     // b = *x;
        VARIABLE_TYPE_NODE,     // "char" | "u8" | "u16" | "u32" | "u64" | "f32" | "f64" | "str"
        IDENTIFIER_NODE,        // ex: a, count, ...
        VALUE_NODE,             // "Value"
        NUMBER_NODE,            // [0-9] number | [0-9]
        WORD_NODE,              //  [a-zA-Z_0-9] word_part | [a-zA-Z_0-9]
        KEYWORD_NODE,
    } type;

    union
    {
        const char *keyword;
        struct
        {
            struct Node *identifier;
            struct Node *variable_type;
            struct Node *value;
        } initialized_var_node;

        struct
        {
            struct Node *identifier;
            struct Node *variable_type;
        } uninitialized_var_node;

        struct
        {
            struct Node *identifier;
            struct Node *variable;
        } pointer_assign_node;

        struct
        {
            struct Node *identifier;
            struct Node *variable_type;
        } pointer_node;

        struct
        {
            struct Node *variable;
            struct Node *variable_type;
        } deref_pointer_node;

        char *variable_type_node;

        char *identifier_node;

        struct
        {
            struct Node *variable;
            struct Node *pointer;
            struct Node *word;
            struct Node *number;
        } value_node;

        struct
        {
            struct Node *number;
            struct Node *number;
        } number_node;

        char *word_node;
    } data;
    u_int32_t line;
    u_int32_t position;
} Node;



extern Node* parse();


#endif //PARSER_H