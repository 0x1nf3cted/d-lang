

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>

typedef struct
{
    int lineNumber;
    char *lineContent;
} Content;

typedef enum
{
    None = 0, // the None token state exists only for lexer stage validation
              // purposes, it should never appear in the actual token stream
    BracketOpen,
    BracketClose,
    BraceOpen,
    // add the rest of the token types
    BraceClose,
    ParenthesisOpen,
    ParenthesisClose,
    Comma,
    Question,
    Equals,
    Plus,
    ReturnArrow,
    Minus,
    Star,
    Ampersand,
    ForwardSlash,
    BackSlash,
    Modulo,
    ShiftRight,
    ShiftLeft,
    SemiColon,
    GreaterThan,
    MathPipe,
    Identifier,
    MacroArrow,
    LessThan,
    GreaterOrEqual,
    LessOrEqual,
    Number,
    Word,
    Colon,
    Not,
    While,
    ExtremeCase,
    FunctionKeword,
    For,
    VariableDefinition,
    Case,
    If,
    Else,
    Elif,
    NewLine,
    Float,
    And,
    Or,
    DoubleEquals,
    NotEqual,
    True,
    False,
    String
} TokenType;

// typedef enum
// {
//     TOKEN_INTEGER,
//     TOKEN_FLOAT,
//     TOKEN_DEFAULT_KEY,
//     TOKEN_OPTION,
//     TOKEN_COMPARATOR,
//     TOKEN_IDENTIFIER,
//     TOKEN_OPERATOR,
//     TOKEN_KEYWORD,
//     TOKEN_DELIMITER,
//     TOKEN_UNKNOWN
// } TokenType;

typedef struct
{
    TokenType type;
    int lineNumber;
    int position;
    char *value;
} Token;

#define IS_DELIMITER(CH) ( \
    (CH) == '(' || (CH) == ')' || (CH) == '[' || (CH) == ']' || (CH) == '{' || (CH) == '}' || (CH) == ',' || (CH) == '|' ||(CH) == '*' || (CH) == '_' || (CH) == ';' || (CH) == ':' || (CH) == '&')

#define IS_OPERATOR(CH) ( \
    (CH) == '+' || (CH) == '-' || (CH) == '*' || (CH) == '/' || (CH) == '%')

#define IS_COMPARATOR(CH) ( \
    (CH) == '=' || (CH) == '<' || (CH) == '>' || (CH) == '!')

#include <string.h>

#define IS_KEYWORD(CH) (           \
    strcmp(CH, "get") == 0 ||      \
    strcmp(CH, "type") == 0 ||     \
    strcmp(CH, "fn") == 0 ||       \
    strcmp(CH, "with") == 0 ||     \
    strcmp(CH, "match") == 0 ||    \
    strcmp(CH, "if") == 0 ||       \
    strcmp(CH, "as") == 0 ||       \
    strcmp(CH, "match") == 0 ||    \
    strcmp(CH, "if") == 0 ||       \
    strcmp(CH, "else") == 0 ||     \
    strcmp(CH, "for") == 0 ||      \
    strcmp(CH, "while") == 0 ||    \
    strcmp(CH, "null") == 0 ||     \
    strcmp(CH, "of") == 0 ||       \
    strcmp(CH, "true") == 0 ||     \
    strcmp(CH, "false") == 0 ||    \
    strcmp(CH, "return") == 0 ||   \
    strcmp(CH, "continue") == 0 || \
    strcmp(CH, "none") == 0 ||     \
    strcmp(CH, "set") == 0)

#define FREE_TOKEN(token)     \
    do                        \
    {                         \
        free((token)->value); \
        free(token);          \
    } while (0)

#define FREE_LINES(line)           \
    do                             \
    {                              \
        free((line)->lineContent); \
        free(line);                \
    } while (0)

void freeTokens(Token **tokens, int numTokens);

void freeLines(Content **fileContent, int numLines);

void printTokens(Token **tokens, int numTokens);

Content *createNewLine(char *line, int lineNumber);
Content **read_file_content(const char *filename, int *lineNumber);
Token *createToken(TokenType type, const char *value, int lineNumber, int position);
Token **tokenize(Content **input, int numLines, int *numTokens);

#endif // LEXER_H
