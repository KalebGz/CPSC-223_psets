#!/usr/bin/perl
#
#   t39.pl N
#
# Create test file using first N words in /c/cs223/Doc/words

@words = `head -n $ARGV[0] /c/cs223/Doc/words`;
chomp (@words);
$words = join ("", @words);

print qq[char *hdr = "Very long file / line / identifier / string / comment";\n];
print qq[char *dictionary[] = {\n];
for $w (@words) {
   print qq[   "$w",\n];
}
print qq[};\n];
print qq[int               $words;\n];
print qq[char *aardvark = "$words";\n];
print qq[int barnacle;  /* $words */\n];
print qq[int cardinal;\n];
