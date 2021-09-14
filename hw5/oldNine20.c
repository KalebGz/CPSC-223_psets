// Name: Kaleb Gezahegn
// netID: kzg2
#define _GNU_SOURCE
#include "/home/accts/kzg2/cs223/hw5/Hash.h"
#include </c/cs223/Hwk4/Deque.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//MACROS
#define DIE(msg) exit(fprintf(stderr, "%s\n", msg));

//FUNCTION PROTOTYPES
// inspired by tutorialspoint example on how to use qsort
int compFn(const void *a, const void *b);
bool oneDash(char *str);
void printSeq(Hash h, char *position, char *goal);
void swapChar(char *str, int firIdx, int secIdx);
bool isNum(char *str);

int main(int argc, char *argv[])
{
    int height = 3;
    int width = 3;
    int dimExist = 0; // =1 if the height width dimensions given

    if (argc > 5)
    {

        if (isNum(argv[1]) && isNum(argv[2]))
        {
            height = atoi(argv[1]);
            width = atoi(argv[2]);
            dimExist = 1;
        }
        else
        {
            DIE("Height/Width not numbers");
        }
    }
    if (height < 2 || height > 5 || width < 2 || width > 5)
    {
        DIE("Dimensions too large");
    }

    int initIdx = 2; 
    int goalIdx = 3;
    if (dimExist)
    {
        initIdx += 2;
        goalIdx += 2;
    }

    // checks if the length of the inital and the goal is the same
    if (strlen(argv[initIdx]) != strlen(argv[goalIdx]))
    {
        DIE("Length Mismatch")
    }

    // Makes sure there is only one -
    if (!oneDash(argv[initIdx]))
    {
        DIE("More than one/ Zero -")
    }

    // Checks if init and goal have the same characters
    char *initCpy = strdup(argv[initIdx]);
    char *goalCpy = strdup(argv[goalIdx]);
    qsort(initCpy, strlen(initCpy), sizeof(char), compFn);
    qsort(goalCpy, strlen(goalCpy), sizeof(char), compFn);
    if (strcmp(initCpy, goalCpy) != 0)
    {
        DIE("Non Identical Characters")
    }
    
    // Check if maxlength is an int
    int maxStepsIdx = dimExist ? 3 : 1;
    if (!isNum(argv[maxStepsIdx]))
    {
        DIE("WRONG MAXSTEPS");
    }

    Deque queue = NULL;
    createD(&queue);
    Hash h = createH();

    char *initalStr = strdup(argv[goalIdx]);
    addD(&queue, argv[goalIdx]);        // add the word GOAL to the queue
    // char* null0 = "null";
    insertH(h, initalStr, NULL, 0);     //add the triple (INITIAL, NULL, 0) to the search table
    free(initalStr);
    

    char *currPos = NULL;
    int MAXSTEPS = atoi(argv[maxStepsIdx]);
    int n = 0;
    while (!isEmptyD(&queue))
    {
        remD(&queue, &currPos);
        Triple posTrip = searchH(h, currPos);
        n = posTrip->nSteps;

        if (n < MAXSTEPS)
        {
            char *possibleMoves[] = {NULL, NULL, NULL, NULL};

            int index = (int)(strchr(currPos, '-') - currPos);
            //Up
            if ((index + 1) > width)
            {
                char *up = strdup(currPos);
                swapChar(up, index, index - width);
                possibleMoves[0] = up;
            }
            // down
            if ((index + 1) <= (height * width) - width)
            {
                char *down = strdup(currPos);
                swapChar(down, index, index + width);
                possibleMoves[1] = down;
            }
            // Left
            if ((index + 1) % width != 1)
            {
                char *left = strdup(currPos);
                swapChar(left, index, index - 1);
                possibleMoves[2] = left;
            }
            //Right
            if ((index + 1) % width > 0)
            {
                char *right = strdup(currPos);
                swapChar(right, index, index + 1);
                possibleMoves[3] = right;
            }

            for (int i = 0; i < 4; i++)
            {
                if (possibleMoves[i] == NULL)
                {
                    continue;
                }
                char *pPrime = possibleMoves[i];
                if (strcmp(pPrime, argv[initIdx]) == 0)
                {
                    insertH(h, pPrime, currPos, posTrip->nSteps + 1);
                    printSeq(h, pPrime, argv[goalIdx]);
                    free(pPrime);
                    destroyH(h);
                    exit(0); // DONE
                }
                else if (searchH(h, pPrime) == NULL)
                {
                    //char* from = strdup(currPos);
                    addD(&queue, pPrime);
                    insertH(h, pPrime, currPos, posTrip->nSteps + 1);
                }else{
                    free(pPrime);
                }
                
                    
                    
                
            }
        }
        free(currPos);
    }
    destroyH(h);
    exit(0);
}
void swapChar(char *str, int firIdx, int secIdx)
{
    char tmp = *(str + firIdx);
    str[firIdx] = str[secIdx];
    str[secIdx] = tmp;
}

void printSeq(Hash h, char *position, char *goal)
{
    Triple seq = searchH(h, position);
    while (1)
    {
        printf("%s\n", seq->position);
        if (strcmp(seq->position, goal) == 0)
        {
            break;
        }
        seq = searchH(h, seq->from);
    }
}
int compFn(const void *a, const void *b)
{
    return (*(char *)a - *(char *)b);
}

// returns true if there is only one -
bool oneDash(char *str)
{
    int len = strlen(str);
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '-')
        {
            count += 1;
        }
    }

    return (count == 1);
}

bool isNum(char *str)
{
    int i;
    int len = strlen(str);
    for (i = 0; i < len; i++)
    {
        if (!(isdigit(str[i])))
        {
            return false;
        }
    }

    return true;
}
