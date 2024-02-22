#include "syntax.h"

bool check_variable_syntax_error(Token **var_tokens, int buffer_size)
{
    uint32_t error_nb = 0;

    if (var_tokens[0]->type == VariableDefinition)
    {
        if (var_tokens[1]->type != Identifier)
        {
            printf("Error: '%s' is not a valid identifier, error at %d:%d \n", var_tokens[1]->value, var_tokens[0]->lineNumber, var_tokens[0]->position);
            return false;
        }
        else if (var_tokens[1]->type == Identifier)
        {
            
            if (var_tokens[2]->type != Colon)
            {
                if (!IS_TYPE(var_tokens[3]->type) || var_tokens[2]->type != Identifier)
                {
                    printf("Error: '%s' is not a valid delimiter in dzair, error at %d:%d \n", var_tokens[2]->value, var_tokens[2]->lineNumber, var_tokens[2]->position);
                    return false;
                }
                printf("Error: you forgot to include ':' between the identifier and the type, error at %d:%d \n", var_tokens[2]->lineNumber, var_tokens[2]->position);
                return false;
            }
            
            if (IS_TYPE(var_tokens[3]->type) != true)
            {
                printf("Error: '%s' is not a valid data type, error at %d:%d \n", var_tokens[4]->value, var_tokens[3]->lineNumber, var_tokens[3]->position);
                return false;
            }
            
            if (var_tokens[4]->type != Equals)
            {
                
                if (var_tokens[5]->type == Equals)
                {
                    return true;
                }
                printf("Error: '=' was expected but received '%s', error at %d:%d \n", var_tokens[4]->value, var_tokens[4]->lineNumber, var_tokens[4]->position);
                return false;
            }
            // else
            // {
            //     // i++; was giving an error
            //     Token **result = (Token **)malloc((buffer_size - i) * sizeof(Token *)); // +1 for NULL terminator
            //     if (result == NULL)
            //     {
            //         printf("Error: couldn't allocate memory for syntax checker");
            //         exit(0);
            //     }

            //     for (int j = 0; j < buffer_size; j++)
            //     {
            //         result[j] = var_tokens[i + j];
            //     }

            //     // result[buffer_size] = NULL;
            //     check_value_syntax_error(result, buffer_size - i);
            //     free(result); // Freeing the dynamically allocated memory after usage
            // }
        }
    }
}

/**
 * get the variable type
 we'll update it later, for now it does basic check
  */

void get_var_info(struct Node *tree, Token **var_tokens, int buffer_size, struct symbol_tab *tab)
{

    /**
     *  if true, it can be either:
     *     INITIALIZED_VAR_NODE,   // set a: u8 = 10;
            UNINITIALIZED_VAR_NODE, // set a: u8;
            POINTER_ASSIGN_NODE,    // x = &a;
            INITIALIZED_POINTER_NODE,   // set a: u8 = 10;
            UNINITIALIZED_POINTER_NODE, // set a: u8;
    */
    int l = tree->branch_count;
    if (var_tokens[0]->type == VariableDefinition)
    {
        if ((buffer_size == 7 && IS_TYPE(var_tokens[3]->type)))
        { // it's a variable, now we check if initialized or not
            if (var_tokens[4]->type == Equals)
            {

                // Node *var_node = (Node *)malloc(sizeof(Node));
                tree = realloc(tree, (l + 1) * sizeof(Node));

                if (tree == NULL)
                {
                    printf("malloc error");
                    exit(0);
                }
                tree[l].line = var_tokens[0]->lineNumber;
                tree[l].data.variable->identifier = var_tokens[1]->value;
                tree[l].data.variable->parent = NULL;
                tree[l].data.variable->var_context = GLOBAL_CONTEXT;
                tree[l].data.variable->variable_type = var_tokens[3]->type;
                tree[l].start_position = var_tokens[0]->position;
                tree[l].end_position = var_tokens[6]->position;
                tree[l].children = NULL;
                tree->branch_count++;
                int c = tab->sym_count;
                tab = realloc(tab, (c + 1) * sizeof(struct symbol_tab));
                if (tab == NULL)
                {
                    printf("realloc error");
                    exit(0);
                }
                tab[c + 1].variable = tree[l].data.variable;
                l++;
            }
        } // it's a pointer
        else if ((buffer_size == 8 && IS_TYPE(var_tokens[4]->type) && var_tokens[3]->type == Star))
        {
            tree = realloc(tree, (l + 1) * sizeof(Node));

            if (tree == NULL)
            {
                printf("malloc error");
                exit(0);
            }
            tree[l].line = var_tokens[0]->lineNumber;
            tree[l].data.pointer->identifier = var_tokens[1]->value;
            tree[l].data.pointer->ptr_context = GLOBAL_CONTEXT;
            tree[l].data.pointer->variable_type = var_tokens[4]->type;
            tree[l].start_position = var_tokens[0]->position;
            tree[l].end_position = var_tokens[7]->position;
            tree[l].data.pointer->referenced_variable = get_variable_ref(tab, tree[l].data.pointer->identifier);
            tree[l].data.pointer->address = 0X000;
            tree[l].children = NULL;
            tree->branch_count++;
            int c = tab->sym_count;
            tab = realloc(tab, (c + 1) * sizeof(struct symbol_tab));
            if (tab == NULL)
            {
                printf("realloc error");
                exit(0);
            }
            tab[c + 1].pointer = tree[l].data.pointer;
            l++;
        }
    }
}

bool check_value_syntax_error(Token **var_tokens, int buffer_size)
{
}

Node *get_variable_ref(struct symbol_tab *tab, char *Identifier)
{
    int l = tab->sym_count;
    for (int i = 0; i < l; i++)
    {
        if (strcmp(tab[i].variable->data.variable->identifier, Identifier) == 0)
        {
            return tab[i].variable;
        }
    }
    return NULL;
}