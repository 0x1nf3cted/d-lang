#define _GNU_SOURCE
#include "lexer.h"

Content *createNewLine(char *line, int lineNumber)
{
    Content *newLine = malloc(sizeof(Content));
    if (newLine == NULL)
    {
        perror("Memory allocation failed");
        free(newLine);
        return NULL;
    }
    newLine->lineContent = strdup(line);
    newLine->lineNumber = lineNumber;
    return newLine;
}

Content **read_file_content(const char *filename, int *lineNumber)
{
    Content **fileContent = NULL;
    FILE *file = fopen(filename, "r");
    size_t lineLen = 0;
    *lineNumber = 0;
    char *lineBuffer = NULL;
    ssize_t read;

    if (file == NULL)
    {
        perror("Failed to open file");
        return NULL;
    }

    while ((read = getline(&lineBuffer, &lineLen, file)) != -1)
    {
        *lineNumber = *lineNumber + 1;
        Content *lineContent = createNewLine(lineBuffer, *lineNumber);
        fileContent = realloc(fileContent, (*lineNumber) * sizeof(Content *));
        if (fileContent == NULL)
        {
            perror("Memory allocation failed");
            // Cleanup before returning NULL
            free(lineBuffer);
            fclose(file);
            for (int i = 0; i < *lineNumber - 1; i++)
            {
                free(fileContent[i]->lineContent);
                free(fileContent[i]);
            }
            free(fileContent);
            return NULL;
        }

        fileContent[*lineNumber - 1] = lineContent;
    }

    fclose(file);
    free(lineBuffer);

    return fileContent;
}

Token *createToken(TokenType type, const char *value, int lineNumber, int position)
{
    Token *token = malloc(sizeof(Token));
    token->lineNumber = lineNumber;
    token->position = position;
    token->type = type;
    token->value = strdup(value);
    return token;
}
TokenType get_token_kind(char *lexeme, uint16_t length)
{
    if (strncmp(lexeme, "while", length) == 0)
    {
        return While;
    }
    else if (strncmp(lexeme, "set", length) == 0)
    {
        return VariableDefinition;
    }
    else
    {
        return Word;
    }
}

TokenType get_token_datatype(char *lexeme, uint16_t length)
{
    if (strcmp(lexeme, "char") == 0)
    {
        return Character;
    }
    else if (strcmp(lexeme, "str") == 0)
    {
        return String;
    }
    else if (strcmp(lexeme, "u8") == 0)
    {
        return UnsignedInteger8;
    }
    else if (strcmp(lexeme, "u16") == 0)
    {
        return UnsignedInteger16;
    }
    else if (strcmp(lexeme, "u32") == 0)
    {
        return UnsignedInteger32;
    }
    else if (strcmp(lexeme, "u64") == 0)
    {
        return UnsignedInteger64;
    }
    else if (strcmp(lexeme, "u128") == 0)
    {
        return UnsignedInteger128;
    }
    else if (strcmp(lexeme, "i8") == 0)
    {
        return SignedInteger8;
    }
    else if (strcmp(lexeme, "i16") == 0)
    {
        return SignedInteger16;
    }
    else if (strcmp(lexeme, "i32") == 0)
    {
        return SignedInteger32;
    }
    else if (strcmp(lexeme, "i64") == 0)
    {
        return SignedInteger64;
    }
    else if (strcmp(lexeme, "i128") == 0)
    {
        return SignedInteger128;
    }
    else if (strcmp(lexeme, "true") == 0)
    {
        return True;
    }
    else if (strcmp(lexeme, "false") == 0)
    {
        return False;
    }
    else
    {
        return Word;
    }
}

TokenType get_delimiter_type(char currentChar)
{
    switch (currentChar)
    {
    case '{':
        return BraceOpen;
        break;
    case '}':
        return BraceClose;
        break;
    case '(':
        return ParenthesisOpen;
        break;
    case ')':
        return ParenthesisClose;
        break;
    case '[':
        return BracketOpen;
        break;
    case ':':
        return Colon;
        break;
    case ';':
        return SemiColon;
        break;
    case '?':
        return Question;
        break;
    case ',':
        return Comma;
        break;

    case '.':
        return Point;
        break;
    case '!':
        return Not;
        break;
    case '&':
        return Ampersand;
        break;
    case '|':
        return MathPipe;
        break;
    case ']':
        return BracketClose;
        break;
    }
}

TokenType get_operator_type(char lexeme)
{
    switch (lexeme)
    {
    case '*':
        return Star;
    case '+':
        return Plus;
    case '-':
        return Minus;
    case '%':
        return Ampersand;
    case '=':
        return Equals;
    default:
        return None;
    }
}

