char *hdr = "What else can precede or follow /";

double foo (double aardvark)
{
    double barnacle = 1234567890;
    aardvark = barnacle/ 12;            /* identifier */
    barnacle = 12 /aardvark;
    aardvark = barnacle /12;            /* integer */
    barnacle = 12/ aardvark;
    aardvark = barnacle                 /* newline */
/
12;
    return aardvark;
}
