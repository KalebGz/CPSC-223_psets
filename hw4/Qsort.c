// Name: Kaleb Gezahegn
// netID: kzg2

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "/c/cs223/Hwk4/Deque.h"

//Macros created by prof slade
// Print message to stderr and exit.
#define DIE(msg) exit(fprintf(stderr, "%s\n", msg))

// Print error message unless COND is true
#define ORDIE(cond, msg) \
    if (cond)            \
        ;                \
    else                 \
        DIE(msg)

void printRes(Deque d);
void quicksort(int pos, int len, Deque q, Deque s, Deque res, int numS, int numRes, int dir);
void printToFile(Deque d);
int main(int argc, char *argv[])
{

    /*
    Edge cases: 
    - pos / len are negative numbers or not numbers
    - Lots of pos/len edge casing: not seperated by a comma, random characters, only one value, no value after comma ...
    - 2nd file's name starts with -(shoudln't matter though. just chack)
    - if an empty Deque were represented as the NULL pointer, the value of *D could change as the result of the operation.
    - ^^ POinter to deque changes inside one of the deque functions
    - strcmp when pos is > length of string (have an ifstatement that makes that string empty)

    */

    int argi = 1; // variable for iterating through the file types

    int pos = 0;
    int len = INT_MAX; // Take the rest of the string

    //Flag parsing
    if (argc > 1 && *argv[1] == '-')
    {
        argi += 1;
        char *ptr;
        pos = (int)strtol(argv[1] + 1, &ptr, 10);
        if (strlen(ptr) > 0)
        {
            len = (int)strtol(ptr + 1, &ptr, 10);
        }
    }
    // printf("Positon: %d\n", pos);
    // printf("Length: %d\n", len);

    //TODO: FIX casting
    if ((isdigit(pos) != 0 && isdigit(len) != 0))
    {
        printf("WRONG POS/LEN\n");
        return -1;
    }

    // Create the stacks and Queque for quick sort
    Deque s; //stack 1
    ORDIE(createD(&s),
          "createD() failed");

    Deque q; // queue
    ORDIE(createD(&q),
          "createD() failed");

    Deque res; // stack 2
    ORDIE(createD(&res),
          "createD() failed");

    // File reading system
    FILE *fp;
    int nLines = 0;
    char *line = NULL;
    size_t length = 0;
    // Goes through all the files given in the arguments
    while (argi < argc)
    {
        fp = fopen(argv[argi], "r");

        if(fp == NULL){
            printf("File doesn't exist\n");
            exit(0);
        }

        //read lines from files and add them to the q
        while (getline(&line, &length, fp) >= 0)
        {
            ORDIE(pushD(&s, line), //  Add line to head of D
                  "addD() failed");
            nLines += 1;
            line = NULL;
            length = 0;
        }
        fclose(fp);
        argi++;
    }
    quicksort(pos, len, s, q, res, nLines, 0, 1);
    

    printRes(s);
    free(line);

    // Destroy Deques
    destroyD(&s);
    destroyD(&res);
    destroyD(&q);
}

void quicksort(int pos, int len, Deque s, Deque q, Deque res, int numS, int numRes, int dir)
{

    char *temp = NULL; // temporary pointer storage
    char *pivot = NULL;
    if (numS == 0 )
    {
        return;
    }

    remD(&s, &pivot);
    numS -= 1;

    if (numS == 0)
    {
        pushD(&s, pivot);
        return;
    }

    // Partition
    for (; numS > 0 ; numS--) // Maybe check until n lines
    {

        remD(&s, &temp);
        int cmp;        
        cmp = (strncmp(temp + pos, pivot + pos, len)) * dir;
        if (cmp > 0) // element > pivotclear
        {
            addD(&q, temp);
        }
        else
        {
            pushD(&res, temp);
            numRes++;
        }
    }
    // pop back from q into s
    for (int i = 0; !isEmptyD(&q); i++)
    {
        remD(&q, &temp);
        pushD(&s, temp);
        numS++;
    }

//Quicksort left stack
    quicksort(pos, len, s, q, res, numS, 0, dir);

    pushD(&s, pivot);

    //Quicksort right stack
    quicksort(pos, len, res, q, s, numRes, 0, -1 * dir);

    // push items from the smaller stack back to the main stack
    for (; numRes > 0; numRes--)
    {
        remD(&res, &temp);
        pushD(&s, temp);
    }
}

void printRes(Deque d)
{
    char *line = NULL;
    //  print the contents of q
    for (int i = 0; !isEmptyD(&d); i++)
    {
        ORDIE(remD(&d, &line),
              "remD() failed");
        printf("%s", line);
        free(line);
    }
}
void printToFile(Deque d)
{
    char *line = NULL;
    //  print the contents of q
       FILE *fp;

   fp = fopen("outputs.txt", "w+");
   
   
    for (int i = 0; !isEmptyD(&d); i++)
    {
        // printf("printing\n");
        ORDIE(remD(&d, &line),
              "remD() failed");
        fputs(line, fp);
        free(line);
    }
    fclose(fp);
}
