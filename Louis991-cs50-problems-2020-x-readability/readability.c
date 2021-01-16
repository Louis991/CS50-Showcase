#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

// Function declarations.
float textInfo(string text, float infoArray[]);

// Main.
int main(void)
{
    // Getting text input.
    string text = get_string("Text: ");

    // Text data is passed by the textInfo function (passed by reference) into the array infoArray.
    float infoArray[3];
    textInfo(text, infoArray);

    // Variables required to calculate the Coleman-Liau index.
    float l = (infoArray[0] / infoArray[1]) * 100;
    float s = (infoArray[2] / infoArray[1]) * 100;

    // Index calculation and rounding.
    float index = 0.0588 * l - 0.296 * s - 15.8;
    int rounded = round(index);

    // Index branches.
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }

    else
    {
        printf("Grade %i\n", rounded);
    }
}

//Function that extracts the number of letters, words and sentences in the text.
float textInfo(string text, float infoArray[])
{
    // # of letters.
    float lettercount = 0; 

    //# of words.
    float spacecount = 0; 

    //# of sentences.
    float sentencecount = 0; 

    for (int i = 0, h = strlen(text); i < h; i++)
    {
        if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z'))
        {
            lettercount++;
        }

        if (text[i] == ' ')
        {
            spacecount++;
        }

        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentencecount++;
        }
    }

    infoArray[0] = lettercount;
    infoArray[1] = spacecount + 1;
    infoArray[2] = sentencecount;
    return 0;
}



