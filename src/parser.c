#include "parser.h"

#define u32 uint32_t

void malloc_error(Node *p)
{
    if (p == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory.\n");
        abort();
    }
}

// TODO: check the type of the value at variable init

// should check if the referenced variable is valid and does exist

ValueType get_value_type(Token *type)
{
    if (strcmp(type->value, "u8") == 0)
    {
        return UnsignedInteger_8;
    }
    else if (strcmp(type->value, "u16") == 0)
    {
        return UnsignedInteger_16;
    }
    else if (strcmp(type->value, "u32") == 0)
    {
        return UnsignedInteger_32;
    }
    else if (strcmp(type->value, "u64") == 0)
    {
        return UnsignedInteger_64;
    }
    else if (strcmp(type->value, "u128") == 0)
    {
        return UnsignedInteger_128;
    }
    else if (strcmp(type->value, "i8") == 0)
    {
        return SignedInteger_8;
    }
    else if (strcmp(type->value, "i16") == 0)
    {
        return SignedInteger_16;
    }
    else if (strcmp(type->value, "i32") == 0)
    {
        return SignedInteger_32;
    }
    else if (strcmp(type->value, "i64") == 0)
    {
        return SignedInteger_64;
    }
    else if (strcmp(type->value, "i128") == 0)
    {
        return SignedInteger_128;
    }
    else if (strcmp(type->value, "char") == 0)
    {
        return Char_type;
    }
    else if (strcmp(type->value, "str") == 0)
    {
        return String_type;
    }
    else if (strcmp(type->value, "bool") == 0)
    {
        return Boolean_type;
    }
}

Node *symbol_table_lookup(Node *ast, char *identifier, NodeType type)
{
    for (int i = 0; i < ast->nb_var; i++)
    {
        if (type == POINTER_NODE)
        {
            if (strcmp(ast->table[i].data.pointer->data.pointer.identifier, identifier) == 0)
            {
                return ast->table[i].data.pointer;
            }
        }
        else if (type == VARIABLE_NODE)
        {
            if (strcmp(ast->table[i].data.variable->data.variable.identifier, identifier) == 0)
            {
                return ast->table[i].data.variable;
            }
        }
    }
    return NULL;
}
// Node *parse_function_arg(Token *token)
// {
//     if (token->type == Identifier)
//     {

//     }
//     else
//     {
//         Node *arg = (Node *)malloc(sizeof(ValueNode));
//         if (arg == NULL)
//         {
//             fprintf(stderr, "Fatal: failed to allocate memory.\n");
//             abort();
//         }
//         arg->value = strdup(token->value);
//         if (arg->value == NULL)
//         {
//             fprintf(stderr, "Fatal: failed to allocate memory for identifier.\n");
//             free(arg);
//             abort();
//         }
//         arg->value_type = get_value_type(token);
//         return arg;
//     }
// }

Node *parse_system_call(Token **tokens, int token_number, Token **var_tokens, int buffer_size, Node *ast, int *cursor)
{
    Node *function_node = (Node *)malloc(sizeof(Node));
    if (function_node == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory for function_node.\n");
        abort();
    }
    function_node->branch_count = 0;
    function_node->children = NULL;
    function_node->line = var_tokens[0]->lineNumber;
    function_node->start_position = var_tokens[0]->position;
    function_node->end_position = var_tokens[buffer_size - 1]->position;
    function_node->type = SYSTEM_CALL_NODE;
    function_node->label = strdup("System call function");
    if (function_node->label == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory for identifier.\n");
        free(function_node);
        abort();
    }
    function_node->data.function_call.identifier = strdup(var_tokens[0]->value);

    if (function_node->data.function_call.identifier == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory for identifier.\n");
        free(function_node);
        abort();
    }
    function_node->data.function_call.number_args = 0;
    // Initialize args array
    function_node->data.function_call.args = (ValueNode **)malloc(buffer_size * sizeof(ValueNode *));
    if (function_node->data.function_call.args == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory for args array.\n");
        free(function_node->data.function_call.identifier);
        free(function_node);
        abort();
    }
    // Parse function arguments
for (int i = 1; i < buffer_size; i++) {
    if (var_tokens[i]->type != Comma) {
        printf("res: %s\n", var_tokens[i]->value);

        function_node->data.function_call.args[function_node->data.function_call.number_args] = parse_value(&var_tokens[i], buffer_size - i, ast);
        function_node->data.function_call.number_args += 1;
    }
}

    return function_node;
}

