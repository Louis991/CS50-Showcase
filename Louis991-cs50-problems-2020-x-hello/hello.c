#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("Enter your name here: ");
    printf("Hello, %s.\n", name);
}