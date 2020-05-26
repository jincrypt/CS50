// Problem Set 3 CS 50
// Angus Leung
// August 6, 2019

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Confirm correct input
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover image\n");
        return 1;
    }

    // Declare Input file & Confirms if file can be opened
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    unsigned char *buffer = malloc(512);
    int jpgcount = 0;
    FILE *outptr = NULL;

    // Loop while we can read full 512 byte buffers
    while (fread(buffer, 512, 1, inptr))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If beginning of JPEG is found, & a file is already open, then close and increase count
            if (outptr != NULL)
            {
                fclose(outptr);
                jpgcount++;
            }
            // File names for JPEG
            char filename[8];
            sprintf(filename, "%03i.jpg", jpgcount);
            outptr = fopen(filename, "w");

        }

        // Writes only if output is a JPEG file
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
