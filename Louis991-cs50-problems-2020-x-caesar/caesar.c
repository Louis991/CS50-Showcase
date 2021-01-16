#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h> 

// Function declarations.
void caesar(int key, string message);
int isnumeric(string parameter);


int main(int argc, string argv[])
{

    // Condition for correct input. Program execution + 1 single argument.
    if (argc == 2)
    {
        // Non-numeric string check
        if (isnumeric(argv[1]) == true)
        {
            printf("Argument is not numeric. Usage: ./caesar key\n");
            return 1;
        }

        else
        {
            int z = atoi(argv[1]);
            if (z > 0)
            {
                // Getting the plaintext input.
                string plain = get_string("plaintext: ");
                int k = atoi(argv[1]);
                if (k > 26)
                {
                    // Adapting k for k > 26 so the displacement always starts from the a/A letter.
                    printf("k = %i\n", k);
                    float division = (float) k / 26;
                    printf("division = %f\n", division);
                    int intpart = (int) division;
                    printf("intpart = %i\n", intpart);
                    float decimal =  division - intpart;
                    printf("decimal = %f\n", decimal);
                    int displacement = (int) round(26 * decimal);
                    printf("displacement = %i\n", displacement);

                    // Output cipher for k > 26.
                    caesar(displacement, plain);
                    printf("ciphertext: %s\n", plain);
                    return 0;
                }
                
                // Output cipher for k <= 26.
                caesar(k, plain);
                printf("ciphertext: %s\n", plain);
                return 0;
            }
            else
            {
                printf("Enter a positive k parameter only.\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

// Cipher function
void caesar(int key, string message)
{
    // Goes through the plain text.
    for (int n = 0, l = strlen(message); n < l; n++)
    {
        // Discriminates for letters, A through Z and applies Caesar's encription.
        if (message[n] >= 65 && message[n] <= 90)
        {
            if (message[n] + key > 90)
            {
                message[n] = 65 - 1 + (message[n] + key - 90);
            }
            else
            {
                message[n] += key;
            }
        }
        
        // Discriminates for letters, a through z and applies Caesar's encription.
        if (message[n] >= 97 && message[n] <= 122)
        {
            if (message[n] + key > 122)
            {
                message[n] = 97 - 1 + (message[n] + key - 122);
            }
            else
            {
                message[n] += key;
            }
        }
    }
}

// Function isnumeric
int isnumeric(string parameter)
{
    for (int i = 0, l = strlen(parameter); i < l; i++)
    {
        if (parameter[i] < 48 || parameter[i] > 57)
        {
            return true;
        }
    }
    return false;
}