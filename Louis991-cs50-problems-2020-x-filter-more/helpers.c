#include "helpers.h"
#include <math.h>

// Auxiliary function declarations
void average(int height, int width, int row, int column, RGBTRIPLE image[height][width], RGBTRIPLE imagecopy[height][width]);
void edge(int height, int width, int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE imagecopy[height][width], int Gx[3][3], int Gy[3][3]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int averageColor = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            averageColor = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = averageColor;
            image[i][j].rgbtGreen = averageColor;
            image[i][j].rgbtRed = averageColor;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float sepiaBlue = 0, sepiaGreen = 0, sepiaRed = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round(width / 2); j++)
        {
            int storage1 = image[i][width - 1 - j].rgbtBlue;
            image[i][width - 1 - j].rgbtBlue = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = storage1;

            int storage2 = image[i][width - 1 - j].rgbtGreen;
            image[i][width - 1 - j].rgbtGreen = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = storage2;

            int storage3 = image[i][width - 1 - j].rgbtRed;
            image[i][width - 1 - j].rgbtRed = image[i][j].rgbtRed;
            image[i][j].rgbtRed = storage3;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copying the image. The copy will be used to calculate the blurred value without altering the original image's information.
    RGBTRIPLE imagecopy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i][j] = image[i][j];
        }
    }

    // Goes through the original image, average() reads through its data and overwrites data in the copy.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average(height, width, i, j, image, imagecopy);
        }
    }

    // Substituting data in the original image with data from the copy.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imagecopy[i][j];
        }
    }

    return;
}

