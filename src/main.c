
#define _GNU_SOURCE

#include "lexer.h"
#include "parser.h"
#include "gen.h"

char *read_file(char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", filePath);
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate memory to store file contents
    char *content = (char *)malloc(fileSize + 1);
    if (content == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read file contents into the allocated buffer
    size_t bytesRead = fread(content, 1, fileSize, file);
    if (bytesRead != fileSize)
    {
        fprintf(stderr, "Error reading file %s\n", filePath);
        free(content);
        fclose(file);
        return NULL;
    }

    // Null-terminate the string
    content[fileSize] = '\0';

    fclose(file);
    return content;
}

char *get_token_type(TokenType type)
{
    const char *typeString;

    switch (type)
    {
    case None:
        typeString = "None";
        break;
    case BracketOpen:
        typeString = "BracketOpen";
        break;
    case BracketClose:
        typeString = "BracketClose";
        break;
    case BraceOpen:
        typeString = "BraceOpen";
        break;
    case BraceClose:
        typeString = "BraceClose";
        break;
    case ParenthesisOpen:
        typeString = "ParenthesisOpen";
        break;
    case ParenthesisClose:
        typeString = "ParenthesisClose";
        break;
    case Question:
        typeString = "Question";
        break;
    case Equals:
        typeString = "Equals";
        break;
    case Plus:
        typeString = "Plus";
        break;
    case ReturnArrow:
        typeString = "ReturnArrow";
        break;
    case Minus:
        typeString = "Minus";
        break;
    case Star:
        typeString = "Star";
        break;
    case Ampersand:
        typeString = "Ampersand";
        break;
    case ForwardSlash:
        typeString = "ForwardSlash";
        break;
    case BackSlash:
        typeString = "BackSlash";
        break;
    case Modulo:
        typeString = "Modulo";
        break;
    case ShiftRight:
        typeString = "ShiftRight";
        break;
    case ShiftLeft:
        typeString = "ShiftLeft";
        break;
    case SemiColon:
        typeString = "SemiColon";
        break;
    case GreaterThan:
        typeString = "GreaterThan";
        break;
    case MathPipe:
        typeString = "MathPipe";
        break;
    case Identifier:
        typeString = "Identifier";
        break;
    case MacroArrow:
        typeString = "MacroArrow";
        break;
    case LessThan:
        typeString = "LessThan";
        break;
    case GreaterOrEqual:
        typeString = "GreaterOrEqual";
        break;
    case LessOrEqual:
        typeString = "LessOrEqual";
        break;
    case Number:
        typeString = "Number";
        break;
    case Word:
        typeString = "Word";
        break;
    case Colon:
        typeString = "Colon";
        break;
    case Not:
        typeString = "Not";
        break;
    case While:
        typeString = "While";
        break;
    case ExtremeCase:
        typeString = "ExtremeCase";
        break;
    case FunctionKeword:
        typeString = "FunctionKeword";
        break;
    case For:
        typeString = "For";
        break;

    case AssignPlus:
        typeString = "AssignPlus";
        break;
    case AssignMinus:
        typeString = "AssignMinus";
        break;
    case AssignMultiply:
        typeString = "AssignMultiply";
        break;
    case AssignDivide:
        typeString = "AssignDivide";
        break;
    case AssignPow:
        typeString = "AssignPow";
        break;
    case AssignModulo:
        typeString = "AssignModulo";
        break;

    case VariableDefinition:
        typeString = "VariableDefinition";
        break;
    case Case:
        typeString = "Case";
        break;
    case If:
        typeString = "If";
        break;
    case Else:
        typeString = "Else";
        break;
    case Elif:
        typeString = "Elif";
        break;
    case NewLine:
        typeString = "NewLine";
        break;
    case Float:
        typeString = "Float";
        break;
    case And:
        typeString = "And";
        break;
    case Or:
        typeString = "Or";
        break;
    case Point:
        typeString = "Point";
        break; // we will consider a delimiter for now

    case UnsignedInteger8:
        typeString = "UnsignedInteger8";
        break;
    case UnsignedInteger16:
        typeString = "UnsignedInteger8";
        break;
    case UnsignedInteger32:
        typeString = "UnsignedInteger32";
        break;
    case UnsignedInteger64:
        typeString = "UnsignedInteger64";
        break;
    case UnsignedInteger128:
        typeString = "UnsignedInteger128";
        break;
    case SignedInteger8:
        typeString = "SignedInteger8";
        break;
    case SignedInteger16:
        typeString = "SignedInteger116";
        break;
    case SignedInteger32:
        typeString = "SignedInteger32";
        break;
    case SignedInteger64:
        typeString = "SignedInteger64";
        break;
    case SignedInteger128:
        typeString = "SignedInteger128";
        break;
    case Character:
        typeString = "Character";
        break;

    case DoubleEquals:
        typeString = "DoubleEquals";
        break;
    case NotEqual:
        typeString = "NotEqual";
        break;
    case True:
        typeString = "True";
        break;
    case False:
        typeString = "False";
        break;
    case String:
        typeString = "String";
        break;
    default:
        typeString = "Unknown";
        break;
    }
    return typeString;
}

void *ext_file_name(char *fn, char *filepath)
{
    if (filepath == NULL || *filepath == '\0')
    {
        return NULL; // Handle null or empty string input
    }

    const char *filename = strrchr(filepath, '/'); // Find the last occurrence of '/'
    if (filename == NULL)
    {
        filename = strrchr(filepath, '\\'); // If not found, find the last occurrence of '\'
    }

    if (filename != NULL)
    { // If found, move past the slash
        filename++;
    }
    else
    {
        filename = filepath; // If no slash or backslash found, use the entire filepath as filename
    }
    char buffer[1024];

    // Find the last occurrence of '.' to remove the existing extension
    char *ext = strrchr(filename, '.');
    if (ext != NULL)
    {
        *ext = '\0'; // Null-terminate the string at the '.' to remove the existing extension
    }

    // Add ".asm" extension

    if (strlen(filename) + 5 >= 1024)
    {
        fprintf(stderr, "Filename is too long\n");
        return NULL;
    }

    strcpy(buffer, filename); // Copy filename to buffer
    strcat(buffer, ".asm");   // Append ".asm" extension

    strcpy(fn, buffer);
}
void printTokens(Token **tokens, int numTokens)
{
    for (int i = 0; i < numTokens; i++)
    {
        Token *token = tokens[i];
        const char *typeString = get_token_type(token->type);

        printf("Type: %s,\t\t Value: %s,\t\t Line number: %d,\t\t position: %d\n",
               typeString, token->value, token->lineNumber, token->position);
    }
}

void free_tokens(Token **tokens, int numTokens)
{
    for (int i = 0; i < numTokens; i++)
    {
        free(tokens[i]->value);
        free(tokens[i]);
    }
}

int main(int argc, char const *argv[])
{

    char *filepath;
    int numLines;
    Content **fileContent;
    Node *ast = malloc(sizeof(Node));
    malloc_error(ast);
    ast->branch_count = 0;
    ast->line = 0;
    ast->start_position = 0;
    ast->type = ROOT;
    // ast->value = malloc(sizeof(char));
    ast->label = strdup("Root");

    ast->children = malloc(sizeof(Node *));

    ast->table = (Symbol *)malloc(sizeof(Symbol));
    if (ast->table == NULL)
    {
        fprintf(stderr, "Error: unexpected token type encountered\n");
        exit(1); // or handle the error appropriately
    }
    ast->nb_var = 0;
    int cursor = 0;
    char *filename;
    if (argc >= 2)
    {
        filepath = malloc(sizeof(strlen(argv[1]) + 4));
        strcpy(filepath, argv[1]);
        fileContent = read_file_content(filepath, &numLines);
        if (fileContent != NULL)
        {
            int num_tokens;
            Token **tokens = tokenize(fileContent, numLines, &num_tokens);
            parse(tokens, num_tokens, ast, &cursor);
            // print_ast(ast, 0);
            ext_file_name(filename, filepath);
            gen_asm(filename);

            //  printTokens(tokens, num_tokens);
            free(fileContent);
            free_tokens(tokens, num_tokens);
        }
        free(filepath);
    }

    else
    {
        printf("sorry, not enough arguments");
        exit(EXIT_FAILURE);
    }

    return 0;
}
