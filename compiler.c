#include<stdio.h>
#include <ctype.h>


int main(){
    FILE *fileSrc;
    FILE *fileDest;

    // r = read
    // w = write
    // a = append
    // r+ = read and write
    // w+ = readfileSrc and write (overwrite file)
    // a+ = read and write (append if file exists)

    fileSrc = fopen("source.txt", "r+");
    if (fileSrc == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    fileDest = fopen("output/output.txt", "w+");
    if (fileDest == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
      // Read the file into fileSrcContent
      // should be char by char
      // fgetc
        char c = fgetc(fileSrc);
        char token[256];
        int i = 0;

      while (1) {
        // skip whitespace
        while(isspace(c)){
            c = fgetc(fileSrc);
            if (feof(fileSrc)) {
            break;
        }
        }
        
        // if end of file, break
        if (feof(fileSrc)) {
            break;
        }

        // if the charachter is a letter, it's the start of a token ( identifier or keyword )
        if (isalpha(c)) {
            do{
                token[i++] = c;
                c = fgetc(fileSrc);
            } while (isalnum(c) && !feof(fileSrc));

            token[i] = '\0';
            printf("Token: %s\n", token);
            i = 0;
            
        }else{
           // If the character is not a letter, just print it and move on
        printf("%c", c);
        fputc(c, fileDest);
        c = fgetc(fileSrc);
        }
        
        

        // printf("%c", c);
        // fputc(c, fileDest);
      }

    


    

    fclose(fileDest);
    fclose(fileSrc);

    return 0;

}