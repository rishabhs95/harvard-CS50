#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main (int argc, string argv[])
{
    int i = 0;
    if (argc > 2 || argc < 2)                                           // if command line arguments are greater than 2 or less than 2
    {
        printf("Improper use of Command line arguments\n");
        return 1;
    }
    for (i=0; argv[1][i] != '\0'; i++)                                  // if key is not a number
    {    
        if (!(argv[1][i] >= '0' && argv[1][i] <= '9'))
        {
            printf("Improper use of Command line arguments\n");
            return 1;
        }
    }  
    int key=atoi(argv[1]);    
    string text = GetString();
    int len = strlen(text);
    char cipher[len];
    for (i=0; i<len; i++)                                               // converting into cipher
    {
        if (isalpha(text[i]))
            {   
                if(isupper(text[i]))                                    // if input text's character is uppercase
                    cipher[i] = (int)'A' + ((int)text[i] - (int)'A' + key)%26; 
                else                                                    // if input text's character is lowercase
                    cipher[i] = (int)'a' + ((int)text[i] - (int)'a' + key)%26;    
            }
        else
            cipher[i] = text[i];
    }
    for (i=0; i < len; i++)                                             // prints cipher code
        printf("%c",cipher[i]);
    printf("\n");
    return 0;
}
