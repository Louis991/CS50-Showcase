#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Checks for a correct number of input arguments.
    if (argc == 2)
    {
        // Opens reading stream.
        FILE *rawImage = fopen(argv[1], "r");
        if (rawImage == NULL)
        {
            printf("The image data could not be read.");
            return 1;
        }

        // Initializes pointer to output (NULL for now)
        FILE *outputImage;

        // Counts the number of images recovered thus far.
        int imageNumber = 0;

        // Defines the BYTE type.
        typedef uint8_t BYTE;

        // Declares an array used as a buffer for each 512-byte data block.
        BYTE buffer[512];

        // Declares an 8-bit array used to store the image file name, following the format "###.jpg" specified. 7 chars + \0. It will be used as a buffer by sprintf.
        char imageName[8];

        // Reads through image data.
        while (fread(buffer, 512, 1, rawImage) == 1)
        {
            // Checks for a .jpg header.
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
            {
                // Operations for the 1st block of each of the rest of the images.
                if (imageNumber > 0)
                {
                    fclose(outputImage);
                    sprintf(imageName, "%03i.jpg", imageNumber);
                    outputImage = fopen(imageName, "a");
                    fwrite(buffer, 512, 1, outputImage);
                    imageNumber++;
                }

                // Operations for the 1st block of the first image.
                if (imageNumber == 0)
                {
                    sprintf(imageName, "%03i.jpg", imageNumber);
                    outputImage = fopen(imageName, "a");
                    fwrite(buffer, 512, 1, outputImage);
                    imageNumber++;
                }
            }

            // Writes the rest of the blocks of the current image that's being operated on.
            else
            {
                if (imageNumber > 0)
                {
                    fwrite(buffer, 512, 1, outputImage);
                }
            }
        }
        fclose(outputImage);
        fclose(rawImage);
    }

    else
    {
        printf("Usage: ./recover image.raw");
        return 1;
    }

    // Done.
    return 0;
}