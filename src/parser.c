#include "parser.h"

#define u32 uint32_t

void parse_value() {}

void print_ast(Node *tree)
{
    printf("Tree\n");
    for (int i = 0; i < tree->branch_count; i++)
    {
        printf("identifier: %s, address: %d, context: %s, ligne: %d, position: %s", tree[i].data.pointer->identifier,
               tree[i].data.pointer->address, tree[i].data.pointer->ptr_context, tree[i].line, tree[i].start_position);
        print_ast(tree[i].data.pointer->referenced_variable);
        printf("identifier: %s, type: %d, context: %s, ligne: %d, position: %s, parent: %s", tree[i].data.variable->identifier,
               tree[i].data.variable->variable_type, tree[i].data.variable->var_context, tree[i].line, tree[i].start_position, tree[i].data.variable->parent);
    }
}

void free_tree(struct Node *tree)
{
    for (int i = 0; i < tree->branch_count; i++)
    {
        if (tree->children != NULL)
        {
            free_tree(tree->children);
        }
        if (tree[i].children != NULL)
        {
            free_tree(tree[i].children);
        }
    }
    free(tree);
}

void parse_variable(Token **var_tokens, int buffer_size, struct symbol_tab *tab)
{
    // we check there are no syntax error, so we're ready to parse

    if (buffer_size == 5)
    {
        if (!check_variable_syntax_error(var_tokens, buffer_size))
        {
            // Node *tree = malloc(sizeof(struct Node));
            // tree->branch_count = 0;
            // free_tree(tree);
        }
    }

    // check_variable_syntax_error(var_tokens, buffer_size);
    // get_var_info(tree, var_tokens, buffer_size, tab);
    // print_ast(tree);

    // we're already know the first token will be 'set' and the last is ';' so we parse what's between
    // for (int i = 0; i < buffer_size; i++)
    // {
    //     // printf("token: %s\n", var_tokens[i]->value);
    // }
}

Node *make_node(u32 position, u32 line)
{
}
Node *parse(Token **tokens, int token_number)
{
    struct symbol_tab *tab = (struct symbol_tab *)malloc(sizeof(struct symbol_tab));
    if (tab == NULL)
    {
        printf("Fatal: Memory allocation error: couldn't allocate space at %d:%d", 0, 0);
        exit(0);
    }
    tab->sym_count = 0;
    Token **buffer = (Token **)malloc(sizeof(Token *));
    if (buffer == NULL)
    {
        printf("Fatal: Memory allocation error: couldn't allocate space at %d:%d", 0, 0);
        exit(0);
    }
    int buffer_size = 0;
    for (int i = 0; i < token_number; i++)
    {
        if (tokens[i]->type == VariableDefinition || (tokens[i]->type == Identifier && tokens[i + 1]->type == Colon && IS_TYPE(tokens[i + 3]->type)))
        {
            while (strcmp(tokens[i]->value, "=") != 0)
            {
                // printf("val: %s\n", tokens[i]->value);

                buffer = realloc(buffer, (buffer_size + 1) * sizeof(Token *));
                buffer[buffer_size] = tokens[i];
                buffer_size++;
                i++;
            }
            buffer = realloc(buffer, buffer_size * sizeof(Token *));
            buffer[buffer_size] = tokens[i];
            buffer_size++;
            parse_variable(buffer, buffer_size, tab);
            buffer_size = 0;
            free(buffer); // Free the dynamically allocated buffer memory
        }
    }
    return NULL;
}
