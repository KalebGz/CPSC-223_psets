char *hdr = "/ precedes/follows single or double quote";

double foo (double *bar) {
    *bar = *bar /'x';           /* Why? */
    return 'x'/ *bar;
}

double bar (int *foo) {
    *foo = *foo + 1;            /* Preceding double quote is illegal */
    return (long)"x"/ *foo;
}
