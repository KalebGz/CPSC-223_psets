char *hdr = "Line splice within other escaped character in char constant";
char cdq = '\\
"',   aardvark;                 /* Double quote in char constant */
char cn = '\\
n',   devilray;                 /* Newline in char constant */
char co = '\\
012', elephant;                 /* Octal char constant */
char ch = '\\
x0a', flamingo;                 /* Hexadecimal char constant */
int   grosbeak;