Token **tokenize(Content **input, int numLines, int *numTokens)
{
    Token **tokens = NULL;
    *numTokens = 0;

    char buffer[256];
    int bufferIndex = 0;

    for (int i = 0; i < numLines; i++)
    {
        int index = 0;

        while (input[i]->lineContent[index] != '\0')
        {
            char currentChar = input[i]->lineContent[index];

            if (isspace(currentChar))
            {
                index++;
                continue;
            }

            if (IS_DELIMITER(currentChar))
            {
                TokenType type = get_delimiter_type(currentChar);

                buffer[bufferIndex] = currentChar;
                buffer[bufferIndex + 1] = '\0'; // Null-terminate the buffer
                tokens = realloc(tokens, (*numTokens + 1) * sizeof(Token *));
                tokens[*numTokens] = createToken(type, buffer, i, index + 1);
                (*numTokens)++;
                bufferIndex = 0;
                index++;
                continue;
            }
            if (IS_OPERATOR(currentChar))
            {
                TokenType type = get_operator_type(currentChar);

                buffer[bufferIndex++] = currentChar;

                // Check for compound assignment operators
                if (input[i]->lineContent[index] == '=' && input[i]->lineContent[index + 1] == '=')
                {
                    type = DoubleEquals;
                    buffer[bufferIndex++] = '=';
                    index++;
                }
                else if (input[i]->lineContent[index] == '-' && input[i]->lineContent[index + 1] == '>')
                {
                    type = MacroArrow;
                    buffer[bufferIndex++] = '>';
                    index++;
                }
                else
                {
                    switch (input[i]->lineContent[index])
                    {
                    case '-':
                        if (input[i]->lineContent[index + 1] == '=')
                        {
                            type = AssignMinus;
                            buffer[bufferIndex++] = '=';
                            index++;
                        }
                        break;
                    case '+':
                        if (input[i]->lineContent[index + 1] == '=')
                        {
                            type = AssignPlus;
                            buffer[bufferIndex++] = '=';
                            index++;
                        }
                        break;
                    case '*':
                        if (input[i]->lineContent[index + 1] == '=')
                        {
                            type = AssignMultiply;
                            buffer[bufferIndex++] = '=';
                            index++;
                        }
                        else if (input[i]->lineContent[index + 1] == '*')
                        {
                            buffer[bufferIndex++] = '*';
                            index++;
                            if (input[i]->lineContent[index + 1] == '=')
                            {
                                type = AssignPow;
                                buffer[bufferIndex++] = '=';
                                index++;
                            }
                            else
                            {
                                type = Power;
                            }
                        }
                        else
                        {
                            type = Star;
                        }
                        break;
                    case '/':
                        if (input[i]->lineContent[index + 1] == '=')
                        {
                            type = AssignDivide;
                            buffer[bufferIndex++] = '=';
                            index++;
                        }
                        break;
                    case '%':
                        if (input[i]->lineContent[index + 1] == '=')
                        {
                            type = AssignModulo;
                            buffer[bufferIndex++] = '=';
                            index++;
                        }
                        break;
                    default:
                        break;
                    }
                }

                buffer[bufferIndex] = '\0'; // Null-terminate the buffer

                tokens = realloc(tokens, (*numTokens + 1) * sizeof(Token *));
                tokens[*numTokens] = createToken(type, buffer, i, index - bufferIndex + 1);
                (*numTokens)++;

                bufferIndex = 0;
                index++;
                continue;
            }

            if (IS_COMPARATOR(currentChar))
            {
                TokenType type;
                if (currentChar == '<')
                {
                    if (input[i]->lineContent[index] == '=')
                    {
                        type = LessOrEqual;
                    }
                    else
                    {
                        type = LessThan;
                    }
                }
                else if (currentChar = '>')
                {
                    if (input[i]->lineContent[index] == '=')
                    {
                        type = GreaterOrEqual;
                    }
                    else
                    {
                        type = GreaterThan;
                    }
                }
                buffer[bufferIndex++] = currentChar;
                if (input[i]->lineContent[index] == '!' && input[i]->lineContent[index + 1] == '=')
                {
                    type = NotEqual;
                    buffer[bufferIndex++] = '=';
                    index++;
                }

                buffer[bufferIndex] = '\0'; // Null-terminate the buffer

                tokens = realloc(tokens, (*numTokens + 1) * sizeof(Token *));
                tokens[*numTokens] = createToken(type, buffer, i, index - bufferIndex + 1);
                (*numTokens)++;

                bufferIndex = 0;
                index++;
                continue;
            }

            if (isalpha(currentChar))
            {
                while (isalnum(input[i]->lineContent[index]) || input[i]->lineContent[index] == '_')
                {
                    buffer[bufferIndex++] = input[i]->lineContent[index++];
                }
                buffer[bufferIndex] = '\0';

                TokenType type = Identifier;
                if (IS_KEYWORD(buffer))
                {
                    type = get_token_kind(buffer, bufferIndex + 1);
                }
                else if (IS_DATATYPE(buffer))
                {
                    type = get_token_datatype(buffer, bufferIndex + 1);
                }
                tokens = realloc(tokens, (*numTokens + 1) * sizeof(Token *));
                tokens[*numTokens] = createToken(type, buffer, i, index - bufferIndex + 1);
                (*numTokens)++;
                bufferIndex = 0;
                continue;
            }

            if (isdigit(currentChar))
            {
                while (isdigit(input[i]->lineContent[index]) || input[i]->lineContent[index] == '.')
                {
                    buffer[bufferIndex++] = input[i]->lineContent[index++];
                }
                buffer[bufferIndex] = '\0';

                TokenType type = Number;
                for (int j = 0; j < bufferIndex; j++)
                {
                    if (buffer[j] == '.')
                    {
                        type = Float;
                        break;
                    }
                }

                tokens = realloc(tokens, (*numTokens + 1) * sizeof(Token *));
                tokens[*numTokens] = createToken(type, buffer, i, index - bufferIndex + 1);
                (*numTokens)++;
                bufferIndex = 0;
                continue;
            }

            buffer[bufferIndex++] = currentChar;
            buffer[bufferIndex] = '\0';
            tokens = realloc(tokens, (*numTokens + 1) * sizeof(Token *));
            tokens[*numTokens] = createToken(Identifier, buffer, i, index + 1);
            (*numTokens)++;
            bufferIndex = 0;
            index++;
        }
    }

    return tokens;
}
