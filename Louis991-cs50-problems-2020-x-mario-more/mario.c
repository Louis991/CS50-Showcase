#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get pyramid height, between 1 and 8.
    int height;
    do
    {
        height = get_int("Enter the pyramid's height: ");
    }
    while (height > 8 || height < 1);

    //Draw the pyramid.
    //Outer loop. Starts a new line.
    for (int i = 0; i < height; i++)
    {
        //Inner loop, to align the 1st pyramid to the right.
        for (int k = 0; k < (height - i - 1); k++)
        {
            printf(" ");
        }

        //Inner loop, to draw hashes of the 1st pyramid.
        int j = 0;
        do
        {
            printf("#");
            j++;
        }
        while (j <= i);

        //Draws the two spaces between pyramids.
        int l = 0;
        printf("  ");

        //Inner loop, to draw hashes of the 2nd pyramid.
        while (l <= i)
        {
            printf("#");
            l++;
        }
        printf("\n");
    }
}