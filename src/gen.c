

#include "gen.h"


void gen_variable_code(Node *ast){

}

void gen_pointer_code(Node *ast){

}

void gen_code(Node *ast)
{
    for (int i = 0; i < ast->branch_count; i++)
    {
        if (ast->children[i] && ast->children[i]->type == INITIALIZE_VARIABLE_NODE)
        {
 
            gen_variable_code(ast->children[i]);
        }
        else if (ast->children[i] && ast->children[i]->type == NUMBER_VALUE)
        {

            printf("Node: %s, value: %s, value type: %d\n", ast->children[i]->label, ast->children[i]->data.value.value, ast->children[i]->data.value.value_type);
        }
        else if (ast->children[i] && ast->children[i]->type == INITIALIZE_POINTER_NODE)
        {

            if (ast->children[i]->data.pointer.referenced_variable != NULL)
            {
                printf("Node: %s, identifier: %s, referenced variable value: %s, referenced variable value type: %d\n", ast->children[i]->label,
                       ast->children[i]->data.pointer.identifier, ast->children[i]->data.pointer.referenced_variable->data.variable.identifier, ast->children[i]->data.pointer.referenced_variable->data.variable.value_type);
                gen_code(ast->children[i]->data.pointer.referenced_variable);
            }

            gen_code(ast->children[i]);
        }
    }
}

void gen_asm(const char *filename)
{
    FILE *file = fopen(filename, "w"); // Open the file for writing

    if (file != NULL)
    {
        // Write assembly boilerplate to the file
        fprintf(file, ".global _start\n"); // Entry point symbol

        fprintf(file, ".section .data\n"); // Data section for variables

        fprintf(file, ".section .text\n"); // Text section

        // Placeholder for generated code
        fprintf(file, "_start:\n");
        fprintf(file, "    # Your generated code goes here\n");

        // Exit the program
        fprintf(file, "    movq $60, %%rax\n");   // syscall number for sys_exit
        fprintf(file, "    xorq %%rdi, %%rdi\n"); // exit code 0
        fprintf(file, "    syscall\n");           // invoke syscall

        fclose(file); // Close the file
    }
    else
    {
        printf("Error: Could not create assembly file '%s'.\n", filename);
    }
}
