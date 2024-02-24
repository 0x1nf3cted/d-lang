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

void print_buffer(Token **buffer, int buffer_s)
{
    for (int i = 0; i < buffer_s - 1; i++)
    {
        printf("Token: %s", buffer[i]->value);
    }
}

void parse_value()
{
}

void parse_variable(Token **var_tokens, int buffer_size)
{
    printf("reached");
}

Node *make_node(u32 position, u32 line, TokenType)
{
    Node *node = (Node *)malloc(sizeof(Node));
    malloc_error(node);
    node->branch_count = 0;
    node->children = 0;
    node->line = 0;
    node->start_position = 0;
    node->type = ROOT,
    strcpy(node->value, "Root");
    node->children = NULL;
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
Node *parse(Token **tokens, int token_number, Node *ast)
{
    int cursor = 0;
    Token **buffer = malloc(sizeof(Token *));
    int buffer_s = 0;

    Token *current_token = get_next_token(tokens, token_number, &cursor);

    if (current_token->type == VariableDefinition)
    {
        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
        if (buffer == NULL)
        {
            fprintf(stderr, "Fatal: failed to allocate memory.\n");
            exit(1);
        }
        buffer[buffer_s++] = current_token;

        current_token = get_next_token(tokens, token_number, &cursor);

        if (current_token->type == Identifier)
        {
            buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
            if (buffer == NULL)
            {
                fprintf(stderr, "Fatal: failed to allocate memory.\n");
                exit(1);
            }
            buffer[buffer_s++] = current_token;

            current_token = get_next_token(tokens, token_number, &cursor);

            if (current_token->type == Colon)
            {
                buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                if (buffer == NULL)
                {
                    fprintf(stderr, "Fatal: failed to allocate memory.\n");
                    exit(1);
                }
                buffer[buffer_s++] = current_token;

                current_token = get_next_token(tokens, token_number, &cursor);

                if (current_token->type == Star)
                {
                    buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                    if (buffer == NULL)
                    {
                        fprintf(stderr, "Fatal: failed to allocate memory.\n");
                        exit(1);
                    }
                    buffer[buffer_s++] = current_token;

                    current_token = get_next_token(tokens, token_number, &cursor);

                    if (IS_TYPE(current_token->type))
                    {
                        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                        if (buffer == NULL)
                        {
                            fprintf(stderr, "Fatal: failed to allocate memory.\n");
                            exit(1);
                        }
                        buffer[buffer_s++] = current_token;

                        current_token = get_next_token(tokens, token_number, &cursor);

                        if (current_token->type == Equals)
                        {
                            buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
                            if (buffer == NULL)
                            {
                                fprintf(stderr, "Fatal: failed to allocate memory.\n");
                                exit(1);
                            }
                            buffer[buffer_s++] = current_token;

                            current_token = get_next_token(tokens, token_number, &cursor);
                            for (int i = 0; i < buffer_s; i++)
                            {
                                printf("Token: %s\n", buffer[i]->value);
                            }

                            parse_variable(buffer, buffer_s);
                        }
                        else
                        {
                            fprintf(stderr, "Error: invalid syntax, you forgot the '=' at the variable declaration %d:%d\n", current_token->lineNumber, current_token->position);
                            exit(1);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Error: invalid syntax, %s is not a valid pointer type %d:%d\n", current_token->value, current_token->lineNumber, current_token->position);
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
    else
    {
        // Handle other cases if needed
    }

    return ast;
}
