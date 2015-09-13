#include <cs50.h>
#include <stdio.h>
#include <math.h>
/*********************************************************************/
void greedy(float money)                // function to calculate number
{                                       // number of coins
    money *= 100.0;                     
    int change = money;                 // convert money to change
    if (money - (float)change < 0.5)
        change--;
    if (money - (float)change > 0.5)
        change++;                       // correcting float value
    int count=0;
        count += change/25;
        change = change%25;
        count += change/10;
        change = change%10;
        count += change/5;
        change = change%5;
        count += change;                // steps for calculating 
    printf("%d\n",count);               // number of coins
}
/*********************************************************************/
int main(void)
{
    printf("O hai! How much change is owed?\n");
    float money = GetFloat();                      
    if (money >= 0)
        greedy(money);              // if positive number in first time
    else
    {
        while(true)                 // checking all conditions till 
        {                           // we get a positive value
            if (money >= 0)
            {
                greedy(money);
                break;              // gives output pattern and exit
            }
            else if(money < 0)
                 {
                     printf("How much change is owed?\n");
                     money = GetFloat();
                     continue;    // start checking all condition again
                 }
                 else               // to handle error input cases
                 {
                     printf("Retry:\n");
                     money = GetFloat();
                     continue;    // start checking all condition again
                 }
        }
    }
    return 0;
}