Node *parse_value(Token **var_tokens, int buffer_size, Node *ast)
{

    Node *val_node = (Node *)malloc(sizeof(Node));
    malloc_error(val_node);
    val_node->branch_count = 0;
    val_node->children = NULL;
    val_node->line = var_tokens[0]->lineNumber;
    val_node->start_position = var_tokens[0]->position;
    val_node->end_position = var_tokens[buffer_size - 1]->position;
    switch (var_tokens[0]->type)
    {
    case Number:
        val_node->type = NUMBER_VALUE;
        val_node->data.value.value = strdup(var_tokens[0]->value);
        val_node->data.value.value_type = UnsignedInteger_8;
        val_node->label = strdup("Litteral value");
        return val_node;
        break;

    case Ampersand:
        val_node->type = POINTER_NODE;
        val_node->data.value.value = strdup(var_tokens[1]->value);
        val_node->data.value.value_type = UnsignedInteger_8;
        val_node->label = strdup("Variable reference value");
        Node *pointer_ref_var = symbol_table_lookup(ast, var_tokens[1]->value, VARIABLE_NODE);
        return pointer_ref_var;
        break;

    case Identifier:
        printf("res: %s", var_tokens[0]->value);
        val_node->type = FUNCTION_CALL_ARGUMENT_NODE;
        val_node->data.value.value = strdup(var_tokens[0]->value);
        val_node->data.value.value_type = UnsignedInteger_8; // for now
        val_node->label = strdup("Function call argument");
        Node *ref_var = symbol_table_lookup(ast, var_tokens[0]->value, VARIABLE_NODE);
        return ref_var;
        break;

    default:
        printf("Error");
        break;
    }
}

