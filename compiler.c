#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define MAX_SYMBOLS 256

typedef struct Symbol
{
    char name[256];
    char type[256];
    int size;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

void insertSymbol(char *name, char *type, int size)
{
    if (symbolCount >= MAX_SYMBOLS)
    {
        printf("Symbol table full\n");
        return;
    }
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolTable[symbolCount].size = size;
    symbolCount++;
}

void printSymbolTable()
{
    printf("Symbol Table\n");
    printf("Name\tType\tSize\n");
    for (int i = 0; i < symbolCount; i++)
    {
        printf("%s\t%s\t%d\n", symbolTable[i].name, symbolTable[i].type, symbolTable[i].size);
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

int isOperator(char *token)
{
    char operators[32][10] = {"+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!"};
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(token, operators[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
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

    // Declare special characters
    char specialChars[32][10] = {"(", ")", "{", "}", "[", "]", ";", ":", ",", ".", "\\"};

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

    // get source file char by char
    char c = fgetc(fileSrc);
    int id = 0;
    while (1)
    {

        // skip whitespace
        while (isspace(c))
        {
            c = fgetc(fileSrc);
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
                fprintf(tokensDest, "<keyword, %s>\n", token);
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
                    insertSymbol(token, "identifier", 1);
                    id++;
                }
                fprintf(tokensDest, "<id, %d>\n", id);
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
            fprintf(tokensDest, "<number, %s>\n", token);
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
            fprintf(tokensDest, "<string, %s>\n", token);
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
            fprintf(tokensDest, "<char, %s>\n", token);
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
            fprintf(tokensDest, "<operator, %s>\n", token);
        }
        else
        {
            // check if the char is a special character
            int isSpecial = 0;
            for (int i = 0; i < 32; i++)
            {
                if (c == specialChars[i][0])
                {
                    isSpecial = 1;
                    break;
                }
            }

            // if it's a special character
            if (isSpecial)
            {
                fprintf(tokensDest, "<special, %c>\n", c);
                c = fgetc(fileSrc);
            }
            else
            {
                // if it's not a special character, it's an error
                fprintf(logDest, "Error: Unknown character %c\n", c);
                c = fgetc(fileSrc);
            }
        }

        // Remove all the white spaces from the source file
        fprintf(cleanDest, "%c", c);
        c = fgetc(fileSrc);
    }
    printSymbolTable();

    fprintf(logDest, "Compilation Successful\n");
    fprintf(logDest, "----------------------------\n");
    fprintf(logDest, "Symbol Table\n");
    fprintf(logDest, "Name\tType\tSize\n");
    for (int i = 0; i < symbolCount; i++)
    {
        fprintf(logDest, "%s\t%s\t%d\n", symbolTable[i].name, symbolTable[i].type, symbolTable[i].size);
    }
    fprintf(logDest, "----------------------------\n");

    // show date and time
    fprintf(logDest, "Date: %s\n", __DATE__);
    fprintf(logDest, "Time: %s\n", __TIME__);
    fprintf(logDest, "Compiled by: Abdullah Arab ( 2201805579 ) \n");
    // show compilation duration
    fprintf(logDest, "compiled in %f seconds\n", (double)clock() / CLOCKS_PER_SEC);
    printf("\033[0;32mCompilation Successful in \033[1;32m%f\n", (double)clock() / CLOCKS_PER_SEC);

    // close file
    fclose(logDest);
    fclose(cleanDest);
    fclose(tokensDest);
    fclose(fileSrc);

    return 0;
}