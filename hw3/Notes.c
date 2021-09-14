Notes

Pointers
- Variable that stores memory adress
-int *a; int* a;


**strdup calls malloc so you need to free it after using it
- getline also allocates memory


Strwrs:
1. input string
2. performs substitutions based on rules you pass in as args
3. prints out result

String substitution rules

    'q' - make at most one substitution, then stop
    'n' - start scanning again from after the replacement
    'r' - start scanning from the begining of the replacement string s
    's' - start scanning again from the begining of the line

Rule order rules 
- A rule application is succesful if there was at least one substituion
- //    //          //  NOTe succesful if there was no substitution

* If rule application was unsuccesful, move on to the next rule. (When moving to the next rule, restart from begining of string)

* If succesful 
    'Q' - Quit 
    'N' - move on to the next rule
    'R' - Reapply the same rule
    'S' - reapply the rules starting from rule 0


If there are a bunch of flags, take the last rule order and substitition rules

+ -> If it the first character, only match it with the begining of the line
- -> Only match if it is at the end

Forward slash at the end 

** Look at note 2 -> Copy paste that
- not allowed to use structs or global variables

for staff solution
echo "aaaaab" | staff solution directory - r +aa aa

echo "aaaaab" | staff solution directory - r -v + aa aa Verbose form


Valgrind can be used for 

- Finding memory leaks 


To check memory leak

valgrind --leak-check=full ./PRGORAM
- Tells you if memory is freed

Strings need to be null terminated
example alphabet[26] = '\0';


Questions:

Useful Functions

asprintf/strcpy/strncpy

getline (usage: see spec Note 2), : Used to get std in

asprintf(): crates a buffer string with given vals and assigns it to pointer
  char *my_string;
  asprintf (&my_string, "Being %d is cool, but being free is best of all.", 4);
  puts (my_string);
  return 0;

strlen(const char *s): returns the lenght of a string in a long int format. doesnt count null character

strcmp(): compares 2 strings and returns 0 if they are identical

strncmp(const char *str1, const char *str2, size_t n): compares the first n bytes of str1 and str 2

strstr(),: This function returns a pointer to the first occurrence in haystack of any of the entire sequence of
characters specified in needle, or a null pointer if the sequence is not present in haystack.

strchr(): This returns a pointer to the first occurrence of the character c in the string str, or NULL if the character is not found.

strcat()
Parameters
dest − This is pointer to the destination array, which should contain a C string, and should be large enough to contain the concatenated resulting string.
src − This is the string to be appended. This should not overlap the destination.
Return Value
This function returns a pointer to the resulting string dest.

strcpy(),
Parameters
dest − This is the pointer to the destination array where the content is to be copied.
src − This is the string to be copied.
Return Value
This returns a pointer to the destination string dest.

// strncpy(),
Parameters
dest − This is the pointer to the destination array where the content is to be copied.
src − This is the string to be copied.
n − The number of characters to be copied from source.
Return Value
This function returns the final copy of the copied string.

strdup(),
The function strdup() is used to duplicate a string. It returns a pointer to null-terminated byte string.

strndup()
The function strndup works similar to the function strndup(). This function duplicates the string at most size bytes i.e. the given size in the function. It also returns a pointer to null-terminated byte string.













  Command             Input     Intermediate stages         Output

  strwrs -q ab bca   "aabbab"                              "abcabab"
                       ^^

  strwrs -n ab bca   "aabbab"  "abcabab"                   "abcabbca"
                       ^^            ^^

  strwrs -r ab bca   "aabbab"  "abcabab", "abcbcaab"       "abcbcabca" abcbcabca

                       ^^          ^^            ^^

  strwrs -s ab bca   "aabbab"  "abcabab", "bcacabab",      "bcacbcbcaca" bcacbcbcaca
                       ^^       ^^             ^^
                                "bcacbcaab", "bcacbcabca"
                                        ^^          ^^

  strwrs    ab bca   "aabbab"  "abcabab"                   "abcabbca"