void average(int height, int width, int row, int column, RGBTRIPLE image[height][width], RGBTRIPLE imagecopy[height][width])
{
    float sumGreen = 0, sumBlue = 0, sumRed = 0;
    float avgCounter = 0;

    // Top-left pixel
    if (row == 0 && column == 0)
    {
        for (int i = row; i <= row + 1; i++)
        {
            for (int j = column; j <= column + 1; j++)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // Top-right pixel
    else if (row == 0 && column == width - 1)
    {
        for (int i = row; i <= row + 1; i++)
        {
            for (int j = column; j >= column - 1; j--)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // Bottom-left pixel
    else if (row == height - 1 && column == 0)
    {
        for (int i = row; i >= row - 1; i--)
        {
            for (int j = column; j <= column + 1; j++)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // Bottom-right pixel
    else if (row == height - 1 && column == width - 1)
    {
        for (int i = row; i >= row - 1; i--)
        {
            for (int j = column; j >= column - 1; j--)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // For pixels along the top edge of the image
    else if (row == 0)
    {
        for (int i = row; i <= row + 1; i++)
        {
            for (int j = column - 1; j <= column + 1; j++)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // For pixels along the bottom edge of the image
    else if (row == height - 1)
    {
        for (int i = row; i >= row - 1; i--)
        {
            for (int j = column - 1; j <= column + 1; j++)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // For pixels along the left edge of the image
    else if (column == 0)
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            for (int j = column; j <= column + 1; j++)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // For pixels along the right edge of the image
    else if (column == width - 1)
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            for (int j = column; j >= column - 1; j--)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // For any pixel in the center area
    else if ((row > 0 && row < height) && (column > 0 && column < width))
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            for (int j = column - 1; j <= column + 1; j++)
            {
                sumGreen += image[i][j].rgbtGreen;
                sumBlue += image[i][j].rgbtBlue;
                sumRed += image[i][j].rgbtRed;
                avgCounter++;
            }
        }
    }

    // Storing blurred values in the copy.
    imagecopy[row][column].rgbtGreen = round(sumGreen / avgCounter);
    if (imagecopy[row][column].rgbtGreen > 255)
    {
        imagecopy[row][column].rgbtGreen = 255;
    }

    imagecopy[row][column].rgbtBlue = round(sumBlue / avgCounter);
    if (imagecopy[row][column].rgbtBlue > 255)
    {
        imagecopy[row][column].rgbtBlue = 255;
    }

    imagecopy[row][column].rgbtRed = round(sumRed / avgCounter);
    if (imagecopy[row][column].rgbtRed > 255)
    {
        imagecopy[row][column].rgbtRed = 255;
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copying the image. The copy will be used to calculate the blurred value without altering the original image's information
    RGBTRIPLE imagecopy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i][j] = image[i][j];
        }
    }

    // Declares and initializes Gx and Gy
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, 1}, {0, 0, 0}, {1, 2, 1}};

    // Goes through the original image, edge() computes the filtered pixels and updates the filtered copy.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            edge(height, width, i, j, image, imagecopy, Gx, Gy);
        }
    }

    // Substituting data in the original image with data from the copy.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imagecopy[i][j];
        }
    }

    return;
}

void edge(int height, int width, int row, int column, RGBTRIPLE image[height][width], RGBTRIPLE imagecopy[height][width], int Gx[3][3], int Gy[3][3])
{
    float sumXBlue = 0, sumYBlue = 0, sumXGreen = 0, sumYGreen = 0, sumXRed = 0, sumYRed = 0;
    int sobelBlue = 0, sobelGreen = 0, sobelRed = 0;
    int rowCount = 0, columnCount = 0;

    // For any pixel in the center area
    if((row > 0 && row < height - 1) && (column > 0 && column < width - 1))
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            columnCount = 0;
            for (int j = column - 1; j <= column + 1; j++)
            {
                sumXBlue += Gx[rowCount][columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[rowCount][columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[rowCount][columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[rowCount][columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[rowCount][columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[rowCount][columnCount] * image[i][j].rgbtRed;

                columnCount++;
            }
            rowCount++;
        }
    }

    // For pixels along the edges

    // Top-left pixel
    else if (row == 0 && column == 0)
    {
        for (int i = row; i <= row + 1; i++)
        {
            columnCount = 0;
            for (int j = column; j <= column + 1; j++)
            {
                sumXBlue += Gx[i + 1][j + 1] * image[i][j].rgbtBlue;
                sumYBlue += Gy[i + 1][j + 1] * image[i][j].rgbtBlue;

                sumXGreen += Gx[i + 1][j + 1] * image[i][j].rgbtGreen;
                sumYGreen += Gy[i + 1][j + 1] * image[i][j].rgbtGreen;

                sumXRed += Gx[i + 1][j + 1] * image[i][j].rgbtRed;
                sumYRed += Gy[i + 1][j + 1] * image[i][j].rgbtRed;

                columnCount++;
            }
            rowCount++;
        }
    }

    // Top-right pixel
    else if (row == 0 && column == width - 1)
    {
        for (int i = row; i <= row + 1; i++)
        {
            columnCount = 0;
            for (int j = column; j >= column - 1; j--)
            {
                sumXBlue += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtRed;

                columnCount--;
            }
            rowCount++;
        }
    }

    // Bottom-left pixel
    else if (row == height - 1 && column == 0)
    {
        for (int i = row; i >= row - 1; i--)
        {
            columnCount = 0;
            for (int j = column; j <= column + 1; j++)
            {
                sumXBlue += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtRed;

                columnCount++;
            }
            rowCount--;
        }
    }

    // Bottom-right pixel
    else if (row == height - 1 && column == width - 1)
    {
        for (int i = row; i >= row - 1; i--)
        {
            columnCount = 0;
            for (int j = column; j >= column - 1; j--)
            {
                sumXBlue += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[1 + rowCount][1 + columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[1 + rowCount][1 + columnCount] * image[i][j].rgbtRed;

                columnCount--;
            }
            rowCount--;
        }
    }

    // For pixels along the top edge of the image
    else if (row == 0)
    {
        for (int i = row; i <= row + 1; i++)
        {
            columnCount = 0;
            for (int j = column - 1; j <= column + 1; j++)
            {
                sumXBlue += Gx[1 + rowCount][columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[1 + rowCount][columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[1 + rowCount][columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[1 + rowCount][columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[1 + rowCount][columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[1 + rowCount][columnCount] * image[i][j].rgbtRed;

                columnCount++;
            }
            rowCount++;
        }
    }

    // For pixels along the bottom edge of the image
    else if (row == height - 1)
    {
        for (int i = row; i >= row - 1; i--)
        {
            columnCount = 0;
            for (int j = column - 1; j <= column + 1; j++)
            {
                sumXBlue += Gx[1 + rowCount][columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[1 + rowCount][columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[1 + rowCount][columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[1 + rowCount][columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[1 + rowCount][columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[1 + rowCount][columnCount] * image[i][j].rgbtRed;

                columnCount++;
            }
            rowCount--;
        }
    }

    // For pixels along the left edge of the image
    else if (column == 0)
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            columnCount = 0;
            for (int j = column; j <= column + 1; j++)
            {
                sumXBlue += Gx[rowCount][1 + columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[rowCount][1 + columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[rowCount][1 + columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[rowCount][1 + columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[rowCount][1 + columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[rowCount][1 + columnCount] * image[i][j].rgbtRed;

                columnCount++;
            }
            rowCount++;
        }
    }

    // For pixels along the right edge of the image
    else if (column == width - 1)
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            columnCount = 0;
            for (int j = column; j >= column - 1; j--)
            {
                sumXBlue += Gx[rowCount][1 + columnCount] * image[i][j].rgbtBlue;
                sumYBlue += Gy[rowCount][1 + columnCount] * image[i][j].rgbtBlue;

                sumXGreen += Gx[rowCount][1 + columnCount] * image[i][j].rgbtGreen;
                sumYGreen += Gy[rowCount][1 + columnCount] * image[i][j].rgbtGreen;

                sumXRed += Gx[rowCount][1 + columnCount] * image[i][j].rgbtRed;
                sumYRed += Gy[rowCount][1 + columnCount] * image[i][j].rgbtRed;

                columnCount--;
            }
            rowCount++;
        }
    }


    // Updates the filtered copy.
    sobelBlue = round(sqrt(sumXBlue * sumXBlue + sumYBlue * sumYBlue));
    if (sobelBlue > 255)
    {
        sobelBlue = 255;
    }

    sobelGreen = round(sqrt(sumXGreen * sumXGreen + sumYGreen * sumYGreen));
    if (sobelGreen > 255)
    {
        sobelGreen = 255;
    }

    sobelRed = round(sqrt(sumXRed * sumXRed + sumYRed * sumYRed));
    if (sobelRed > 255)
    {
        sobelRed = 255;
    }

    imagecopy[row][column].rgbtBlue = sobelBlue;
    imagecopy[row][column].rgbtGreen = sobelGreen;
    imagecopy[row][column].rgbtRed = sobelRed;
}