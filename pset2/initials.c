#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
int main(void)
{
    string name = GetString();
    char initials[10];
    int i = 0, j = 0;
    for (i=0; name[i]!='\0'; i++)
    {
        if (i == 0)                             // first letter of name
            initials[j++]=toupper(name[i]);
        if (name[i-1] == ' ')                   // initial(occurs after space)
            initials[j++]=toupper(name[i]);
    }
    for(i=0; i<j; i++)                          // prints initials
        printf("%c",initials[i]);
    printf("\n");
    return 0;
}
