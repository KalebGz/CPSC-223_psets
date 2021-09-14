/*
Binpack: Program that uses different algorithms to find the number of bins required to store a set of items
Name: Kaleb Gezahegn
NetID: kzg2
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Global Variable used to keep track of the smallest number of bins possible with the calculated permutations for -opt
int MIN_OPT;

int NUM_BACKTRACKS;

//Functions that deal with different flags
int first_fit(int items[], int binSize, int numItems);
int best_fit(int items[], int binSize, int numItems);
int bp(int items[], int binSize, int numItems, int cIdx, int bins[], int lb, int prevIdx, int toggle);

//Helper functions
int validStr(char str[]);
int calcMin(int a, int b);
void printArray(int array[], int length);
void swap(int array[], int idx1, int idx);
void insertionSort(int items[], int length);
void initializeToZero(int items[], int length);
void copyArr(int arr[], int arrDup[], int numItems);
int countNumBins(int items[], int numItems);

int main(int argc, char *argv[])
{
    NUM_BACKTRACKS = 0;
    if (argc <= 2)
    {
        printf("Too few arguments\n");
        return 0;
    }
    int binSize;
    if (validStr(argv[1]))
    {
        binSize = atoi(argv[1]);
    }
    else
    {
        printf("Incorrect binSize\n");
        return -1;
    }

    //skips through ints and sets i to the index of the first flag
    int i;
    for (i = 2; i < argc; i++)
    {
        if (atoi(argv[i]) != 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    //number of items. also means this is upperbound for num of bins required. calculated because this section is where we start looking for flags
    int numItems = i - 2;

    //creates an array(items) that saves all the items
    int items[numItems];
    for (int j = 2; j < i; j++)
    {
        if (validStr(argv[j]))
        {
            items[j - 2] = atoi(argv[j]);
        }
        else
        {
            printf("Incorrect binSize\n");
            return -1;
        }
    }

    // Loop that calls different functions based on the inputed flag
    while (i < argc)
    {
        if (!(strcmp(argv[i], "-opt")))
        {
            int arrDup[numItems];
            copyArr(items, arrDup, numItems);
            insertionSort(arrDup, numItems);
            int bins[numItems]; // TODO; Check if i need to add 1 more because it needs an empty bins
            initializeToZero(bins, numItems);
            int sum;
            for (int i = 0; i < numItems; i++)
            {
                sum += items[i];
            }
            int lowerBound = (sum / binSize) + ((sum % binSize) != 0);
            MIN_OPT = numItems;
            int numBins = bp(arrDup, binSize, numItems, 0, bins, lowerBound, 0, 0);
            printf("-opt %d\n", numBins);
            // printf("Backtracks: %d\n", NUM_BACKTRACKS);
            i++;
            continue;
        }
        else if (!(strcmp(argv[i], "-ff")))
        {
            int arrDup[numItems];
            copyArr(items, arrDup, numItems);
            int numBins = first_fit(arrDup, binSize, numItems);
            printf("-ff  %d\n", numBins);
            i++;
            continue;
        }
        else if (!(strcmp(argv[i], "-ffd")))
        {
            int arrDup[numItems];
            copyArr(items, arrDup, numItems);
            insertionSort(arrDup, numItems);
            int numBins = first_fit(arrDup, binSize, numItems);
            printf("-ffd %d\n", numBins);
            i++;
            continue;
        }
        else if (!(strcmp(argv[i], "-bf")))
        {
            int arrDup[numItems];
            copyArr(items, arrDup, numItems);
            int numBins = best_fit(arrDup, binSize, numItems);
            printf("-bf  %d\n", numBins);
            i++;
            continue;
        }
        else if (!(strcmp(argv[i], "-bfd")))
        {
            int arrDup[numItems];
            copyArr(items, arrDup, numItems);
            insertionSort(arrDup, numItems);
            int numBins = best_fit(arrDup, binSize, numItems);
            printf("-bfd %d\n", numBins);
            i++;
            continue;
        }
        else
        {
            printf("Incorrect flag\n");
            break;
        }
    }
}

/*
Purpose: Calulates number of bins required using the first fit heuristic
Returns: the number of bins required to place all items in an array
*/
int first_fit(int items[], int binSize, int numItems)
{
    //creates an empty array that will be used to organize the items to Bins
    int bins[numItems];
    initializeToZero(bins, numItems);
    for (int i = 0; i < numItems; i++)
    {
        for (int binIdx = 0; binIdx < numItems; binIdx++)
        {
            if ((items[i] + bins[binIdx]) <= binSize)
            {
                bins[binIdx] += items[i];
                break;
            }
            {
            }
        }
    }
    return countNumBins(bins, numItems);
}

/*
Purpose: Calulates number of bins required using the best fit heuristic
Returns: the number of bins required to place all items in an array
*/
int best_fit(int items[], int binSize, int numItems)
{
    int bins[numItems];
    initializeToZero(bins, numItems);
    int minIdx = 0;
    int minSpace = binSize;
    for (int i = 0; i < numItems; i++)
    {
        for (int binIdx = 0; binIdx < numItems; binIdx++)
        {
            //remainder
            int rmdr = (binSize - (bins[binIdx] + items[i]));
            if ((rmdr >= 0) && (rmdr < minSpace))
            {
                minSpace = rmdr;
                minIdx = binIdx;
            }
        }

        bins[minIdx] += items[i];
        minSpace = binSize;
        // printArray(bins, numItems);
    }
    // printArray(items, numItems);
    return countNumBins(bins, numItems);
}

