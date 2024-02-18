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

int main()
{

    // Declare the files
    FILE *fileSrc;
    FILE *tokensDest;
    FILE *cleanDest;
    FILE *logDest;

    // Declare Keywords
    char keywords[32][10] = {"int", "float", "char", "if", "else", "while", "for", "return", "printf", "scanf", "main"};

    // Declare operators
    char operators[32][10] = {"+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!"};

    // Declare special characters
    char specialChars[32][10] = {"(", ")", "{", "}", "[", "]", ";", ":", ",", ".", "\"", "\'", "\\"};

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