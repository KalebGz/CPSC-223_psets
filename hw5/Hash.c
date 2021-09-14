#define _GNU_SOURCE
#include "Hash.h"
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
    int n;        // number of items stored
    int size;     // size of the table pointers(size of hash table)
    Chain *table; // pointer to the start of the table chains
};

//Function prototypes
Hash growH(Hash h);
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

    h->table = malloc(sizeof(Chain) * size);
    for (int i = 0; i < size; i++)
    {
        h->table[i] = malloc(sizeof(struct chain));
        h->table[i]->n = 0;
        h->table[i]->trip = NULL;
    }
    return h;
}

// Insert
void insertH(Hash h, char *position, char *from, int nSteps)
{
    //Create a new Triple for entry
    Triple newTrip = malloc(sizeof(struct triple));
    newTrip->position = position;
    newTrip->from = from;
    newTrip->nSteps = nSteps;

    int hIdx = hFunction(newTrip->position, h->size); // hash index

    Chain oldChain = h->table[hIdx];

    h->table[hIdx] = putInChain(oldChain, newTrip); // put the new triple in the appropriate chain
    h->n += 1;

    // Hash tmp = NULL;
    //grow the hash table if the number of items in the hash table = size of table
    if (h->n == h->size * MAX_LOAD_FACTOR)
    {
        // tmp = h;
        h = growH(h);
    }
}

// iterates through a chain and places a newChain before an item of greater position than it
Chain putInChain(Chain oldChain, Triple newTrip)
{

    int newN = oldChain->n + 1;
    oldChain->trip = realloc(oldChain->trip, sizeof(Triple) * newN);
    oldChain->n = newN;

    int newTripPlaced = 0;
    Triple tmp1; //= NULL;
    Triple tmp2; //= NULL;
    int i;
    for (i = 0; i < oldChain->n; i++)
    {
        //Account for first element being inputted
        if (oldChain->n == 1)
        {
            oldChain->trip[i] = newTrip;
            continue;
        }

        // if the position of the triple being looked at is greater than the new triple, place this triple instead
        if (!newTripPlaced && strcmp(oldChain->trip[i]->position, newTrip->position))
        {
            // place item at index i in temp and put the new Triple at index I
            tmp1 = oldChain->trip[i];
            oldChain->trip[i] = newTrip;
            newTripPlaced = 1; // Toggle placement on
        }
        else if (newTripPlaced)
        {
            // there is item stored in tmp1 right now so save the next element in 2,
            // then save the item in tmp 1 at the current index.
            // copy what's in tm2 into tmp1 so that it can be placed in the next iteration
            tmp2 = oldChain->trip[i];
            oldChain->trip[i] = tmp1;
            tmp1 = tmp2;
        }
    }
    return oldChain;
}

// grows the hash table by growth factor if Load average exceeds 8
Hash growH(Hash h)
{

    Hash copy = internalCreateH(h->size * GROWTH_FACTOR);
    Triple tempTrip; // temp Triple used for copying over Triples

    // Goes through all the pointers to chains in table
    for (int i = 0; i < h->size; i++)
    {
        for (int j = 0; j < h->table[i]->n; j++)
        {
            tempTrip = h->table[i]->trip[j];
            insertH(copy, tempTrip->position, tempTrip->from, tempTrip->nSteps);
        }
    }
    // Hash tmpHash = h; // old hash table becomes saved in tmpHash
    // *h = *copy;       // h becomes the copied hash with the expanded size

    // destroyH(h); // old hash is destroyed
    return copy;
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
    }
    return resTrip;
}

//destroys the Hash table
void destroyH(Hash h)
{
    Chain *tbl = h->table;
    for (int i = 0; i < h->size; i++)
    {

        Chain chn = tbl[i];
        for (int j = 0; j < chn->n; j++)
        {
            // free(chn->trip[j]->position);
            // free(chn->trip[j]->from);
            free(chn->trip[j]);
        }
        free(tbl[i]);
    }

    free(h->table);
    free(h);
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