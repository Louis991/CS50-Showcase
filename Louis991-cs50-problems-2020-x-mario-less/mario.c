#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get pyramid height.
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
        //Inner loop, to align the pyramid to the right.
        for (int k = 0; k < (height - i - 1); k++)
        {
            printf(" ");
        }
        
        //Inner loop, to draw hashes.
        int j = 0;
        do
        {
            printf("#");
            j++;
        }
        while (j <= i);
        printf("\n");
    }
}