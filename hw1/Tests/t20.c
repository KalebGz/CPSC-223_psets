char *hdr = "*/ and /* cannot overlap";

int foo (int aardvark, int barnacle) {
    int cardinal;
    cardinal = aardvark/*  devilray
                        */*barnacle
                        */*elephant*/ 2;
    return cardinal;
}
