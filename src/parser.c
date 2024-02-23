#include "parser.h"

#define u32 uint32_t

static int cursor = 0;

void malloc_error(Node *p)
{
    if (p == NULL)
    {
        fprintf(stderr, "Fatal: failed to allocate memory.\n");
        abort();
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

void append_token(Token **tokens, Token **buffer, int *buffer_s)
{
    buffer = realloc(buffer, *(++buffer_s) * sizeof(Token *));
    buffer[*buffer_s - 1] = tokens[cursor];
}

Token *get_next_token(Token **tokens, int token_number)
{

    if (cursor >= token_number)
    {
        perror("error: eached the end of the file");
        abort();
    }
    return tokens[cursor++];
}

// will take a node and append to its child or parent the rest of the tree
Node *parse(Token **tokens, int token_number, Node *ast)
{

    Token **buffer = malloc(sizeof(Token *));
    int buffer_s = 0;

    Token *current_token = malloc(sizeof(Token));
    current_token = get_next_token(tokens, token_number);
    if (current_token->type == VariableDefinition)
    {
        buffer = realloc(buffer, (buffer_s + 1) * sizeof(Token *));
        buffer[buffer_s++] = current_token;
        current_token = get_next_token(tokens, token_number);

        if (current_token->type == Identifier)
        {

            append_token(tokens, buffer, &buffer_s);
            current_token = get_next_token(tokens, token_number);

            if (current_token->type == Colon)
            {

                append_token(tokens, buffer, &buffer_s);
                current_token = get_next_token(tokens, token_number);

                // we check if it's variable or pointer
                if (current_token->type == Star)
                {
                    append_token(tokens, buffer, &buffer_s);
                    current_token = get_next_token(tokens, token_number);
                    if (IS_TYPE(current_token->type))
                    {
                        append_token(tokens, buffer, &buffer_s);
                        current_token = get_next_token(tokens, token_number);
                        if (current_token->type == Equals)
                        {
                            append_token(tokens, buffer, &buffer_s);
                            current_token = get_next_token(tokens, token_number);
                            parse_variable(buffer, buffer_s);
                        }
                        else
                        {
                            fprintf(stderr, "Error: invalid syntax, you forgot the '=' at the variable declaration %d:%d", current_token->value, current_token->lineNumber, current_token->position);
                            abort();
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Error: invalid syntax, %s is not a pointer type %d:%d", current_token->value, current_token->lineNumber, current_token->position);
                        abort();
                    }
                }
                else if (IS_TYPE(current_token->type))
                {
                    append_token(tokens, buffer, &buffer_s);
                    current_token = get_next_token(tokens, token_number);
                    if (current_token->type == Equals)
                    {
                        append_token(tokens, buffer, &buffer_s);
                        current_token = get_next_token(tokens, token_number);
                        parse_variable(buffer, buffer_s);
                    }
                    else
                    {
                        fprintf(stderr, "Error: invalid syntax, you forgot the '=' at the variable declaration %d:%d", current_token->value, current_token->lineNumber, current_token->position);
                        abort();
                    }
                }
                else
                {
                    fprintf(stderr, "Error: invalid syntax, %s is not a type %d:%d", current_token->value, current_token->lineNumber, current_token->position);
                    abort();
                }
            }
            else
            {
                fprintf(stderr, "Your forgot to add a colon after the identifier at %d:%d", current_token->lineNumber, current_token->position);
                abort();
            }
        }
        else
        {
            fprintf(stderr, "Not a valid identifier at %d:%d", current_token->lineNumber, current_token->position);
            abort();
        }
    }
    else
    {
    }
}