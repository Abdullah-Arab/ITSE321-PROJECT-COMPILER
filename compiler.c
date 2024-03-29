#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define MAX_SYMBOLS 256

typedef struct Symbol
{
    int id;
    char name[256];
    char type[256];
    int size;
    int location;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

void insertSymbol(int id, char *name, char *type, int size, int location)
{
    if (symbolCount >= MAX_SYMBOLS)
    {
        printf("Symbol table full\n");
        return;
    }
    symbolTable[symbolCount].id = id;
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolTable[symbolCount].size = size;
    symbolTable[symbolCount].location = location;
    symbolCount++;
}

void printSymbolTable()
{
    printf("Symbol Table\n");
    printf("%-20s%-20s%-20s%-20s%-20s\n", "ID", "Name", "Type", "Size", "Location");
    for (int i = 0; i < symbolCount; i++)
    {
        printf("%-20d%-20s%-20s%-20d%-20d\n", symbolTable[i].id, symbolTable[i].name, symbolTable[i].type, symbolTable[i].size, symbolTable[i].location);
    }
}

Symbol *findSymbol(char *name)
{
    for (int i = 0; i < symbolCount; i++)
    {
        if (strcmp(symbolTable[i].name, name) == 0)
        {
            return &symbolTable[i];
        }
    }
    return NULL;
}

int alreadyDeclared(char *name)
{
    if (findSymbol(name) != NULL)
    {
        printf("Error: %s already declared\n", name);
        return 1;
    }
    return 0;
}

int getSymbolId(char *name)
{
    Symbol *symbol = findSymbol(name);
    if (symbol != NULL)
    {
        return symbol->id;
    }
    return -1;
}

int getLastSymbolId()
{
    return symbolTable[symbolCount - 1].id;
}

int isOperator(char *token)
{
    switch (*token)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case '>':
    case '<':
    case '&':
    case '|':
    case '!':
        return 1;
    default:
        return 0;
    }
}

int isSpecial(char *token)
{
    switch (*token)
    {
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case ';':
    case ',':
    case '.':
        return 1;
    default:
        return 0;
    }
}

