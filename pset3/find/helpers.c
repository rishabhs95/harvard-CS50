/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */

/*original search function*/

bool binary_search(int value, int values[], int n)
{
    int ending = n-1;
    int beggining = 0;

    while (ending >= beggining)
    {
        int middle = (beggining + ending) / 2;
        if (values[middle] == value)
            return true;
        else if (values[middle] > value)
            ending = middle - 1;
        else
            beggining = middle + 1;
    }

    return false;
}

bool search(int value, int values[], int n)
{
    //  implement a searching algorithm
    if (value < 0)
        return false;
    else
        return binary_search(value, values, n);
}

/**
 * Sorts array of n values.
 */

void sort(int values[], int n)
{
    int i, j, temp;
    for (i=0; i < n; i++)
    {
        for (j=i; j > 0 && values[j] < values[j-1]; j--)
        {
            temp = values[j];
            values[j] = values[j-1];
            values[j-1] = temp;
        }
        
    }
}
