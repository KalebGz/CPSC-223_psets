TA Questions

1.  free(): double free detected in tcache 2


char *concat(char *begin, char *Ti, char *end)
{
    char *newStr = malloc(sizeof(char) * (strlen(begin) + strlen(Ti) + strlen(end)));
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
    for (int i = 0; i < strlen(end)-1; i++)
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