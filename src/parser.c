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
    }
    else if (var_tokens[0]->type == Identifier)
    {
        var_node->type = VARIABLE_ASSIGN_NODE;

        strcpy(var_node->data.variable.identifier, var_tokens[0]->value);
        var_node->label = strdup("Variable assignement");
    }
    else
    {
        fprintf(stderr, "Error: unexpected token type encountered\n");
        exit(1); // or handle the error appropriately
    }

    var_node->children = malloc(sizeof(Node));
    malloc_error(var_node->children);
    var_node->children[var_node->branch_count] = parse(tokens, token_number, ast, cursor);
    var_node->branch_count += 1;

    return var_node;
}

void parse_pointer(Token **tokens, int token_number, Token **var_tokens, int buffer_size, Node *ast)
{
    printf("pointer parser reached");
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
        perror("error: reached the end of the file");
        exit(1);
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

                            parse_pointer(tokens, token_number, buffer, buffer_s, ast);
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
                        ast->children = malloc(sizeof(Node));
                        malloc_error(ast->children);
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
    else
    {
    }

    return ast;
}
