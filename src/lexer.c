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
    else if (strncmp(lexeme, "true", length) == 0)
    {
        return True;
    }
    else if (strncmp(lexeme, "false", length) == 0)
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
    case '*':
        return Star;
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

                if (input[i]->lineContent[index] == '-' && input[i]->lineContent[index + 1] == '>')
                {
                    type = MacroArrow;
                    buffer[bufferIndex++] = '>';
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

            if (IS_COMPARATOR(currentChar))
            {
                TokenType type;
                if (currentChar == '<')
                {
                    type = LessThan;
                }
                else if (currentChar = '>')
                {
                    type = GreaterThan;
                }
                buffer[bufferIndex++] = currentChar;

                if (input[i]->lineContent[index + 1] == '=')
                {
                    if (input[i]->lineContent[index] == '<')
                    {
                        type = LessOrEqual;
                    }
                    else if (input[i]->lineContent[index + 1] == '>')
                    {
                        type = GreaterOrEqual;
                    }
                    buffer[bufferIndex++] = '=';

                    index++;
                }
                // if (input[i]->lineContent[index] == '=' && input[i]->lineContent[index + 1] == '>')
                // {
                //     buffer[bufferIndex++] = '>';
                //     type = TOKEN_DEFAULT_KEY;
                //     index++;
                // }
                buffer[bufferIndex] = '\0'; // Null-terminate the buffer

                tokens = realloc(tokens, (*numTokens + 1) * sizeof(Token *));
                tokens[*numTokens] = createToken(type, buffer, i, index - bufferIndex + 1);
                (*numTokens)++;

                bufferIndex = 0;
                index++;
                continue;
            }

            // <= >= == !=
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
