
#define _GNU_SOURCE

#include "lexer.h"

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

void printTokens(Token **tokens, int numTokens)
{
    for (int i = 0; i < numTokens; i++)
    {
        Token *token = tokens[i];
        const char *typeString;

        switch (token->type)
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

        printf("Type: %s,\t\t Value: %s,\t\t Line number: %d,\t\t position: %d\n",
               typeString, token->value, token->lineNumber, token->position);
    }
}

int main(int argc, char const *argv[])
{

    char *filepath;
    int numLines;
    Content **fileContent;
    if (argc >= 2)
    {
        filepath = malloc(sizeof(strlen(argv[1]) + 4));
        strcpy(filepath, argv[1]);
        fileContent = read_file_content(filepath, &numLines);
        free(filepath);
        if (fileContent != NULL)
        {
            int numTokens;
            Token **tokens = tokenize(fileContent, numLines, &numTokens);
            printTokens(tokens, numTokens);
            free(fileContent);
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
