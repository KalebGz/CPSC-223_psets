#!/usr/bin/perl
#
#   xQuick.pl [-debug] [-trace] FILE
#
# Use FILE to verify that Qsort implements quickSort
#
# If no environment variable REQUIRE_STRCMP, then assume correct.

$ME      = "xQuick.pl";
$TESTS   = "/c/cs223/Hwk4/Tests";
$PRELOAD = "XSTRCMP_DUMP= LD_PRELOAD=$TESTS/libxCompare.so";
$QUICK   = "./Qsort";
$OK      = "quickSort\n";

$ENV{'LC_ALL'} = "C";

($ARGV[0] ne "-debug")
   || ($debug = shift);

($ARGV[0] ne "-trace")
   || ($trace = shift);

(@ARGV == 1)
   || die ("usage: $ME FILE\n");

(-x $QUICK)                                     # Verify $QUICK is executable
   || die ("$ME: cannot run $QUICK\n");

$file = shift;                                  # Verify that $QUICK sorts
(-r $file)
   || die ("$ME: $file is not readable\n");
! system ("$QUICK $file | sort -c")
   || die ("$ME: '$QUICK $file | sort -c' failed\n");

open (FILE, "< $file")                          # Read lines in FILE
   || die ("$ME: cannot open $file\n");
@lines = <FILE>;
close (FILE);
chomp (@lines);
$nLines = @lines;

@sort = sort (@lines);                          # Associate rank with each line
for ($i = 0; $i < $nLines; $i++) {
   die ("$ME: $i-th line of $file is empty; "
      . "please try another file\n")
      if ($lines[$i] =~ m{^\s*$});
   die ("$ME: $i-th line of $file is repeated; "
      . "please try another file\n")
      if (defined $rank{$sort[$i]});
   $rank{$sort[$i]} = $i;
}

$quick = "$PRELOAD $QUICK $file 2>&1";          # Extract labeled compares
$quick = "$quick > /dev/null";
open (QUICK, "($quick) |")
   || die ("$ME: cannot run $quick\n");
while (<QUICK>) {
   chomp;
   next                                         # Ignore strcmp ($file, "-")
      if (m{^[LR]: -$} || m{^[LR]: $file$});
   next
      unless (m{^[LR]:});
   $debug && print "$_\n";
   if (m{^L: (.*)$}) {
      $l = $_;
      $key = $1;
      ($key !~ m{^\s*$})
	 || die ("$ME: empty line compared; "
	       . "please try another file\n");
      if (! exists $rank{$key}) {               #  If not a line in the file,
	 @match = ();                           #   Assume that it is a prefix
	 foreach $k (keys %rank) {
	    push @match, $k
	       if (substr($k,0,length($key)) eq $key);
	 }
	 $match = @match;
	 ($match == 1)
	    || die ("$ME: $match prefixes match [$key]; "
		   . "please try another file\n");
	 $key = $match[0];
      }
      push @L, $rank{$key};
      
   } elsif (m{^R: (.*)$}) {
      $r = $_;
      $key = $1;
      ($key !~ m{^\s*$})
	 || die ("$ME: empty line compared; "
	       . "please try another file\n");
      if (! exists $rank{$key}) {               #  If not a line in the file,
	 @match = ();                           #   Assume that it is a prefix
	 foreach $k (keys %rank) {
	    push @match, $k
	       if (substr($k,0,length($key)) eq $key);
	 }
	 $match = @match;
	 ($match == 1)
	    || die ("$ME: $match prefixes match [$key]; "
		   . "please try another file\n");
	 $key = $match[0];
      }
      push @R, $rank{$key};
   }
}

$L = @L;                                        # Consistency checks
$R = @R;
($L == $R)
   || die ("$ME: internal error; numbers of "
	 . "labeled lines differ: $L != $R\n");

if (@L == 0 && @R == 0 &&                       # Special case of no strcmp()
    !exists $ENV{REQUIRE_STRCMP}) {             #   and none required
   print $OK;
   exit;
}

die ("$ME: too few comparisons; "
   . "are you using strcmp() / strncmp()\n")
   if ($#L < 0 || $#R < 0);

# At this point ($L[$i], $R[$i]) contains the ranks of the lines
# compared in the $i-th call to strcmp() / strncmp().

for ($i = 0; $i < @L; $i++) {
   $debug && printf ("%d: (%d,%d)\n", $i, $L[$i], $R[$i]);
}
  
quickSort ($nLines, 0, $nLines-1, @L, @R);

print $OK;

sub quickSort { # (N, LO, HI, L, R)
   my $N = shift @_;
   my $lo = shift @_;
   my $hi = shift @_;
   my $C = @_ / 2;
   my @L = @_[0...$C-1];
   my @R = @_[$C...2*$C-1];

   $trace && printf ("%sSort line(s) in [$lo,$hi]\n", "  " x $Depth++);

#  print "N = $N, lo = $lo, hi = $hi, C = $C\n";
#  print "L: " . join (",", @L) . "\n";
#  print "R: " . join (",", @R) . "\n";

   # Special case: N = 1
   goto RETURN if ($N == 1);

   # Find splitter and remove from L and R comparisons used to split
   my %w;
   for (my $i = 0; $i < $N-1; $i++) {
      ($lo <= $L[$i] && $L[$i] <= $hi)
	 || die ("$ME: s1 = $L[$i] not in [$lo,$hi]\n");
      $w{$L[$i]}++;
      ($lo <= $R[$i] && $R[$i] <= $hi)
	 || die ("$ME: s2 = $R[$i] not in [$lo,$hi]\n");
      $w{$R[$i]}++;
   }
   my $m = keys %w;
   ($m == $N)
      || die ("$ME: line never compared\n");
   ($w{$L[0]} == $N-1 || $w{$R[0]} == $N-1)
      || die ("$ME: splitter not compared to line\n");
   my $s = ($w{$L[0]} == $N-1) ? $L[0] : $R[0];
   @L = @L[$N-1...$C-1];
   @R = @R[$N-1...$C-1];
   $C = $C - ($N-1);

   #
   if ($s == $lo) {
      quickSort ($N-1, $s+1,$hi, @L, @R);
      goto RETURN;
   }
   if ($s == $hi) {
      quickSort ($N-1, $lo,$s-1, @L, @R);
      goto RETURN;
   }

   my $blo = -1;
   my $bhi = -1;
   for (my $i = 0; $i < $C; $i++) {
      $blo = $i
	 if ($lo <= $L[$i] && $L[$i] < $s
	  && $lo <= $R[$i] && $R[$i] < $s);
      $bhi = $i
	 if ($s < $L[$i] && $L[$i] <= $hi
	  && $s < $R[$i] && $R[$i] <= $hi);
   }

   if ($blo < $bhi) {
      quickSort ($s-$lo, $lo,$s-1, @L, @R);
      @L = @L[$blo+1...$bhi];
      @R = @R[$blo+1...$bhi];
      quickSort ($hi-$s, $s+1,$hi, @L, @R);
   } else {
      quickSort ($hi-$s, $s+1,$hi, @L, @R);
      @L = @L[$bhi+1...$blo];
      @R = @R[$bhi+1...$blo];
      quickSort ($s-$lo, $lo,$s-1, @L, @R);
   }

 RETURN:
   $trace && printf ("%sEnd sort of [$lo,$hi]\n", "  " x --$Depth);
}
