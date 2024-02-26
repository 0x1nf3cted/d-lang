#ifndef GEN_H
#define GEN_H

#include "parser.h"
// typedef struct
// {

// } Symbol;

// will look for an available register and return it's number if it's unused
int scratch_allocate();

// will free the register
void scratch_free(int r);

// will return name
const char *scratch_name(int r);

int label_create();

const char *label_name();

void gen_asm(const char *filename, Node *ast);
void gen_code(FILE *file, Node *ast);

#endif // GEN_H
