#!/usr/bin/perl
#
#   xHybrid.pl TARGET PREREQ
#
# Warn if TARGET does not contain all variables and functions in PREREQ

(@ARGV == 2)
   || die ("usage: xHybrid.pl TARGET PREREQ\n");

$target = shift;
(-e $target)
   || die ("$target does not exist\n");
(-x $target)
   || die ("$target is not executable\n");

$prereq = shift;
(-e $prereq)
   || die ("$prereq does not exist\n");

open (NM, "nm -S -t d $target |")
   || die ("nm -S -t d $target failed\n");

while (<NM>) {
   chomp;
   if (m{^(\d+)\s+(\d+)\s+([BbDdTt])\s+(.*)$}) {
      ($loc, $len, $type, $name) = ($1, 0+$2, $3, $4);
   } else {
      next;
   }
   next
      if ($name =~ m{^_|@});
   $type{$name} = $type;
   $len{$name} = $len;
}

open (NM, "nm -S -t d $prereq |")
   || die ("nm -S -t d $prereq failed\n");

while (<NM>) {
   chomp;
   if (m{^(\d+)\s+(\d+)\s+([BbDdTt])\s+(.*)$}) {
      ($loc, $len, $type, $name) = ($1, 0+$2, $3, $4);
   } else {
      next;
   }
   next
      if ($name =~ m{^_|@});
   if (! exists $type{$name}) {
      die ("$name not found in $target\n");
   } elsif ($type ne $type{$name}) {
      die ("type($name) = $type does not match\n");
   } elsif ($len ne $len{$name}) {
      die ("length($name) = $len does not match\n");
   }
}
