#include<stdio.h>
#define MAX_FILE_SIZE 1000 // Define the maximum file size


int main(){
    FILE *fileSrc;
    FILE *fileDest;
    char fileSrcContent[MAX_FILE_SIZE];
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
      // Read the file into fileSrcContent
    fread(fileSrcContent, sizeof(char), MAX_FILE_SIZE, fileSrc);


    fileDest = fopen("output/destination.txt", "w+");
    if (fileDest == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(fileDest, "%s", fileSrcContent);
    fclose(fileDest);
    fclose(fileSrc);

    return 0;

}