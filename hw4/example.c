// example.c                                    Stan Eisenstat (02/26/20)
//
// Demonstrate the use of the Deque ADT

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/c/cs223/Hwk4/Deque.h"

// Print message to stderr and exit.
#define DIE(msg) exit(fprintf(stderr, "%s\n", msg))

// Print error message unless COND is true
#define ORDIE(cond, msg) \
    if (cond)            \
        ;                \
    else                 \
        DIE(msg)

int main(int argc, char *argv[])
{
    FILE *fp = stdin; // Read from standard input

    Deque D;
    ORDIE(createD(&D), // Create Deque for stdin
          "createD() failed");

    int nLines = 0;
    char *line = NULL;
    size_t length = 0;
    while (0 <= getline(&line, &length, fp))
    {
        if (nLines % 2 == 0)
        {                          // On even iterations
            ORDIE(pushD(&D, line), //  Add line to head of D
                  "pushD() failed");
        }
        else
        {
            ORDIE(addD(&D, line),   // On odd iterations
                  "addD() failed"); //  Add line to tail of D
        }
        nLines++;
        line = NULL;
        length = 0;
    }
    free(line);

    for (int i = 0; !isEmptyD(&D); i++)
    {
        if (i % 2 == 1)
        {                          // On even iterations
            ORDIE(topD(&D, &line), //  Output line at head of D
                  "topD() failed");
            // printf("\n");
            // printf("OUT OF FIRST ITERATION\n");
            // printf("\n");
            fputs(line, stdout);

            ORDIE(remD(&D, &line), //  Remove line at head of D
                  "remD() failed");
        }
        else
        {                           // On odd iterations
            ORDIE(headD(&D, &line), //  Output line at head of D
                  "headD() failed");
            fputs(line, stdout);
            ORDIE(popD(&D, &line), //  Remove line at head of D
                  "popD() failed");
        }
        fputs(line, stdout); // Output line removed from D
        free(line);          // Free storage for removed line
    }

    ORDIE(destroyD(&D), // Destroy Deque
          "destroyD() failed");

    return EXIT_SUCCESS;
}
