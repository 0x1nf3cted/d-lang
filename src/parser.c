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

Node *parse_value(Token **var_tokens, int buffer_size, Node *ast, int *cursor)
{

    Node *val_node = (Node *)malloc(sizeof(Node));
    malloc_error(val_node);
    val_node->branch_count = 0;
    val_node->children = NULL;
    val_node->line = var_tokens[0]->lineNumber;
    val_node->start_position = var_tokens[0]->position;
    val_node->end_position = var_tokens[buffer_size - 1]->position;

    if (var_tokens[0]->type == Number)
    {
        val_node->type = NUMBER_VALUE;
        val_node->data.value.value = strdup(var_tokens[0]->value);
        val_node->data.value.value_type = UnsignedInteger_8;
        val_node->label = strdup("Litteral value");
        return val_node;
    }
    else if (var_tokens[0]->type == Ampersand)
    {
        val_node->type = POINTER_NODE;
        val_node->data.value.value = strdup(var_tokens[1]->value);
        val_node->data.value.value_type = UnsignedInteger_8;
        val_node->label = strdup("Variable reference value");
        Node *ref_var = symbol_table_lookup(ast, var_tokens[1]->value, VARIABLE_NODE);
        
        return ref_var;
    }
    else
    {
        printf("Error");
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

                            // Print buffer contents
                            for (int i = 0; i < buffer_s; i++)
                            {
                                printf("Token: %s\n", buffer[i]->value);
                            }

                            // Reallocate memory for ast->children
                            ast->children = realloc(ast->children, (ast->branch_count + 1) * sizeof(Node *));
                            if (ast->children == NULL)
                            {
                                fprintf(stderr, "Memory allocation error");
                                exit(1);
                            }
                            ast->children[ast->branch_count] = parse_pointer(tokens, token_number, buffer, buffer_s, ast, cursor);
                            ast->branch_count += 1;
                            printf("children number after ptr: %d\n", ast->branch_count);

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

                        for (int i = 0; i < buffer_s; i++)
                        {
                            printf("Token: %s\n", buffer[i]->value);
                        }

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
            return parse_value(buffer, buffer_s, ast, cursor);
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

            return parse_value(buffer, buffer_s, ast, cursor);

            // should add more checks for poiners ect.., for now we suppose the syntax is simple
        }
        else
        {
            fprintf(stderr, "Error: invalid identifier for the pointer assignement %d:%d\n", current_token->lineNumber, current_token->position);
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
            printf("Node: %s,\n", ast->children[i]->label);
            for (int j = 0; j <= level; j++)
            {
                printf("\t");
            }
            printf("identifier: %s,\n", ast->children[i]->data.variable.identifier);
            print_ast(ast->children[i], level + 1);
        }
        else if (ast->children[i] && ast->children[i]->type == NUMBER_VALUE)
        {
            printf("Node: %s,\n", ast->children[i]->label);
            for (int j = 0; j <= level; j++)
            {
                printf("\t");
            }
            printf("value: %s\n", ast->children[i]->data.value.value);
        }
        else if (ast->children[i] && ast->children[i]->type == INITIALIZE_POINTER_NODE)
        {
            printf("Node: %s,\n", ast->children[i]->label);
            for (int j = 0; j <= level; j++)
            {
                printf("\t");
            }
            if (ast->children[i]->data.pointer.referenced_variable != NULL)
            {
                printf("identifier: %s, referenced variable: %s",
                       ast->children[i]->data.pointer.identifier, ast->children[i]->data.pointer.referenced_variable->data.variable.identifier);
            }
            else
            {
                printf("identifier: %s, referenced variable: NULL");
            }
            print_ast(ast->children[i], level + 1);
        }
    }
}