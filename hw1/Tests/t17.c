char *hdr = "/ or * precedes begin-comment";

int foo (int aardvark) {
    int barnacle;
    barnacle = aardvark//*   cardinal
                     /*  */* aardvark;

    return aardvark * barnacle;
}

int bar (int *aardvark) {
    int barnacle;
    barnacle = */*  cardinal
                 */ aardvark;
    return *aardvark * barnacle;
}
