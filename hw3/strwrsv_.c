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
    // char *str;

    //Variables for storing the change conditions for applying + and -
    int onlyFirst = 0;
    int onlyLast = 0;
    while (getline(&line, &n, stdin) != -1)
    {


        // Getting the flags
        int subRule = 'n';
        int ordRule = 'N';
        // Means that the 2nd argument is a flag based on spec
        if (argc % 2 == 0)
        {
            //check if it is a flag
            if (*argv[1] != '-')
            {
                printf("strwrs [ -v ] [ -SRNQsrnq ] [ FROM TO ]*\n"); // Invalid input
                return -1;
            }
            //TODO: Uninitialzie to handle incorrect flags given
            find_flag(argv[1], &subRule, &ordRule);
        }

        //Exit program if there aren't any rules specified
        if (argc <= 2)
        {
            printf("strwrs [ -v ] [ -SRNQsrnq ] [ FROM TO ]*\n");
            return -1;
        }

        int FiIdx = 2;
        //means that there were no flags specified so need to decrement start of rules to argv[1] and use the standard flags
        if ((argc % 2 != 0))
        {
            FiIdx -= 1;
        }

        char *newStr; // formatted string (what will be outputted)
        // For loop for iterating through the rules
        for (; (FiIdx + 2) <= argc; FiIdx += 2)
        {

            char *Fi = argv[FiIdx];
            char *Ti = argv[FiIdx + 1];

            // Set toggle(onlyFirst) on for only swapping the first instance

            // If the string is bounded by * and -, change the whole string
            if ((*(Fi) == '+') && (*(Fi + strlen(argv[FiIdx]) - 1) == '-'))
            {

                Fi += 1;
                *(Fi + strlen(Fi) - 1) = '\0';

                char *newStrr;
                newStrr = strdup(line);
                *(newStrr + strlen(newStrr) - 1) = '\0';
                // printf("old strlen: %ld\n", strlen(newStrr));

                // printf("new strlen: %ld\n", strlen(newStrr));
                // printf("newStrr:");
                // printString(newStrr);
                // printf("Fi:");
                // printString(Fi);
                // printf("Equal?: %d\n", strcmp(Fi, newStrr));
                // printf("newstr strlen: %ld\n", strlen(newStrr));
                if (strcmp(Fi, newStrr) == 0)
                {
                    // printf("They are equal!\n");
                    newStr = strdup(Ti);
                }
                free(newStrr);

                Fi -= 1;
                *(Fi + strlen(Fi) - 1) = '\0';
            }
            else
            {

                if (*(Fi) == '+')
                {
                    onlyFirst = 1;
                    Fi += 1;
                }

                //Todo: Check what happens if the switch is only '/' ('/' is the last elemtn)
                if ((*(Fi) == '/'))
                {
                    if ((*(Fi + 1) == '-'))
                    {
                        ;
                    }
                    else
                    {
                        Fi += 1;
                    }
                }
                // printf("Fi: ");
                // printString(Fi);
                int toggle = 0; // Used to tell the if statement on line 91 to not run because last - has been skipped

                //TODO: implment +- and +blahblah-
                //TODO: If both + and - are provided, check if the strings are equal
                //TODO: Check if argv[FiIdx] needs to be Fi

                if (strlen(Fi) > 1 && *(Fi + strlen(Fi) - 2) == '/')
                {
                    if (*(Fi + strlen(Fi) - 1) == '-')
                    {
                        //Remove the / by reaplcing it with - and set the following pointer to a string terminator
                        *(Fi + strlen(Fi) - 2) = '-'; //*(Fi + strlen(argv[FiIdx]) - 1);
                        *(Fi + strlen(Fi) - 1) = '\0';
                        toggle = 1;
                    }
                }
                // printf("Fi: ");
                // printString(Fi);
                // Set toggle(onlyLast) on for only swapping the last instance
                if (*(Fi + strlen(Fi) - 1) == '-' && toggle == 0 && strlen(Fi) > 1)
                {
                    onlyLast = 1;
                    *(Fi + strlen(Fi) - 1) = '\0';
                    toggle = 0;
                }

                // String substituted for one rule
                newStr = substitute(Fi, Ti, subRule, line, onlyFirst, onlyLast);
            }

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
                if (!strcmp(newStr, line))
                {
                    continue;
                }
                FiIdx -= 2; // Repeat current swap config
            }
            else if (ordRule == 'S')
            {
                if (!strcmp(newStr, line))
                {
                    continue;
                }
                FiIdx = 0; // Restart the swap configurations
            }
            free(line);
            line = newStr;
        }
        printString(newStr); // Where output is printed
        free(newStr);
        n = 0;
        line = NULL;
    }
    free(line);
}

char *substitute(char *Fi, char *Ti, int subRule, char *str, int onlyBeg, int onlyLas)
{
    char *newStr;
    newStr = strdup(str); // duplicate to check if rules change old string in any way

    // Tells us which portion of the strings have had their matching substrings substituted
    int index = 0;
    index = strstr(newStr + index, Fi) - newStr;

    // What will become the concatonated string
    char *finalStr;

    // if the pointer is not null, then a substitution can take place
    while (index < strlen(newStr) && strstr(newStr + index, Fi) != NULL)
    {
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

        finalStr = concat(begin, Ti, end);

        free(begin);
        free(end);
        free(newStr);

        newStr = finalStr;

        if (subRule == 'q')
        {
            break;
        }
        else if (subRule == 'n')
        {

            index += strlen(Ti);
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
    return newStr;
}

// Concatonates the begining of the string(Start -> right before), Ti, and the end(End of Fi -> String Terminator)
char *concat(char *begin, char *Ti, char *end)
{
    char *newStr = malloc(sizeof(char) * (strlen(begin) + strlen(Ti) + strlen(end) + 1));
    int index = 0;
    for (int i = 0, len = strlen(begin); i < len; i++)
    {
        *(newStr + index) = *(begin + i);
        index++;
    }

    for (int i = 0, len = strlen(Ti); i < len; i++)
    {
        *(newStr + index) = *(Ti + i);
        index++;
    }
    for (int i = 0, len = strlen(end); i < len; i++)
    {

        *(newStr + index) = *(end + i);
        index++;
    }
    newStr[index] = '\0';
    return newStr;
}

// Goes through the flag and updates the pointers that store the subRule and ordRule
void find_flag(char flag[], int *subRule, int *ordRule)
{
    for (int i = 0; i < strlen(flag); i++)
    {
        //TODO: Check if erraneous inputs mean using default meta rules, update this fn to return ints if so
        char val = flag[i]; //(*(flag + i));
        if (val == 'Q' || val == 'R' || val == 'S' || val == 'N')
        {
            *ordRule = val;
        }
        else if (val == 'q' || val == 'r' || val == 's' || val == 'n')
        {
            *subRule = val; //(flag + i);
        }
        else if (val == '-')
        {
            ;
        }
        else
        {
            printf("strwrs [ -v ] [ -SRNQsrnq ] [ FROM TO ]*\n");
            exit(0); //TODO: Check if this will cause memory errors
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
