#include <cs50.h>
#include <stdio.h>
/*********************************************************************/
void pattern(int height)    // function to print mario pattern 
{
    for(int i = 1;i <= height;i++)
    {
        for(int j = 1;j <= height - i;j++)
            printf(" ");
        for(int j = 1;j <= i + 1;j++)
            printf("#");
        printf("\n");
    }
}
/*********************************************************************/
int main(void)
{
    printf("Height:");
    int height= GetInt();
    if (height >= 0 && height <= 23) //if height found right first time
        pattern(height);
    else
    {                           // else checking height again and again
        while(true)
        {   
            if (height >= 0 && height <= 23)
            {
                pattern(height);
	    	    break; // gives output pattern and exit
            }
	   
            else 
                if (height < 0 || height > 23)
            	{
              	    printf("Height:");
                    height = GetInt();
                    continue;  // start checking all condition again
            	} 
                else // to handle error input cases
                {
                    printf("Retry:");
                    height = GetInt();
                    continue; // start checking all condition again
                }
        }
    }
    return 0;
}
