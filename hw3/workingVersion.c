/*
Name: Kaleb Gezahegn
netID: kzg2
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printString(char *arr);
void find_flag(char flag[], int *subRule, int *ordRule);
char *substitute(char *Fi, char *Ti, int subRule, char *str, int onlyBeg, int onlyLas);
char *concat(char *begin, char *Ti, char *end);

int main(int argc, char *argv[])
{
    //Gets the string being manipulated and duplicates it
    size_t n = 0;
    char *line = NULL;
    char *str;

//Variables for storing the change conditions for applying +
    int onlyFirst = 0;
    int onlyLast = 0;
    while (getline(&line, &n, stdin) != -1)
    {
        // printf("NEW LINE\n");
        str = strdup(line);
        // printf("String being read: ");
        // printString(str);
        free(line);
        n = 0;
        line = NULL;

        // Getting the flags
        int subRule = 'n';
        int ordRule = 'N';
        // Means that the 2nd argument is a flag based on spec
        if (argc % 2 == 0)
        {
            find_flag(argv[1], &subRule, &ordRule);
        }

        //Exit program if there aren't any rules specified
        if (argc <= 2)
        {
            printf("No Rules\n");
            return -1;
        }

        // index is the current element of string being looked at
        int ruleIdx = 2;
        //means that there were no flags specified so need to decrement start of rules to argv[1] and use the standard flags
        if ((argc % 2 != 0))
        {
            ruleIdx -= 1;
        }

        char *newStr; // formatted string (what will be outputted)
        // For loop for iterating through the rules
        for (; (ruleIdx + 2) <= argc; ruleIdx += 2)
        {

            char *Fi = argv[ruleIdx];
            char *Ti = argv[ruleIdx + 1];

            // char *Fi; //[strlen(argv[ruleIdx])];
            // char *Ti;// [strlen(argv[ruleIdx + 1])];
            // strcpy((argv[ruleIdx]), Fi);
            // strcpy((argv[ruleIdx + 1]), Ti);

            // printString(Fi);
            // printString(Ti);

            if (*(Fi) == '+')
            {
                onlyFirst = 1;
                Fi += 1;
            }

            if ((*(Fi) == '/'))
            {
                Fi += 1;
  
            }
            // printf("New FI: ");
            // printString(Fi);

            int toggle = 0;
            // printf("char at strlen: 1: %c \n", *(Fi + strlen(argv[ruleIdx]) - 1));
            if (*(Fi + strlen(argv[ruleIdx]) - 2) == '/')
            {
                if (*(Fi + strlen(argv[ruleIdx]) - 1) == '-')
                {
                    *(Fi + strlen(argv[ruleIdx]) - 2) = *(Fi + strlen(argv[ruleIdx]) - 1);
                    *(Fi + strlen(argv[ruleIdx]) - 1) = '\0';
                    toggle = 1;
                }
            }

            if (*(Fi + strlen(argv[ruleIdx]) - 1) == '-' && toggle == 0)
            {
                onlyLast = 1;
                *(Fi + strlen(argv[ruleIdx]) - 1) = '\0'; // remove the - from Fi
                toggle = 0;
            }

            // printf("Final FI: ");
            // printString(Fi);

            newStr = substitute(Fi, Ti, subRule, str, onlyFirst, onlyLast);
            // free(str); // don't use str from this point on so we can free it
            // printf("Returned: ");
            // printString(newStr, strlen(newStr));

            if (ordRule == 'Q')
            {
                break;
            }
            else if (ordRule == 'N')
            {
                ;
            }
            else if (ordRule == 'R')
            {
                if (!strcmp(newStr, str))
                {
                    // printf("Same Array\n");
                    continue;
                }
                ruleIdx -= 2; // will just reset ruleIdx(repeat currently implemented rule)
                // to the value it was because it will be incremented by 2 in the next iteration
            }
            else if (ordRule == 'S')
            {
                if (!strcmp(newStr, str))
                {
                    // printf("Same Array\n");
                    continue;
                }
                ruleIdx = 0; // Actually = 2 because it will be incremented at the top of the for loop
            }
            free(str);
            str = newStr;

            // printf("RuleEnd: ");
            // printString(newStr, strlen(newStr)); // Where output is printed
        }
        printString(newStr); // Where output is printed
        free(newStr);
    }
    free(line);
}

char *substitute(char *Fi, char *Ti, int subRule, char *str, int onlyBeg, int onlyLas)
{
    char *newStr;
    newStr = strdup(str); // duplicate to check if rules change old string in anay way
    int index = 0;
    index = strstr(newStr + index, Fi) - newStr;

    char *finalStr; // Used

    //TODO: If both + and - are provided, check if the strings are equal

    // if the pointer is not null, then a substitution can take place
    while (index < strlen(newStr) && strstr(newStr + index, Fi) != NULL)
    {
        // printf("index: %d\n", index);
        // printf("Start of repl: %ld\n", (strlen(newStr) - strlen(Fi)) - 1);
        // printf("loop");
        if (onlyBeg == 1 && index != 0)
        {
            break;
        }

        if (onlyLas == 1 && (index != (strlen(newStr) - strlen(Fi) - 1)))
        {
            index += 1;
            index = strstr(newStr + index, Fi) - newStr;
            continue;
        }

        char *begin = strndup(newStr, (strstr(newStr + index, Fi) - newStr)); // duplicate the characters up to Fi
        char *end = strdup(strstr(newStr + index, Fi) + strlen(Fi));          // duplicate characters from Ti to the end
        // printf("begining: ");
        // printString(begin);
        // printf("end: ");
        // printString(end);

        finalStr = concat(begin, Ti, end);
        // printf("Change: ");
        // printString(finalStr, strlen(finalStr));
        free(begin);
        // printf("End String Length: %ld\n", strlen(end));

        free(end);
        // }

        // newStr = malloc(sizeof(char) * (strlen(begin) + strlen(Ti) + strlen(end) + 1));
        // *newStr = '\0';
        // printf("New malloc String: ");
        // printString(newStr, strlen(newStr));
        // newStr = strcat(begin, Ti);
        // printf("Begin + middle: ");
        // printString(newStr, strlen(newStr));
        // strcat(newStr, end);
        // printf("Begin + middle + end: ");
        // printString(newStr, strlen(newStr));

        // free(begin);
        // free(end);

        // printf("index: %d\n", index);
        // printString(newStr, strlen(newStr));

        free(newStr);
        newStr = finalStr;
        
        if (subRule == 'q')
        {
            break;
        }
        else if (subRule == 'n')
        {

            index += strlen(Ti);
            // printf("index after n: %d , ", index);
            // printf("Char at index: %c\n", newStr[index]);
        }
        else if (subRule == 'r')
        {
            index = strstr(newStr + index, Fi) - newStr;
            if (!strcmp(newStr, str))
            {
                break;
            }
        }
        else if (subRule == 's')
        {
            index = 0;
            if (!strcmp(newStr, str))
            {
                break;
            }
        }
        else
        {
            index += 1;
        }
    }

    // printf("Str Version: ");
    // printString(finalStr, strlen(newStr));
    return newStr;
}

char *concat(char *begin, char *Ti, char *end)
{
    char *newStr = malloc(sizeof(char) * (strlen(begin) + strlen(Ti) + strlen(end) + 1));
    int index = 0;
    for (int i = 0; i < strlen(begin); i++)
    {
        *(newStr + index) = *(begin + i);
        index++;
    }

    for (int i = 0; i < strlen(Ti); i++)
    {
        *(newStr + index) = *(Ti + i);
        index++;
    }
    for (int i = 0; i < strlen(end); i++)
    {

        *(newStr + index) = *(end + i);
        index++;
    }
    // printf("CHARACTER: %c\n", newStr[index-1]);
    newStr[index] = '\0';
    // printf("IN FUNCTION: ");
    // printString(newStr, strlen(newStr));
    return newStr;
}
// Goes through the flag and updates the pointers that store the subRule and ordRule
void find_flag(char flag[], int *subRule, int *ordRule)
{
    for (int i = 0; i < strlen(flag); i++)
    {
        //TODO: Check if erraneous inputs mean using default meta rules, update this fn to return ints if so
        char val = flag[i]; //(*(flag + i));
        // printf("Val: %c\n", val);
        if (val == 'Q' || val == 'R' || val == 'S' || val == 'N')
        {
            *ordRule = val;
        }
        else if (val == 'q' || val == 'r' || val == 's' || val == 'n')
        {
            *subRule = val; //(flag + i);
        }
    }
}

//prints a char array. For debugging
void printString(char *arr)
{
    for (int i = 0; *(arr + i) != '\0'; i++)
    {
        if (arr[i] == '\n')
        {
            continue;
        }
        printf("%c", arr[i]);
    }
    printf("\n");
}
