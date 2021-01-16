#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    //Setting global variables: amount owed and count, the min. number of coins.
    float owed;
    int count = 0;
    
    //Get user input, in $.
    do
    {
        owed = get_float("Enter the amount owed: ");
    }
    while (owed <= 0);
    
    //Convert amount owed to cents, which implies changing the data type of the input from float to int.
    int cents = round(owed * 100);
    
    //Calculate the minimum amount of change necessary.
    if (cents >= 25)
    {
        do
        {
            cents -= 25;
            count += 1;
        }
        while (cents >= 25);
    }
    
    if (cents >= 10)
    {
        do
        {
            cents -= 10;
            count += 1;
        }
        while (cents >= 10);
    }
    
    if (cents >= 5)
    {
        do
        {
            cents -= 5;
            count += 1;
        }
        while (cents >= 5);
    }
    
    if (cents >= 1)
    {
        do
        {
            cents -= 1;
            count += 1;
        }
        while (cents >= 1);
    }
    //Print the min. number of coins required.
    printf("%i\n", count);
}