Node *parse_variable(Token **tokens, int token_number, Token **var_tokens, int buffer_size, Node *ast, int *cursor)
{
    Node *var_node = (Node *)malloc(sizeof(Node));
    if (var_node == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for var_node\n");
        exit(1); // or handle the error appropriately
    }
    var_node->branch_count = 0;
    var_node->children = NULL;
    var_node->line = var_tokens[0]->lineNumber;
    var_node->start_position = var_tokens[0]->position;
    var_node->end_position = var_tokens[buffer_size - 1]->position;

    if (var_tokens[0]->type == VariableDefinition)
    {
        var_node->type = INITIALIZE_VARIABLE_NODE;
        strcpy(var_node->data.variable.identifier, var_tokens[1]->value);
        var_node->label = strdup("Variable initialization");
        ast->table = realloc(ast->table, (ast->nb_var + 1) * sizeof(Symbol));
        ast->nb_var += 1;
        if (ast->table == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        ast->table[ast->nb_var - 1].identifier = strdup(var_tokens[1]->value);
        ast->table[ast->nb_var - 1].type = INITIALIZE_VARIABLE_NODE;
        ast->table[ast->nb_var - 1].data.variable = var_node;

        var_node->data.variable.value_type = get_value_type(var_tokens[3]);
    }
    else if (var_tokens[0]->type == Identifier)
    {
        var_node->type = VARIABLE_ASSIGN_NODE;

        strcpy(var_node->data.variable.identifier, var_tokens[0]->value);
        var_node->label = strdup("Variable assignement");
        ast->table = realloc(ast->table, (ast->nb_var + 1) * sizeof(Symbol));

        ast->nb_var += 1;
        if (ast->table == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        ast->table[ast->nb_var - 1].identifier = strdup(var_tokens[0]->value);
        ast->table[ast->nb_var - 1].type = VARIABLE_ASSIGN_NODE;
        ast->table[ast->nb_var - 1].data.variable = var_node;
    }
    else
    {
        fprintf(stderr, "Error: unexpected token type encountered\n");
        exit(1); // or handle the error appropriately
    }

    var_node->children = malloc(sizeof(Node *));
    if (var_node->children == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory.\n");
        exit(1);
    }
    // for (int i = 0; i < token_number; i++)
    // {
    //     FREE_TOKEN(var_tokens[i]);
    // }

    var_node->children[var_node->branch_count] = parse(tokens, token_number, ast, cursor);
    var_node->branch_count += 1;

    return var_node;
}

Node *parse_pointer(Token **tokens, int token_number, Token **var_tokens, int buffer_size, Node *ast, int *cursor)
{
    Node *ptr_node = (Node *)malloc(sizeof(Node));
    if (ptr_node == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for var_node\n");
        exit(1); // or handle the error appropriately
    }
    ptr_node->branch_count = 0;
    ptr_node->children = NULL;
    ptr_node->line = var_tokens[0]->lineNumber;
    ptr_node->start_position = var_tokens[0]->position;
    ptr_node->end_position = var_tokens[buffer_size - 1]->position;

    if (var_tokens[0]->type == VariableDefinition)
    {
        ptr_node->type = INITIALIZE_POINTER_NODE;
        ptr_node->data.pointer.identifier = strdup(var_tokens[1]->value);

        ptr_node->label = strdup("Pointer initialization");

        ast->table = realloc(ast->table, (ast->nb_var + 1) * sizeof(Symbol));
        ast->nb_var += 1;
        if (ast->table == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        ast->table[ast->nb_var - 1].identifier = strdup(var_tokens[1]->value);
        ast->table[ast->nb_var - 1].type = INITIALIZE_POINTER_NODE;
        ast->table[ast->nb_var - 1].data.pointer = ptr_node;
    }
    else if (var_tokens[0]->type == Identifier)
    {
        ptr_node->type = POINTER_ASSIGN_NODE;
        ptr_node->data.pointer.identifier = strdup(var_tokens[0]->value);

        ptr_node->label = strdup("Pointer assignement");
        ast->table = realloc(ast->table, (ast->nb_var + 1) * sizeof(Symbol));
        ast->nb_var += 1;
        if (ast->table == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        ast->table[ast->nb_var - 1].identifier = strdup(var_tokens[1]->value);
        ast->table[ast->nb_var - 1].type = POINTER_ASSIGN_NODE;
        ast->table[ast->nb_var - 1].data.pointer = ptr_node;
    }
    else
    {
        fprintf(stderr, "Error: unexpected token type encountered\n");
        exit(1); // or handle the error appropriately
    }

    ptr_node->children = malloc(sizeof(Node *));
    Node *identifier_var_ref = parse(tokens, token_number, ast, cursor);
    // printf("Referenced variable %s", identifier_var_ref->data.variable.identifier);
    ptr_node->data.pointer.referenced_variable = identifier_var_ref;
    if (ptr_node->children == NULL)
    {
        fprintf(stderr, "Memory allocation error");
        exit(0);
    }
    // for (int i = 0; i < token_number; i++)
    // {
    //     FREE_TOKEN(var_tokens[i]);
    // }

    ptr_node->children[ptr_node->branch_count] = parse(tokens, token_number, ast, cursor);
    ptr_node->branch_count += 1;

    return ptr_node;
}

void append_token(Token **tokens, Token **buffer, int *buffer_s, int *cursor)
{
    buffer = realloc(buffer, (*buffer_s) + 1 * sizeof(Token *));
    if (buffer == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory.\n");
        exit(1);
    }
    buffer[(*buffer_s)] = tokens[*cursor];
    (*buffer_s) += 1;
}

Token *get_next_token(Token **tokens, int token_number, int *cursor)
{

    if (*cursor >= token_number)
    {
        return NULL;
    }
    *cursor += 1;
    return tokens[*cursor - 1];
}

// will take a node and append to its child or parent the rest of the tree
Node *parse(Token **tokens, int token_number, Node *ast, int *cursor)
{

    Token **buffer = malloc(sizeof(Token *));
    int buffer_s = 0;

    Token *current_token = get_next_token(tokens, token_number, cursor);
    if (current_token == NULL)
    {
        return ast;
    }

    if (current_token->type == VariableDefinition)
    {
        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
        if (buffer == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        buffer[buffer_s++] = current_token;

        current_token = get_next_token(tokens, token_number, cursor);

        if (current_token->type == Identifier)
        {
            buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
            if (buffer == NULL)
            {
                fprintf(stderr, "Fatal: failed to allocate memory.\n");
                exit(1);
            }
            buffer[buffer_s++] = current_token;

            current_token = get_next_token(tokens, token_number, cursor);

            if (current_token->type == Colon)
            {
                buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                if (buffer == NULL)
                {
                    fprintf(stderr, "Fatal: failed to allocate memory.\n");
                    exit(1);
                }
                buffer[buffer_s++] = current_token;

                current_token = get_next_token(tokens, token_number, cursor);

                if (current_token->type == Star)
                {
                    buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                    if (buffer == NULL)
                    {
                        fprintf(stderr, "Fatal: failed to allocate memory.\n");
                        exit(1);
                    }
                    buffer[buffer_s++] = current_token;

                    current_token = get_next_token(tokens, token_number, cursor);

                    if (IS_TYPE(current_token->type))
                    {
                        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                        if (buffer == NULL)
                        {
                            fprintf(stderr, "Fatal: failed to allocate memory.\n");
                            exit(1);
                        }
                        buffer[buffer_s++] = current_token;

                        current_token = get_next_token(tokens, token_number, cursor);

                        if (current_token->type == Equals)
                        {
                            // Reallocate buffer
                            buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                            if (buffer == NULL)
                            {
                                fprintf(stderr, "Fatal: failed to allocate memory.\n");
                                exit(1);
                            }
                            buffer[buffer_s++] = current_token;

                            // Reallocate memory for ast->children
                            ast->children = realloc(ast->children, (ast->branch_count + 1) * sizeof(Node *));
                            if (ast->children == NULL)
                            {
                                fprintf(stderr, "Memory allocation error");
                                exit(1);
                            }
                            ast->children[ast->branch_count] = parse_pointer(tokens, token_number, buffer, buffer_s, ast, cursor);
                            ast->branch_count += 1;

                            // Parse the next tokens
                            parse(tokens, token_number, ast, cursor);
                        }

                        else
                        {
                            fprintf(stderr, "Error: invalid syntax, you forgot the '=' at the pointer declaration %d:%d\n", current_token->lineNumber, current_token->position);
                            exit(1);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Error: invalid syntax, %s is not a valid pointer type %d:%d\n", current_token->value, current_token->lineNumber, current_token->position);
                        exit(1);
                    }
                }
                else if (IS_TYPE(current_token->type))
                {
                    buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                    if (buffer == NULL)
                    {
                        fprintf(stderr, "Fatal: failed to allocate memory.\n");
                        exit(1);
                    }
                    buffer[buffer_s++] = current_token;

                    current_token = get_next_token(tokens, token_number, cursor);

                    if (current_token->type == Equals)
                    {
                        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                        if (buffer == NULL)
                        {
                            fprintf(stderr, "Fatal: failed to allocate memory.\n");
                            exit(1);
                        }
                        buffer[buffer_s++] = current_token;

                        ast->children = realloc(ast->children, (ast->branch_count + 1) * sizeof(Node *));
                        if (ast->children == NULL)
                        {
                            fprintf(stderr, "Memory allocation error");
                            exit(0);
                        }
                        ast->children[ast->branch_count] = parse_variable(tokens, token_number, buffer, buffer_s, ast, cursor);
                        ast->branch_count += 1;
                        parse(tokens, token_number, ast, cursor);
                    }
                    else
                    {
                        fprintf(stderr, "Error: invalid syntax, you forgot the '=' at the variable declaration %d:%d\n", current_token->lineNumber, current_token->position);
                        exit(1);
                    }
                }
                else
                {
                    fprintf(stderr, "Error: missing '*' after the colon %d:%d\n", current_token->lineNumber, current_token->position);
                    exit(1);
                }
            }
            else
            {
                fprintf(stderr, "Error: missing colon after the identifier %d:%d\n", current_token->lineNumber, current_token->position);
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "Error: not a valid identifier %d:%d\n", current_token->lineNumber, current_token->position);
            exit(1);
        }
    }
    else if (current_token->type == Number)
    {
        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
        if (buffer == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        buffer[buffer_s++] = current_token;

        current_token = get_next_token(tokens, token_number, cursor);

        if (current_token->type == SemiColon)
        {
            return parse_value(buffer, buffer_s, ast);
        }
        else
        {
            printf("Not a number");
        }
    }
    else if (current_token->type == Ampersand)
    {
        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
        if (buffer == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        buffer[buffer_s++] = current_token;
        current_token = get_next_token(tokens, token_number, cursor);
        if (current_token->type == Identifier)
        {
            buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
            if (buffer == NULL)
            {
                fprintf(stderr, "Fatal: failed to allocate memory.\n");
                exit(1);
            }

            buffer[buffer_s++] = current_token;

            return parse_value(buffer, buffer_s, ast);

            // should add more checks for poiners ect.., for now we suppose the syntax is simple
        }
        else
        {
            fprintf(stderr, "Error: invalid identifier for the pointer assignement %d:%d\n", current_token->lineNumber, current_token->position);
            exit(1);
        }
    }
    if (current_token->type == ExitSystemcall)
    {
        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
        if (buffer == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        buffer[buffer_s++] = current_token;

        current_token = get_next_token(tokens, token_number, cursor);
        if (current_token->type == ParenthesisOpen)
        {
            current_token = get_next_token(tokens, token_number, cursor);

            while (current_token->type != ParenthesisClose)
            {
                buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                if (buffer == NULL)
                {
                    fprintf(stderr, "Fatal: failed to allocate memory.\n");
                    exit(1);
                }
                buffer[buffer_s++] = current_token;

                current_token = get_next_token(tokens, token_number, cursor);
            }

            ast->children[ast->branch_count] = parse_system_call(tokens, token_number, buffer, buffer_s, ast, cursor);
            ast->branch_count += 1;
            parse(tokens, token_number, ast, cursor);
        }

        else
        {
            fprintf(stderr, "Error: was waiting for a parenthesis %d:%d\n", current_token->lineNumber, current_token->position);
            exit(1);
        }
    }
    else
    {
    }
    return ast;
}

void print_ast(Node *ast, int level)
{
    if (ast == NULL)
    {
        return;
    }
    if (strcmp(ast->label, "Root") == 0)
    {
        printf("\nNode: %s\n", ast->label);
    }

    for (int j = 0; j < level; j++)
    {
        printf("\t");
    }

    for (int i = 0; i < ast->branch_count; i++)
    {

        if (ast->children[i] && ast->children[i]->type == INITIALIZE_VARIABLE_NODE)
        {
            for (int j = 0; j <= level; j++)
            {
                printf("\t");
            }
            printf("Node: %s, identifier: %s, value type: %d\n", ast->children[i]->label, ast->children[i]->data.variable.identifier, ast->children[i]->data.variable.value_type);
            print_ast(ast->children[i], level + 1);
        }
        else if (ast->children[i] && ast->children[i]->type == SYSTEM_CALL_NODE)
        {
            printf("Node: %s, value: %s\n", ast->children[i]->label, ast->children[i]->data.function_call.identifier);
            for (int j = 0; j <= level; j++)
            {
                printf("\t");
            }

            printf("Number of arguments: %d\n", ast->children[i]->data.function_call.number_args);
             for (int k = 0; k < ast->children[i]->data.function_call.number_args; k++)
            {
                printf("\targ: %s\n", ast->children[i]->data.function_call.args[k]->data.value.value);
            }
        }

        else if (ast->children[i] && ast->children[i]->type == NUMBER_VALUE)
        {
            for (int j = 0; j <= level; j++)
            {
                printf("\t");
            }
            printf("Node: %s, value: %s, value type: %d\n", ast->children[i]->label, ast->children[i]->data.value.value, ast->children[i]->data.value.value_type);
        }
        else if (ast->children[i] && ast->children[i]->type == INITIALIZE_POINTER_NODE)
        {
            for (int j = 0; j <= level; j++)
            {
                printf("\t");
            }
            if (ast->children[i]->data.pointer.referenced_variable != NULL)
            {
                printf("Node: %s, identifier: %s, referenced variable value: %s, referenced variable value type: %d\n", ast->children[i]->label,
                       ast->children[i]->data.pointer.identifier, ast->children[i]->data.pointer.referenced_variable->data.variable.identifier, ast->children[i]->data.pointer.referenced_variable->data.variable.value_type);
                print_ast(ast->children[i]->data.pointer.referenced_variable, level + 1);
            }
            else
            {
                printf("identifier: %s, referenced variable: NULL");
            }
            print_ast(ast->children[i], level + 1);
        }
    }
}