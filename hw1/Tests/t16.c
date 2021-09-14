char *hdr = "Adjacent and 'overlapping' comments";
int abalone;    /* One
		   comment *//* and
				another */ int barnacle;

int cuttlefish; /* Does // end this comment?
		     int damselfish;
		   No. */

int elephant;   // Does this /* start an ANSI comment?
		     int flounder;
		// No. */

int grouper;    //* Did we start an ANSI comment?
		      int haddock;
		// No. */
