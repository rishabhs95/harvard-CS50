#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main (int argc, string argv[])
{
    int i;
    string key = argv[1];
    if (argc > 2 || argc < 2)                               // if command line arguments are greater than 2 or less than 2
    {
        printf("Improper use of Command line arguments!\n");
        return 1;
    }
    else
    {    for (i=0; i<strlen(key); i++)                      // if key is not alphabetic
        {   
            if(!isalpha(key[i]))
            {
                printf("Improper use of Command line arguments!\n");
                return 1;
            }
        }
    }  
    int keylen = strlen(key); 
    string text = GetString();
    int len = strlen(text);
    int add, j=0;
    char cipher[len];
    for (i=0; i<len; i++)                                   // converting into cipher code
    {
        
        if (isalpha(text[i]))                               // if input text is alphabetic
            {   
                if (isupper(key[j%keylen]))
                    add = (int)key[j%keylen] - 'A';         // determining value if key has uppercase character
                if (islower(key[j%keylen]))
                    add = (int)key[j%keylen] - 'a';         // determining value if key has lowercase character
                j++;
                if(isupper(text[i]))
                    cipher[i] = (int)'A' + ((int)text[i] - (int)'A' + add)%26;
                else 
                    cipher[i] = (int)'a' + ((int)text[i] - (int)'a' + add)%26;
            }
        else
            cipher[i] = text[i];
    }
    for (i=0; i<len; i++)                                   // prints cipher code
        printf("%c",cipher[i]);
    printf("\n");
    return 0;
}
