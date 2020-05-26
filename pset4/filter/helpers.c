// Problem Set 4
// Angus Leung
// April 29, 2020
// Filter

#include "helpers.h"
#include <math.h>

// RGBTRIPLE.rgbtBlue, rgbtGreen, rgbtRED
/// image[2][3].rgbtRed=0

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average;
            average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        // Loop to copy image into a temporary field
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
        // Copy temp back into image backwards for this row
        for (int k = width - 1; k >= 0; k--)
        {
            image[i][width - 1 - k] = temp[i][k];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
        
    double BlueGx, BlueGy, GreenGx, GreenGy, RedGx, RedGy;

    RGBTRIPLE temp[height][width];
    
    for (int i = 0; i < height; i++)
    {
        // Loop to copy image into a temporary field
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BlueGx = 0;
            BlueGy = 0;
            GreenGx = 0;
            GreenGy = 0;
            RedGx = 0;
            RedGy = 0;
            
            // Add right pixel if it exists (x)
            if (j < (width - 1))
            {
                BlueGx = BlueGx + (temp[i][j + 1].rgbtBlue * 2);
                GreenGx = GreenGx + (temp[i][j + 1].rgbtGreen * 2);
                RedGx = RedGx + (temp[i][j + 1].rgbtRed * 2);
            }
            
            // Add left pixel if it exists (x)
            if (j > 0)
            {
                BlueGx = BlueGx + (temp[i][j - 1].rgbtBlue * -2);
                GreenGx = GreenGx + (temp[i][j - 1].rgbtGreen * -2);
                RedGx = RedGx + (temp[i][j - 1].rgbtRed * -2);
            }
            
            // Add pixels above if it exists (y)
            if (i > 0)
            {
                BlueGy = BlueGy + (temp[i - 1][j].rgbtBlue * -2);
                GreenGy = GreenGy + (temp[i - 1][j].rgbtGreen * -2);
                RedGy = RedGy + (temp[i - 1][j].rgbtRed * -2);
            }
            
            // Add pixels below if it exists (y)
            if (i < (height - 1))
            {
                BlueGy = BlueGy + (temp[i + 1][j].rgbtBlue * 2);
                GreenGy = GreenGy + (temp[i + 1][j].rgbtGreen * 2);
                RedGy = RedGy + (temp[i + 1][j].rgbtRed * 2);
            }
            
            // add Top left (x * -1, y * -1)
            if ((i > 0) && (j > 0))
            {
                BlueGx = BlueGx + (temp[i - 1][j - 1].rgbtBlue * -1);
                BlueGy = BlueGy + (temp[i - 1][j - 1].rgbtBlue * -1);
                GreenGx = GreenGx + (temp[i - 1][j - 1].rgbtGreen * -1);
                GreenGy = GreenGy + (temp[i - 1][j - 1].rgbtGreen * -1);
                RedGx = RedGx + (temp[i - 1][j - 1].rgbtRed * -1);
                RedGy = RedGy + (temp[i - 1][j - 1].rgbtRed * -1);
            }
            
            // add Top Right (x * 1, y * -1)
            if ((i > 0) && (j < (width - 1)))
            {
                BlueGx = BlueGx + (temp[i - 1][j + 1].rgbtBlue * 1);
                BlueGy = BlueGy + (temp[i - 1][j + 1].rgbtBlue * -1);
                GreenGx = GreenGx + (temp[i - 1][j + 1].rgbtGreen * 1);
                GreenGy = GreenGy + (temp[i - 1][j + 1].rgbtGreen * -1);
                RedGx = RedGx + (temp[i - 1][j + 1].rgbtRed * 1);
                RedGy = RedGy + (temp[i - 1][j + 1].rgbtRed * -1);
            }            
            
            // add Bottom left (x * -1, y * 1)
            if ((i < (height - 1)) && (j > 0))
            {
                BlueGx = BlueGx + (temp[i + 1][j - 1].rgbtBlue * -1);
                BlueGy = BlueGy + (temp[i + 1][j - 1].rgbtBlue * 1);
                GreenGx = GreenGx + (temp[i + 1][j - 1].rgbtGreen * -1);
                GreenGy = GreenGy + (temp[i + 1][j - 1].rgbtGreen * 1);
                RedGx = RedGx + (temp[i + 1][j - 1].rgbtRed * -1);
                RedGy = RedGy + (temp[i + 1][j - 1].rgbtRed * 1);
            }    
            
            // add Bottom right (x * 1, y * 1)
            if ((i < (height - 1)) && (j < (width - 1)))
            {
                BlueGx = BlueGx + (temp[i + 1][j + 1].rgbtBlue * 1);
                BlueGy = BlueGy + (temp[i + 1][j + 1].rgbtBlue * 1);
                GreenGx = GreenGx + (temp[i + 1][j + 1].rgbtGreen * 1);
                GreenGy = GreenGy + (temp[i + 1][j + 1].rgbtGreen * 1);
                RedGx = RedGx + (temp[i + 1][j + 1].rgbtRed * 1);
                RedGy = RedGy + (temp[i + 1][j + 1].rgbtRed * 1);
            }    
            
            if (round(sqrt((BlueGx * BlueGx) + (BlueGy * BlueGy))) > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = round(sqrt((BlueGx * BlueGx) + (BlueGy * BlueGy)));
            }
            
            if (round(sqrt((GreenGx * GreenGx) + (GreenGy * GreenGy))) > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = round(sqrt((GreenGx * GreenGx) + (GreenGy * GreenGy)));
            }
            
            if (round(sqrt((RedGx * RedGx) + (RedGy * RedGy))) > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = round(sqrt((RedGx * RedGx) + (RedGy * RedGy)));
            }
        }
    }
    
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
            
    double counter, sumBlue, sumGreen, sumRed;

    RGBTRIPLE temp[height][width];
    
    for (int i = 0; i < height; i++)
    {
        // Loop to copy image into a temporary field
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            counter = 1;
            sumBlue = temp[i][j].rgbtBlue;
            sumGreen = temp[i][j].rgbtGreen;
            sumRed = temp[i][j].rgbtRed;
            
            // Add right pixel if it exists
            if (j < (width - 1))
            {
                sumBlue = sumBlue + temp[i][j + 1].rgbtBlue;
                sumGreen = sumGreen + temp[i][j + 1].rgbtGreen;
                sumRed = sumRed + temp[i][j + 1].rgbtRed;
                counter++;
            }
            
            // Add left pixel if it exists
            if (j > 0)
            {
                sumBlue = sumBlue + temp[i][j - 1].rgbtBlue;
                sumGreen = sumGreen + temp[i][j - 1].rgbtGreen;
                sumRed = sumRed + temp[i][j - 1].rgbtRed;
                counter++;
            }
            
            // Add pixels above if it exists
            if (i > 0)
            {
                sumBlue = sumBlue + temp[i - 1][j].rgbtBlue;
                sumGreen = sumGreen + temp[i - 1][j].rgbtGreen;
                sumRed = sumRed + temp[i - 1][j].rgbtRed;
                counter++;
            }
            
            // Add pixels below if it exists
            if (i < (height - 1))
            {
                sumBlue = sumBlue + temp[i + 1][j].rgbtBlue;
                sumGreen = sumGreen + temp[i + 1][j].rgbtGreen;
                sumRed = sumRed + temp[i + 1][j].rgbtRed;
                counter++;
            }
            
            // add Top left
            if ((i > 0) && (j > 0))
            {
                sumBlue = sumBlue + temp[i - 1][j - 1].rgbtBlue;
                sumGreen = sumGreen + temp[i - 1][j - 1].rgbtGreen;
                sumRed = sumRed + temp[i - 1][j - 1].rgbtRed;
                counter++;
            }
            
            // add Top Right
            if ((i > 0) && (j < (width - 1)))
            {
                sumBlue = sumBlue + temp[i - 1][j + 1].rgbtBlue;
                sumGreen = sumGreen + temp[i - 1][j + 1].rgbtGreen;
                sumRed = sumRed + temp[i - 1][j + 1].rgbtRed;
                counter++;
            }            
            
            // add Bottom left
            if ((i < (height - 1)) && (j > 0))
            {
                sumBlue = sumBlue + temp[i + 1][j - 1].rgbtBlue;
                sumGreen = sumGreen + temp[i + 1][j - 1].rgbtGreen;
                sumRed = sumRed + temp[i + 1][j - 1].rgbtRed;
                counter++;
            }    
            
            // add Bottom right
            if ((i < (height - 1)) && (j < (width - 1)))
            {
                sumBlue = sumBlue + temp[i + 1][j + 1].rgbtBlue;
                sumGreen = sumGreen + temp[i + 1][j + 1].rgbtGreen;
                sumRed = sumRed + temp[i + 1][j + 1].rgbtRed;
                counter++;
            }    

            image[i][j].rgbtBlue = round(sumBlue / counter * 1.0);
            image[i][j].rgbtGreen = round(sumGreen / counter * 1.0);
            image[i][j].rgbtRed = round(sumRed / counter * 1.0);
        }
    }
    
    return;
}
