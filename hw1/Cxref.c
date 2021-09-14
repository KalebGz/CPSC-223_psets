/*
@uthor: Kaleb Gezahegn
netID: KZG2

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ungetchar(c) ungetc(c, stdin) // Unread char read from stdin

int LINE_NUM = 1;     // Keeps track of the line num that the last character is on
int Identif_LINE = 1; // keeps track of the line number the last identifier is on

/* Keeps track of the different states that the scanner is in
State Key:
0 = neutral
1 = Identifier
2 = String
3 = Single Comment
4 = multiComment
5 = backslash detected -> Checks for spliced comments
6 = char constats -> ignore everything under ''
7 = Numeric constants -> Chunk of chars starts with a number, ignore it
*/
int STATE = 0;

int main()
{
        int c;
        while ((c = getchar()) != EOF)
        {
                // Centralized location where line splices are taken care of
                if (c == '\\')
                {
                        c = getchar();
                        if (c == '\n')
                        {
                                LINE_NUM++;
                                continue;
                        }
                        else
                        {
                                ungetchar(c);
                        }
                }

                // Increments linenumber if nothing happened/(last char was a space) in a line and a new line char is detected.
                if (c == '\n' && STATE == 0)
                {
                        LINE_NUM++;
                }

                // Skips Numeric constats
                if (STATE == 0 && isdigit(c) != 0)
                {
                        STATE = 7;
                        continue;
                }

                // Leaves numeric constant state if there is a new line char, next char isn't alphanum or empty space is detected
                if (STATE == 7)
                {
                        if (c == '\n')
                        {
                                LINE_NUM++;
                        }
                        if (isspace(c) > 0 || isalnum(c) == 0)
                        {

                                STATE = 0;
                                continue;
                        }
                }

                //***STATE ASSIGNMENT: Identifier State***
                if (STATE == 0 && (isalpha(c) || c == '_'))
                {
                        putchar(c);
                        Identif_LINE = LINE_NUM;
                        STATE = 1;
                        continue;
                }

                // Deal with identifiers
                if (STATE == 1)
                {
                        if (c == '\n')
                        {
                                printf(":%d\n", Identif_LINE);
                                STATE = 0;
                                LINE_NUM++;
                                continue;
                        }
                        else if (c == '/')
                        {
                                printf(":%d\n", Identif_LINE);
                                STATE = 0;
                                ungetchar(c);
                                continue;
                        }
                        // if any of the conditions are met, the identifier has finished -> reset state
                        else if (isspace(c) > 0 || (isalnum(c) == 0 && c != '_') || c == '.')
                        {
                                printf(":%d\n", Identif_LINE);
                                STATE = 0;
                                continue;
                        }
                        else
                        {
                                putchar(c);
                        }
                }

                // ***STATE ASSIGNMENT: Strings***
                if (c == '\"' && STATE == 0)
                {
                        STATE = 2;
                        continue;
                }

                // Deal with strings
                if (STATE == 2)
                {
                        //Escape Character. Skip next val
                        if (c == '\\')
                        {
                                c = getchar();
                                if (c == '\n')
                                {
                                        LINE_NUM++;
                                }
                                else
                                {
                                        continue;
                                }
                        }
                        //String is temrinted, reset State
                        if (c == '\"')
                        {
                                STATE = 0;
                                continue;
                        }
                        if (c == '\n')
                        {
                                LINE_NUM++;
                        }
                        //prints line number
                }

                // ***STATE ASSIGNMENT: Comments***
                if (c == '/' && STATE == 0)
                {
                        c = getchar();
                        if (c == '/')
                        { // singleline comment
                                STATE = 3;
                                continue;
                        }
                        else if (c == '*')
                        { // multiline
                                STATE = 4;
                                continue;
                        }
                        else
                        {
                                STATE = 5;
                                //printf("State: %d\n", STATE);
                                ungetchar(c);
                                continue;
                        }
                }

                // Deal with Single comments
                if (STATE == 3)
                {
                        if (c == '\n')
                        {
                                LINE_NUM++;
                                STATE = 0;
                                continue;
                        }
                }

                //Deal with Multi line comments
                if (STATE == 4)
                {
                        if (c == '\n')
                        {
                                LINE_NUM++;
                        }
                        if (c == '*')
                        {
                                c = getchar();
                                if (c == '/')
                                {
                                        STATE = 0;
                                        continue;
                                }
                                else
                                {
                                        ungetchar(c);
                                }
                        }
                }

                // Deal with splicing comments
                if (STATE == 5)
                {
                        if (c == '/')
                        { // singleline comment
                                STATE = 3;
                                continue;
                        }
                        else if (c == '*')
                        { // multiline
                                STATE = 4;
                                continue;
                        }
                        else
                        {
                                ungetchar(c);
                                STATE = 0;
                        }
                }

                // ***STATE ASSIGNMENT: Strings
                if (c == '\'' && STATE == 0)
                {
                        STATE = 6;
                        continue;
                }

                // Deal with strings
                if (STATE == 6)
                {
                        //Escape Character. Skip next val
                        if (c == '\\')
                        {
                                c = getchar();
                                if (c == '\n')
                                {
                                        LINE_NUM++;
                                }
                                else
                                {
                                        continue;
                                }
                        }
                        if (c == '\'')
                        {
                                STATE = 0;
                                continue;
                        }
                }
        }
}
