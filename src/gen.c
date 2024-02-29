

#include "gen.h"

void gen_variable_code(FILE *file, Node *ast)
{
    printf("%s: ", ast->data.variable.identifier);

    // we suppose it's an integer
    char *asm_type;

    switch (ast->children[0]->data.value.value_type)
    {
    case UnsignedInteger_8:
        asm_type = ".long";
        break;
    case Char_type:
        asm_type = ".byte";
        break;
    case String_type:
        asm_type = ".asciz";
        break;
    case Boolean_type:
        asm_type = ".byte";
        break;
    default:
        asm_type = ".byte";
        break;
    }
    fprintf(file, "%s: \n", ast->data.variable.identifier);
    fprintf(file, "\t%s %s\n", asm_type, ast->children[0]->data.value.value);
    // printf_variable(file, ast->data.variable.identifier);

}

// void printf_variable(FILE *file, char *identifier)
// {
//     // movl $a, %esi      # Load the address of variable 'a' into the 'esi' register
//     // movl (%esi), %esi  # Load the value of 'a' into the 'esi' register
//     // movl $0, %eax      # Clear the %eax register
//     // call printf        # Call the printf function
//     fprintf(file, ".section .text\n"); // Text section

//     // Placeholder for generated code
//     fprintf(file, "_start:\n");
//     fprintf(file, "\tmovl $%s, %%esi \n", identifier);
//     fprintf(file, "\tmovl (%%esi), %%esi \n");
//     fprintf(file, "\tmovl $0, %%eax \n");
//     fprintf(file, "\tcall printf\n");
//     fprintf(file, "\tmovq $60, %%rax\n");   // syscall number for sys_exit
//     fprintf(file, "\txorq %%rdi, %%rdi\n"); // exit code 0
//     fprintf(file, "\tsyscall\n");           // invoke syscall
// }

void gen_pointer_code(FILE *file, Node *ast)
{
}

void gen_code(FILE *file, Node *ast)
{
    for (int i = 0; i < ast->branch_count; i++)
    {
        if (ast->children[i] && ast->children[i]->type == INITIALIZE_VARIABLE_NODE)
        {

            gen_variable_code(file, ast->children[i]);
        }

        else if (ast->children[i] && ast->children[i]->type == INITIALIZE_POINTER_NODE)
        {

            if (ast->children[i]->data.pointer.referenced_variable != NULL)
            {
                gen_code(file, ast->children[i]->data.pointer.referenced_variable);
            }

            gen_code(file, ast->children[i]);
        }
    }
}

void gen_asm(const char *filename, Node *ast)
{
    FILE *file = fopen(filename, "w"); // Open the file for writing

    if (file != NULL)
    {
        // Write assembly boilerplate to the file
        fprintf(file, ".global _start\n"); // Entry point symbol

        fprintf(file, ".section .data\n"); // Data section for variables

        gen_code(file, ast);

        fclose(file); // Close the file
    }
    else
    {
        printf("Error: Could not create assembly file '%s'.\n", filename);
    }
}
