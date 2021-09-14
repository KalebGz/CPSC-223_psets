\
char *hdr = "Line splices in other unusual places";
int aardvark;                   /* At start of program (see above) */
int barnacle = 4 /\
2;                              /* Between / and non-* */
int cardinal (int dromedary) {
   return dromedary/\
1;                              /* Between * and non-/ in comment */
}
char *elephant = "\\\
";                              /* At end of string following escaped escape */
\
/* Comment */\
char *flamingo = \
"string"\
, \
hedgehog\
= \
'h'\
;                               /* Surrounding comment, string,
                                   identifier, or char constant */
\

\
                                /* Surrounding newline */
int ironwood;
int computer\
;                               /* Before identifier         */
int \
science;                        /* After identifier          */
