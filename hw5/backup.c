#include "/home/accts/kzg2/cs223/hw5/Hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (8)
#define MAX_LOAD_FACTOR (8)

// Inspired by Aspnes's implementation of Hash tables with linked lists

//Structs

typedef struct chain
{
    int n;        // Number of items in chain
    Triple *trip; // pointer to the first triple in the array
} * Chain;

struct hash
{
    int n;       // number of items stored
    int size;    // size of the table pointers(size of hash table)
    Chain *table; // pointer to the start of the table chains
};

//Function prototypes
void growH(Hash h);
Hash internalCreateH(int size);
Chain putInChain(Chain oldChain, Triple newTrip);
static long hFunction(char *s, long size);

//Initial create
Hash createH()
{
    return internalCreateH(INITIAL_SIZE);
}

Hash internalCreateH(int size)
{
    Hash h;
    h = malloc(sizeof(*h));

    h->n = 0;
    h->size = size;
    Chain arrTable = malloc(sizeof(*arrTable) * size);
    //h->table =  &h->table[0];
    // intiialize empty chains
    for (int i = 0; i < size; i++)
    {
        arrTable->n = 0;
        arrTable->trip = NULL;
    }
    h->table = &arrTable;
    return h;
}

// Insert
void insertH(Hash h, char *position, char *from, int nSteps)
{
    Triple newTrip = malloc(sizeof(*newTrip));
    newTrip->position = position;
    newTrip->from = from;
    newTrip->nSteps = nSteps;

    int hIdx = hFunction(newTrip->position, h->size); // hash index

    Chain oldChain = h->table[hIdx];

    h->table[hIdx] = putInChain(oldChain, newTrip); // put the new triple in the appropriate chain

    h->n += 1;
    //grow the hash table if the number of items in the hash table = size of table
    if (h->n == h->size * MAX_LOAD_FACTOR)
    {
        growH(h);
    }
}

// iterates through a chain and places a newChain before an item of greater position than it
Chain putInChain(Chain oldChain, Triple newTrip)
{

    Chain newChain = NULL;
    newChain = malloc(sizeof(*newChain));
    newChain->n = 0;
    newChain->trip = malloc(sizeof(*newChain->trip) * (oldChain->n + 1));

    int newTripPlaced = 0;
    int i;
    for (i = 0; i < oldChain->n; i++)
    {

        // if the position of the triple being looked at is greater than the new triple, place this triple instead
        if (!newTripPlaced && strcmp(oldChain->trip[i]->position, newTrip->position))
        {
            newChain->trip[i] = newTrip;
            newChain->n += 1;
            newTripPlaced = 1; // toggle variable
            i--;            // allows for triple in i to be added to new chain in the next iteration
        }
        else
        {
            if (newTripPlaced)
            {
                newChain->trip[i + 1] = oldChain->trip[i];
                newChain->n += 1;
            }
            else
            {
                newChain->trip[i] = oldChain->trip[i];
                newChain->n += 1;
            }
        }
    }

    // triple has largest pos so put it last
    if (newTripPlaced == 0)
    {
        newChain->trip[i] = newTrip;
        newChain->n += 1;
    }

    //Sanity Check
    if ((newChain->n - 1) != oldChain->n)
    {
        printf("Something went wrong\n");
    }

    if(oldChain->n!= 0){
        free(oldChain);
    }

    return newChain;
}

// grows the hash table by growth factor if Load average exceeds 8
void growH(Hash h)
{

    Hash copy = internalCreateH(h->size * GROWTH_FACTOR);
    Triple tempTrip; // temp Triple used for copying over Triples

    for (int i = 0; i < h->size; i++) // Goes through all the pointers to chains in table
    {
        for (int j = 0; j < h->table[i]->n; j++)
        {
            tempTrip = h->table[i]->trip[j];
            insertH(copy, tempTrip->position, tempTrip->from, tempTrip->nSteps);
        }
    }

    Hash tmpHash = h; // old hash table becomes saved in tmpHash
    *h = *copy;       // Hash becomes the copy

    destroyH(tmpHash); // old hash is destroyed
}
// searches through hash table and returns
Triple searchH(Hash h, char *key)
{
    Triple tempTrip = NULL;

    int cmpVal; // value when comparing the keys
    int hVal = hFunction(key, h->size);

    Triple resTrip = NULL;
    for (int j = 0; j < h->table[hVal]->n; j++)
    {
        tempTrip = h->table[hVal]->trip[j];

        cmpVal = strcmp(key, tempTrip->position);

        if (cmpVal == 0)
        {
            if (resTrip == NULL || tempTrip->nSteps < resTrip->nSteps)
            {
                resTrip = tempTrip;
            }
        }
        // pos of array is greater than key
        if (cmpVal < 0)
        {
            break;
        }
    }

    return resTrip; // not foud
}

//destroys the Hash table
void destroyH(Hash h)
{
    // not necessary right now
    // printf("numItems: %d\n", h->n);
    // printf("size: %d\n", h->size);
    // printf("DONE\n");
}

// Return hash of string S into [0,SIZE)
static long hFunction(char *s, long size)
{
    unsigned long sum;
    int shift;
    const unsigned long prime = 3141592653589793239L;

    for (sum = 0, shift = 0; *s; s++)
    { // Compress string to long
        sum ^= *s << shift;
        shift += 4;
        if (shift >= 57)
            shift -= 57;
    }
    sum += sum >> 33; // Condition the bits
    sum *= prime;
    sum += sum >> 33;

    return (sum % size); // Reduce to [0,SIZE)
}
/*
Question:
1. dereferencing pointer to incomplete type ‘struct triple’
2. For the 

*/

    // Chain newChain = NULL;
    // newChain = malloc(sizeof(*newChain));
    // newChain->n = 0;
    // newChain->trip = malloc(sizeof(Triple) * (oldChain->n + 1));


        // if(!newTripPlaced){
        //     oldChain->trip[i] = newTrip;
        // }

    
    // int i;
    // for (i = 0; i < oldChain->n; i++)
    // {

    //     // if the position of the triple being looked at is greater than the new triple, place this triple instead
    //     if (!newTripPlaced && strcmp(oldChain->trip[i]->position, newTrip->position))
    //     {
    //         newChain->trip[i] = newTrip;
    //         newChain->n += 1;
    //         newTripPlaced = 1; // toggle variable
    //         i--;            // allows for triple in i to be added to new chain in the next iteration
    //     }
    //     else
    //     {
    //         if (newTripPlaced)
    //         {
    //             newChain->trip[i + 1] = oldChain->trip[i];
    //             newChain->n += 1;
    //         }
    //         else
    //         {
    //             newChain->trip[i] = oldChain->trip[i];
    //             newChain->n += 1;
    //         }
    //     }
    //     free(oldChain)
    // }

    // // triple has largest pos so put it last
    // if (newTripPlaced == 0)
    // {
    //     newChain->trip[i] = newTrip;
    //     newChain->n += 1;
    // }

    // //Sanity Check
    // if ((newChain->n - 1) != oldChain->n)
    // {
    //     printf("Something went wrong\n");
    // }

    // if(oldChain->n!= 0){
    //     free(oldChain->trip);
    //     free(oldChain);
    // }