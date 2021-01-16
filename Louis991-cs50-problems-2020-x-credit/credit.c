#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Get input
    long num = get_long("Insert your credit card number (no hyphens): ");

    // Checksum logic and initializing some variables used.
    int num2; // Used to store the expression that's used to cycle through each digit of the card number.
    long n = 10; // n is used in both the expression to cycle through each digit and to control the condition that sustains the main do-while loop. Long data type was used to circumvent floating point errors in the while condition of the main loop.
    int k = 1; // k is used to keep track of our current digit position in the card number, to determine parity when counting and to determine the total number of digits in the card number.
    int num3; // Stores each number multiplied by 2.
    int num4; // Same expression as in num2, but used in an inner loop when one of the digits multiplied by 2 is equal to or larger than 10 and we need to sum its digits.
    int sum = 0; // Used to store the sum of the products’ (by 2) digits.
    int sum2 = 0; // Used to store the sum of every other number that wasn't multiplied by 2, as per Luhn’s algorithm.
    do
    {
        // Cycling through each digit of the card number, from last to first (right to left), via the expression assigned to num2.
        num2 = (num % n) / (n / 10);
        n *= 10;
        // if conditional to separate every other digit of the card number, in order to apply Luhn’s algorithm. These are considered when (k % 2) == 0 is true.
        if ((k % 2) == 0)
        {
            num3 = num2 * 2;
            // If conditional for when one of the digits multiplied by 2 is equal to or larger than 10 and we need to sum its digits. For example: if num = 11 -> 1+1 = 2.

            if (num3 >= 10)
            {
                int m = 1;
                do
                {
                    m *= 10;
                    num4 = (num3 % m) / (m / 10);
                    sum += num4;
                }
                while ((num3 % m) != num3);
            }

            else
            {
                // This is the same sum variable as above, for the case of numbers with a single digit after being multiplied by 2. In this case: 0 through 4.
                sum += num3;
            }
        }

        else
        {
            sum2 += num2;
        }

        k++;
    }
    // Condition to get out of the main loop, after cycling through every digit of the card number.
    while ((num % (n / 10)) != num);

    // Actual checksum and determining the card company, based on the length of the number and the number(s) with which it starts.
    int check = sum + sum2;

    // Check condition.
    if ((check % 10) == 0)
    {
        // k-1 is used in the check instead of k, due to it being initialized with a value of 1 above.
        if (((k - 1) == 15) && (((num / 10000000000000) == 34) || ((num / 10000000000000) == 37)))
        {
            string result = "AMEX\n";
            printf("%s", result);
        }

        else if (((k - 1) == 16) && (((num / 100000000000000) == 51) || ((num / 100000000000000) == 52) || ((num / 100000000000000) == 53)
                                     || ((num / 100000000000000) == 54) || ((num / 100000000000000) == 55)))
        {
            string result = "MASTERCARD\n";
            printf("%s", result);
        }

        else if (((k - 1) >= 13 && (k - 1) <= 16) && (((num / 1000000000000) == 4) || ((num / 10000000000000) == 4)
                 || ((num / 100000000000000) == 4) || ((num / 1000000000000000) == 4)))
        {
            string result = "VISA\n";
            printf("%s", result);
        }

        else
        {
            string result = "INVALID\n";
            printf("%s", result);
        }
    }

    else
    {
        string result = "INVALID\n";
        printf("%s", result);
    }
}