// printf("Items");
// printArray(items, numItems);
// printf("%d\n", binSize);
// printf("%d\n", numItems);
// printf("%d\n", cIdx);
// printArray(bins, numItems);
// printf("%d\n", min);
// return -1;
/*
Purpose: Calulates minimum number of bins required by generating every optimal permutation for storing items in a bin
Importatant params:
    int cIdx: Used to keep track of the current item being placed
    int lb: minimum possible number of bins that would be required(met when each bin fits capacity), return bincount if this is ever the case
    int prevIdx: The index that the previous item of equal value was placed(Necessary for Heuristic D)
    int toggle: when toggle = 1; that means that two bins have the same capacity and so you shouldn't try placing one item into the 2nd(Heuristic )
Returns: the number of bins required to place all items in an array
*/
int bp(int items[], int binSize, int numItems, int cIdx, int bins[], int lb, int prevIdx, int toggle)
{
    NUM_BACKTRACKS++;
    int numBins = countNumBins(bins, numItems);
    if (cIdx == (numItems))
    {
        // printArray(bins,numItems);
        return numBins;
    }
    else if (numBins == MIN_OPT)
    {
        return MIN_OPT;
        // printArray(bins, numItems);
    }
    else if (MIN_OPT == lb)
    {
        return lb;
    }

    int startVal = 0;
    if (cIdx > 0 && (items[cIdx] == items[cIdx - 1]))
    {
        startVal = prevIdx;
    }

    //printf("startval: %d\n", startVal);
    for (int i = startVal; i < numItems; i++)
    {
        for (int j = i - 1; j >= startVal; j--)
        {
            if (bins[i] == bins[j])
            {
                toggle = 1;
                break;
            }
            if (j == startVal)
            {
                toggle = 0;
            }
        }
        if (startVal == MIN_OPT)
        {
            break;
        }

        if (toggle == 1)
        {
            continue;
        }
        // Stops loop from placing item into every empty session if (bins[i] == 0)
        if (bins[i] == 0)
        {
            // printf("A");
            // printf("Add item[%d] = %d to bin #%d\n", cIdx,items[cIdx], i);
            bins[i] += items[cIdx];
            MIN_OPT = calcMin(bp(items, binSize, numItems, cIdx + 1, bins, lb, i, 0), MIN_OPT);
            bins[i] -= items[cIdx];
            if (MIN_OPT == lb)
            {
                break;
            }
            // printf("Removed item[%d] = %d from bin #%d\n", cIdx, items[cIdx], i);
            break;
        }
        if ((binSize - (bins[i] + items[cIdx])) >= 0)
        {
            // printf("B");

            bins[i] += items[cIdx];
            // printf("Add item[%d] = %d to bin #%d\n", cIdx,items[cIdx], i);
            MIN_OPT = calcMin(bp(items, binSize, numItems, cIdx + 1, bins, lb, i, 0), MIN_OPT);
            // printf("Removed item[%d] = %d from bin #%d\n", cIdx, items[cIdx], i);
            bins[i] -= items[cIdx];
            if (MIN_OPT == lb)
            {
                break;
            }
        }
    }

    return MIN_OPT;
}

/*
Purpose: Sets the value of all the indexes in a given array to 0
*/
void initializeToZero(int items[], int length)
{
    for (int i = 0; i < length; i++)
    {
        items[i] = 0;
    }
}

/*
Purpose: Returns the min of a and b
*/
int calcMin(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/*
Purpose: goes through an array and counts the number of non-zero(non-empty) items
*/
int countNumBins(int bins[], int numItems)
{
    // printArray(bins, numItems);
    int numbins = 0;
    for (int i = 0; i < numItems; i++)
    {
        if (bins[i] != 0)
        {
            numbins++;
        }
    }
    // printf("numBins: %d\n", numbins);
    return numbins;
}

/*
Purpose: uses insertion sort to sort a given array in decreasing order
*/
void insertionSort(int items[], int numItems)
{
    // printArray(items, numItems);
    if (numItems == 1)
    {
        return;
    }
    for (int i = 1; i < numItems; i++)
    {
        if (items[i] > items[i - 1])
        {
            for (int j = i - 1; j >= 0; j--)
            {

                if (items[j] < items[j + 1])
                {
                    swap(items, j + 1, j);
                }
                else
                {
                    break;
                }
            }
        }
    }
}

/*
Purpose: swaps the values of given indices(used for shifting and inserting items in insertion sort)
*/
void swap(int array[], int idx1, int idx2)
{
    int temp = array[idx1];
    array[idx1] = array[idx2];
    array[idx2] = temp;
}

/*
Purpose: copies the values of arr to arrDup, useful for creating different bins for different flags
*/
void copyArr(int arr[], int arrDup[], int length)
{
    for (int i = 0; i < length; i++)
    {
        arrDup[i] = arr[i];
    }
}
/*
Purpose: prints the values of a given array. Useful for debugging
*/
void printArray(int array[], int length)
{
    printf("Array: [ ");
    for (int i = 0; i < length; i++)
    {
        printf("%d ", array[i]);
    }
    printf("]\n");
}

/*
Purpose: iterated through every character in a string to check if it is a digit. Used for checking for erraneous binSize and items
*/
int validStr(char str[])
{
    // printf("String: %s\n", str);
    for (int i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}