int main()
{

    // Declare the files
    FILE *fileSrc;
    FILE *tokensDest;
    FILE *cleanDest;
    FILE *logDest;

    // Declare Keywords
    char keywords[32][10] = {"int", "float", "char", "if", "else", "while", "for", "return", "printf", "scanf", "main", "string"};

    // Open the files
    fileSrc = fopen("source.txt", "r+");
    if (fileSrc == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }

    tokensDest = fopen("output/tokens.txt", "w+");
    if (tokensDest == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }

    cleanDest = fopen("output/clean.txt", "w+");
    if (cleanDest == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }

    logDest = fopen("output/log.txt", "w+");
    if (logDest == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    int line = 1;
    // get source file char by char
    char c = fgetc(fileSrc);
    int id = 0;
    while (1)
    {

        // skip whitespace
        while (isspace(c))
        {
            c = fgetc(fileSrc);
            if (c == '\n')
            {
                line++;
                printf("\033[0;31mError: Unknown character (\\n)\n");
                c = fgetc(fileSrc);
            }
        }

        // skip comments
        if (c == '/')
        {
            c = fgetc(fileSrc);
            if (c == '/')
            {
                while (c != '\n')
                {
                    c = fgetc(fileSrc);
                }
                line++;
            }
            else if (c == '*')
            {
                c = fgetc(fileSrc);
                while (1)
                {
                    if (c == '*')
                    {
                        c = fgetc(fileSrc);
                        if (c == '/')
                        {
                            break;
                        }
                    }
                    c = fgetc(fileSrc);
                }
                c = fgetc(fileSrc);
            }
        }

        // if end of file, break
        if (feof(fileSrc))
        {
            break;
        }

        // Tokenize the source file
        char token[256];
        int tokenIndex = 0;

        // check if the char is a letter
        if (isalpha(c))
        {
            // get the whole word
            while (isalnum(c) || c == '_')
            {
                token[tokenIndex++] = c;
                c = fgetc(fileSrc);
            }
            if (c == '\n')
            {
                line++;
            }

            token[tokenIndex] = '\0';

            // check if the word is a keyword
            int isKeyword = 0;
            for (int i = 0; i < 32; i++)
            {
                if (strcmp(token, keywords[i]) == 0)
                {
                    isKeyword = 1;
                    break;
                }
            }

            // if it's a keyword
            if (isKeyword)
            {
                fprintf(tokensDest, "<KEYWORD, %s> ", token);
                fprintf(cleanDest, "%s", token);
            }

            else
            {
                // if it's not a keyword, it's an identifier
                // check if the identifier is already declared
                if (alreadyDeclared(token))
                {
                    fprintf(logDest, "Info: %s already declared\n", token);
                }
                else
                {
                    // if it's not declared, add it to the symbol table
                    id = getLastSymbolId() + 1;
                    insertSymbol(id, token, "identifier", 1, line);
                }
                int tokenId = getSymbolId(token);
                fprintf(tokensDest, "<ID, %d> ", tokenId);
                fprintf(cleanDest, "%s", token);
            }
        }
        else if (isdigit(c))
        {
            // get the whole number
            while (isdigit(c))
            {
                token[tokenIndex++] = c;
                c = fgetc(fileSrc);
            }

            token[tokenIndex] = '\0';
            fprintf(tokensDest, "<CONST, %s> ", token);
            fprintf(cleanDest, "%s", token);
        }
        else if (c == '"')
        {
            // get the whole string
            token[tokenIndex++] = c;
            c = fgetc(fileSrc);
            while (c != '"')
            {
                token[tokenIndex++] = c;
                c = fgetc(fileSrc);
            }
            token[tokenIndex++] = c;
            token[tokenIndex] = '\0';
            fprintf(tokensDest, "<STRING, %s> ", token);
            fprintf(cleanDest, "%s", token);
            c = fgetc(fileSrc);
        }
        else if (c == '\'')
        {
            // get the whole char
            token[tokenIndex++] = c;
            c = fgetc(fileSrc);
            while (c != '\'')
            {
                token[tokenIndex++] = c;
                c = fgetc(fileSrc);
            }
            token[tokenIndex++] = c;
            token[tokenIndex] = '\0';
            fprintf(tokensDest, "<CHAR, %s> ", token);
            fprintf(cleanDest, "%s", token);
            c = fgetc(fileSrc);
        }
        else if (isOperator(&c))
        {
            // get the whole operator
            while (isOperator(&c))
            {
                token[tokenIndex++] = c;
                c = fgetc(fileSrc);
            }
            token[tokenIndex] = '\0';
            fprintf(tokensDest, "<%s> ", token);
            fprintf(cleanDest, "%s", token);
        }
        else if (c == '\n')
        {
            line++;
            printf("\033[0;31mError: Unknown character (\\n)\n");
            c = fgetc(fileSrc);
        }
        else
        {
            // if it's any other special character
            if (isSpecial(&c))
            {
                token[tokenIndex++] = c;
                token[tokenIndex] = '\0';
                fprintf(tokensDest, "<%s> ", token);
                fprintf(cleanDest, "%s", token);
                c = fgetc(fileSrc);
                if (c == '\n')
                {
                    line++;
                }
            }
            else
            {
                fprintf(logDest, "Error: Unknown character (%c)\n", c);
                switch (c)
                {
                case '\0':
                    printf("\033[0;31mError: Unknown character (\\0)\n");
                    fprintf(logDest, "Error: Unknown character (\\0)\n");
                    break;
                case '\n':
                    line++;
                    printf("\033[0;31mError: Unknown character (\\n)\n");
                    fprintf(logDest, "Error: Unknown character (\\n)\n");
                    break;
                default:
                    printf("\033[0;31mError: Unknown character (%c)\n", c);
                    fprintf(logDest, "Error: Unknown character (%c)\n", c);
                }
            }
        }
    }

    printSymbolTable();
    fprintf(logDest, "Compilation Successful\n");
    fprintf(logDest, "----------------------------\n");
    fprintf(logDest, "Symbol Table\n");
    fprintf(logDest, "%-10s%-10s%-10s%-10s%-10s\n", "ID", "Name", "Type", "Size", "Location");
    for (int i = 0; i < symbolCount; i++)
    {
        fprintf(logDest, "%-10d%-10s%-10s%-10d%-10d\n", symbolTable[i].id, symbolTable[i].name, symbolTable[i].type, symbolTable[i].size, symbolTable[i].location);
        // fprintf(logDest, "%d\t%s\t%s\t%d\t%d\n", symbolTable[i].id, symbolTable[i].name, symbolTable[i].type, symbolTable[i].size, symbolTable[i].location);
    }
    fprintf(logDest, "----------------------------\n");

    // show date and time
    fprintf(logDest, "Date: %s\n", __DATE__);
    fprintf(logDest, "Time: %s\n", __TIME__);
    fprintf(logDest, "Compiled by: Abdullah Arab ( 2201805579 ) \n");
    // show compilation duration
    fprintf(logDest, "compiled in %f seconds\n", (double)clock() / CLOCKS_PER_SEC);
    printf("\033[0;32mCompilation Successful in \033[1;32m%f\n", (double)clock() / CLOCKS_PER_SEC);
    printf("By DaDev 2201805579\n");

    // close file
    fclose(logDest);
    fclose(cleanDest);
    fclose(tokensDest);
    fclose(fileSrc);

    return 0;
}