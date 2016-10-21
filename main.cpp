//main.cpp
//Contains code for getting files and running the emulator


#include "8080Core.h"

int main()
{
    FILE *file = fopen("invaders.h", "r");
    if(file == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    
    fseek(file, 0, SEEK_END);
    int fsize = ftell(file);
    
    unsigned char *buffer = (unsigned char*)malloc(fsize);
    fseek(file, 0, SEEK_SET);
    fread(buffer, 1, fsize, file);
    fclose(file);
    int programCounter = 0;
    printf("Beginning Program");
    while(programCounter < fsize)
    {
        programCounter += Disassemble(buffer, programCounter);
                          
    }
    
    
    return 0;
}