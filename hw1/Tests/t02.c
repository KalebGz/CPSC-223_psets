char *hdr = "Simple comments";

int main() {
    int c;
    int getchar(void);          // Read a char from stdin
    int putchar(int);           /* Write a char to stdout */
    int EOF = -1;               // End-of-file indicator; */ is ignored

    while ((c = getchar()) != EOF)      /* Copy chars from stdin to stdout
					     until an EOF is encountered.  */
	putchar (c);

    /* And having copied the file
       from stdin to stdout, we now */ return 0;
}
