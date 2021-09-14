char *hdr = "Preprocessor directives (graceful)";

#include <stdio.h>
#define TRUE  1                 /* Truth and falsity */
#define FALSE 0

int main() {
    char c = '!';

    #ifdef DEBUG
        printf ("c = >%c<\n", c);
    #endif
    printf ("Hello world%c\n", c);
    return 0;
}
