// Name: Kaleb Gezahegn
// netID: kzg2                           Stan Eisenstat (02/27/20)
//
// An incomplete implementation of the Deque ADT.

#include <stdio.h>
#include <stdlib.h>
#include "/c/cs223/Hwk4/Deque.h"

// The internal representation of the Deque ADT is a pair of stacks (the H
// stack and the T stack), each implemented using a headless, singly-linked
// list.  To add an item to the tail (head) of the Deque, push it onto the
// top of the T (H) stack.  To remove an item from the head of the Deque,
// pop it off the top of the H stack (but if the H stack is empty, first
// move items one at a time from the top of the T sxtack to the top of the H
// stack until the T stack is empty).
//
// This data structure requires one link per item rather than two as in the
// doubly-linked list implementation given in Aspnes' notes.  But while
// removing an item may not take constant time when the H stack is empty,
// the average time per operation for a sequence of operations that begins
// and ends with an empty Deque is constant.  (Why?)

typedef struct node
{
    struct node *next;
    char *str;
} Node;

struct deque
{
    Node *Head;
    Node *Tail;
};

// Implementation of createD()
bool createD(Deque *d)
{

    //tries to malloc and if it fails, it returns false
    if ((*d = malloc(sizeof(**d)))) // TODO: Revisit logic
    {
        (*d)->Head = NULL;
        (*d)->Tail = NULL;
        return true;
    }
    return false;
}

// Implementation of isEmptyD()
bool isEmptyD(Deque *d)
{
    if ((*d)->Head == NULL && (*d)->Tail == NULL)
    {
        return true;
    }
    return false;
}


// Implementation of addD()
bool addD(Deque *d, char *s)
{
    Node *e;
    if ((e = malloc(sizeof(*e))))
    {
        e->str = s;
        e->next = (*d)->Tail;
        (*d)->Tail = e;
        return true;
    }
    return false; //malloc fails
}

// Implementation of remD()
bool remD(Deque *d, char **s)
//TODO: worry about invalid **s arguments (such as NULL) to headD() / remD()
{
    if (*d == NULL || isEmptyD(d))
    {
        *s = NULL;
        return false;
    }

    //Copy from the Tail stack to the head stack
    if ((*d)->Head == NULL)
    {
        // printf("Flipped\n");
        // Node *topTail = (*d)->Tail; // Node at the top of the tail stack
        (*d)->Head = (*d)->Tail;
        Node *nextTail = (*d)->Tail->next;
        (*d)->Head->next = NULL;

        Node *oldHead;
        (*d)->Tail = nextTail;
        while ((*d)->Tail != NULL)
        {
            nextTail = ((*d)->Tail->next); // copy of next node of the top of tail
            oldHead = (*d)->Head;

            (*d)->Head = (*d)->Tail;
            (*d)->Head->next = oldHead;
            (*d)->Tail = nextTail;
        }
    }
    // printf("%s", (*d)->Head->str);
    *s = (*d)->Head->str;

    Node *prevHead = (*d)->Head; // pointer copy of orig head node in order to free
    
    if ((*d)->Head->next == NULL)
    {
        (*d)->Head = NULL;
        free(prevHead);
    }
    else
    {
        (*d)->Head = (*d)->Head->next;
        free(prevHead);
    }


    // // Copy everything back to the Tail
    // if ((*d)->Tail == NULL)
    // {
    //     // printf("Flipped\n");
    //     // Node *topTail = (*d)->Tail; // Node at the top of the tail stack
    //     (*d)->Tail = (*d)->Head;
    //     Node *nextHead = (*d)->Head->next;
    //     (*d)->Tail->next = NULL;

    //     Node *oldTail;
    //     (*d)->Head = nextHead;
    //     while ((*d)->Head != NULL)
    //     {
    //         nextHead = ((*d)->Head->next); // copy of next node of the top of tail
    //         oldTail = (*d)->Tail;

    //         (*d)->Tail = (*d)->Head;
    //         (*d)->Tail->next = oldTail;
    //         (*d)->Head = nextHead;
    //     }
    // }

    return true;
}

// Implementation of headD()
bool headD(Deque *d, char **s)
{
    if (*d == NULL || isEmptyD(d))
    {
        *s = NULL;
        return false;
    }

    if ((*d)->Head == NULL)
    {
        // Node *topTail = (*d)->Tail; // Node at the top of the tail stack
        (*d)->Head = (*d)->Tail;
        Node *nextTail = (*d)->Tail->next;
        (*d)->Head->next = NULL;

        Node *oldHead;
        (*d)->Tail = nextTail;
        while ((*d)->Tail != NULL)
        {
            nextTail = ((*d)->Tail->next); // copy of next node of the top of tail
            oldHead = (*d)->Head;

            (*d)->Head = (*d)->Tail;
            (*d)->Head->next = oldHead;
            (*d)->Tail = nextTail;
        }

    }

    *s = (*d)->Head->str;
    return true;
}

// Implementation of pushD()
bool pushD(Deque *d, char *s)
{
    Node *e;
    if ((e = malloc(sizeof(*e))))
    {
        e->str = s; // dereferences s and saves it to e
        e->next = (*d)->Head;
        (*d)->Head = e;
        return true;
    }
    return false; //malloc fails
}

// Implementation of destroyD()
bool destroyD(Deque *d)
{

    free(*d);
    *d = NULL;
    return true;
}
