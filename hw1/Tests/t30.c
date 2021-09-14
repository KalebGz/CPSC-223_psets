char *hdr = "Line splice within escaped character in string";
char *sdq = "\\
"",   aardvark;                 /* Double quote in string */
char *ssq = "\\
'",   barnacle;                 /* Single quote in string */
char *se = "\\
\",   cardinal;                 /* Escape in string */
char *sn = "\\
n",   devilray;                 /* Newline in string */
char *so = "\\
012", elephant;                 /* Octal char constant in string */
char *sh = "\\
x0a", flamingo;                 /* Octal char constant in string */
int   grosbeak;
