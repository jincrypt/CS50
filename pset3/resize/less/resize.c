// Problem Set 3
// Resizes image by integer between 1 to 100
// Angus Leung
// July 20, 19

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4 || atoi(argv[1]) > 100 || atoi(argv[1]) < 1)
    {
        fprintf(stderr, "Usage: resize (size [0 < n < 100] ; infile ; outfile)\n");
        return 1;
    }

    // remember filenames & integer
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, rbf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    rbf = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, rbi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    rbi = bi;

    // Adjust resized height and width
    rbi.biHeight = bi.biHeight * n;
    rbi.biWidth = bi.biWidth * n;

    // determine padding for scanlines. Padding is always constant on each line.
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int rpadding = (4 - (rbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // biWidth is in units of pixels. so it's pixels * RGB (3) + end padding for the width of the image.
    rbi.biSizeImage = ((rbi.biWidth * sizeof(RGBTRIPLE) + rpadding) * abs(rbi.biHeight));
    rbf.bfSize = rbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&rbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&rbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines [original file's, so we don't go into null space]
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // for height
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < bi.biWidth; k++)
            {
                // iterate over pixels in scanline
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int l = 0; l < n; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // add padding
            for (int m = 0; m < rpadding; m++)
            {
                fputc(0x00, outptr);
            }

            // on input file, return back to the beginning of the line in order to have a correct height for next "j" loop
            if (j < n - 1)
            {
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }

        }
        // skip over padding from reading of input file
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
