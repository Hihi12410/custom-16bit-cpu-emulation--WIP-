#include "stdio.h"
#include "stdlib.h"

int main(int argc, char ** argv) 
{
    if (argc < 3) 
    {
        perror("\nPlease specify a source/output file!\n");
        exit(EXIT_FAILURE);
    }

    FILE * src = fopen(argv[1], "r");
    if (src == NULL) 
    {
        perror("\nPlease specify a valid source file!\n");
        exit(EXIT_FAILURE);
    }

    FILE * dest = fopen(argv[2],"w");
    if (dest == NULL) 
    {
        perror("\nPlease specify a valid outputfile!\n");
        fclose(src);
        exit(EXIT_FAILURE);
    }

    fseek(src, 0, SEEK_END);
    unsigned long long flen = ftell(src);
    fseek(src, 0, SEEK_SET);

    char * buffer = malloc(flen * sizeof(char));
    if (buffer == NULL) 
    {
        perror("Error allocating memory");
        fclose(src);
        fclose(dest);
        exit(EXIT_FAILURE);
    }

    unsigned long bytesRead = fread(buffer, sizeof(char), flen ,src);
    if (bytesRead != flen) 
    {
        perror("Error reading file");
        free(buffer);
        fclose(src);
        fclose(dest);
        exit(EXIT_FAILURE);
    }
    

    free(buffer);
    fclose(src);
    fclose(dest);
    return 0;
}