#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //Checking that we have only one command-line argument.
    if (argc != 2)
    {
        printf("Use only 1 argument (key).");
        return 1;
    }

    //Checking the validity of the key.
    int l = strlen(argv[1]);
    if (l != 26)
    {
        printf("The key needs to be 26 characters long.");
        return 1;
    }

    for (int i = 0; i < l; i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("The key needs to be strictly alphabetical.");
            return 1;
        }
    }

    for (int j = 0; j < l; j++)
    {
        for (int k = j + 1; k < l; k++)
        {
            if (argv[1][k] == argv[1][j])
            {
                printf("Each key letter must be unique.");
                return 1;
            }
        }
    }

    //Input.
    string plain = get_string("plaintext: ");

    //Converting the key string into an array of characters, each character being uppercase for purposes of the substitution.
    char upperKey[27];
    for (int q = 0; q < l; q++)
    {
        upperKey[q] = toupper(argv[1][q]);
    }
    upperKey[26] = '\0';

    //Substituting the message.
    int l3 = strlen(plain);
    for (int m = 0; m < l3; m++)
    {
        if (plain[m] >= 65 && plain[m] <= 90)
        {
            plain[m] = upperKey[plain[m] - 65];
        }

        if (plain[m] >= 97 && plain[m] <= 122)
        {
            plain[m] = tolower(upperKey[plain[m] - 97]);
        }
    }

    //Outputting the substituted message.
    printf("ciphertext: %s", plain);
    printf("\n");
    return 0;
}




