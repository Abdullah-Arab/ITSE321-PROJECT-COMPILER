#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
    FILE *fileDest;

    // Declare Keywords
    char keywords[32][10] = {"int", "float", "char", "if", "else", "while", "for", "return", "printf", "scanf", "main"};

    //Declare operators
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

    fileDest = fopen("output/output.txt", "w+");
    if (fileDest == NULL)
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

        // if c is a letter
        if (isalpha(c))
        {
            char word[256];
            int i = 0;
            while (isalpha(c))
            {
                word[i] = c;
                i++;
                c = fgetc(fileSrc);
            }
            word[i] = '\0';
            // printf("%s\n", word);
            // check if word is a keyword
            int isKeyword = 0;
            for (int i = 0; i < 32; i++)
            {
                if (strcmp(word, keywords[i]) == 0)
                {
                    // printf("Keyword: %s\n", word);
                    // fputc(c, fileDest);
                    if (alreadyDeclared(word))
                    {
                        break;
                    }
                    insertSymbol(word, "keyword", sizeof(word));
                    fprintf(fileDest, "Keyword: %s\n", word);
                    isKeyword = 1;
                    break;
                }
            }

            // printf("Identifier: %s\n", word);
            // fputc(c, fileDest);
            if (alreadyDeclared(word))
            {
                // break;
            }
            else
            {
                insertSymbol(word, "/", sizeof(word));
            }

            fprintf(fileDest, "<id%d, ",id);
         

            // printf("%c", c);
            // fputc(c, fileDest);
        }

        // if punctuation
        if (ispunct(c))
        {
            char word[256];
            int i = 0;
            while (ispunct(c))
            {
                word[i] = c;
                i++;
                c = fgetc(fileSrc);
            }
            word[i] = '\0';

            // printf("Identifier: %s\n", word);
            // fputc(c, fileDest);
            if (alreadyDeclared(word))
            {
                break;
            }
            insertSymbol(word, "operator", sizeof(word));
            fprintf(fileDest, "operator: %s>\n", word);

            // printf("%c", c);
            // fputc(c, fileDest);
        }
           id++;
    }
    printSymbolTable();

    // close file
    fclose(fileDest);
    fclose(fileSrc);

    return 0;
}