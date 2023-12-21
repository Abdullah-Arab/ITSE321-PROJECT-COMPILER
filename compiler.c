#include<stdio.h>


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
      while (1) {
        char c = fgetc(fileSrc);
        if (feof(fileSrc)) {
            break;
        }
        printf("%c", c);
        fputc(c, fileDest);
      }

    


    

    fclose(fileDest);
    fclose(fileSrc);

    return 0;

}