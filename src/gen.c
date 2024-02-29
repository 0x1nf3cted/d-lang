#include "gen.h"

// this is code is just for testing purpouses, so it's not the final version

void gen_variable_code(FILE *file, Node *ast)
{
    fprintf(file, "section .data\n"); // Data section for variables

    printf("%s: ", ast->data.variable.identifier);

    // we suppose it's an integer
    char *asm_type;

    switch (ast->children[0]->data.value.value_type)
    {
    case UnsignedInteger_8:
        asm_type = "dd";
        break;
    case Char_type:
        asm_type = "dd";
        break;
    case String_type:
        asm_type = "dd";
        break;
    case Boolean_type:
        asm_type = "dd";
        break;
    default:
        asm_type = "dd";
        break;
    }
    fprintf(file, "\t%s %s %s\n", ast->data.variable.identifier, asm_type, ast->children[0]->data.value.value);
    // printf_variable(file, ast->data.variable.identifier);
}

void gen_pointer_code(FILE *file, Node *ast)
{
}

void gen_system_call_code(FILE *file, Node *ast)
{
    fprintf(file, "_start:\n");

    fprintf(file, "\tmov eax, dword [%s]\n", ast->data.function_call.args[0]->data.variable.identifier); // invoke syscall

    fprintf(file, "\tmov ebx, eax\n");
    fprintf(file, "\tmov eax, 1\n");
    fprintf(file, "\tint 0x80\n");

    //     movl %eax, %ebx  # Exit status
    // movl $60, %eax   # syscall number for exit
    // int $0x80        # Call kernel
}

void gen_code(FILE *file, Node *ast)
{
    for (int i = 0; i < ast->branch_count; i++)
    {
        if (ast->children[i] && ast->children[i]->type == INITIALIZE_VARIABLE_NODE)
        {

            gen_variable_code(file, ast->children[i]);
        }

        if (ast->children[i] && ast->children[i]->type == SYSTEM_CALL_NODE)
        {

            gen_system_call_code(file, ast->children[i]);
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
        fprintf(file, "section .text\n"); // Entry point symbol

        fprintf(file, "\tglobal _start\n"); // Entry point symbol

        gen_code(file, ast);

        fclose(file); // Close the file
    }
    else
    {
        printf("Error: Could not create assembly file '%s'.\n", filename);
    }
}
