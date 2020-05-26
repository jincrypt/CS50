// Problem Set 4
// Angus Leung
// April 30, 2020

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    char *infile = argv[1];   
    int jpgcount = 0;
    FILE *outptr = NULL;
    FILE *inptr = fopen(infile, "r");
    
    if (inptr == NULL)
    {
        printf("File cannot be opened\n");
        return 1;
    }
    
    unsigned char *buffer = malloc(512);
    
    // Looping continuously while we can read 512 bytes
    while (fread(buffer, 512, 1, inptr))
    {
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            if (outptr != NULL)
            {
                fclose(outptr);
                jpgcount++;
            }
            
            char filename[8];
            sprintf(filename, "%03i.jpg", jpgcount);
            outptr = fopen(filename, "w");
        }
        
        // If outptr exists, write the buffer to the file (makes sure a file is opened)
        if (outptr != NULL)
        {
            fwrite(buffer, 512, 1, outptr);
        }
    }
    
    fclose(inptr);
    fclose(outptr);
    free(buffer);
    return 0;
}
