#!/usr/bin/perl
#
#   xArray.pl [-g N|-G N|-S N]* FILE
#
# -g:   List all global variables of size <= N in executable FILE
# -G N: List all global arrays of size > N in executable FILE
# -S N: List all static local arrays of size > N in executable FILE

while (@ARGV > 1) {
   $flag = shift;
   if ($flag eq "-g") {
       $gFlag = shift;
   } elsif ($flag eq "-G") {
       $GFlag = shift;
   } elsif ($flag eq "-S") {
       $SFlag = shift;
   } else {
       last;
   }
}

(@ARGV == 1)
   || die ("usage: xArray.pl [-g|-G N|-S N]* FILE\n");

(-e $ARGV[0])
   || die ("$ARGV[0] does not exist\n");

(-x $ARGV[0])
   || die ("$ARGV[0] is not executable\n");

open (NM, "nm -S -t d $ARGV[0] |")
   || die ("nm -S -t d $ARGV[0] failed\n");

while (<NM>) {
   chomp;
   if (m{^(\d+) (\d+) (\w) (.*)$}) {
      ($loc, $len, $type, $name) = ($1, 0+$2, $3, $4);
   } elsif (m{^(\d+) (\w) (.*)$}) {
      ($loc, $len, $type, $name) = ($1, 0,    $2, $3);
   }
											# B/b = uninitialized data section
   next unless ($type =~ m{^[BbDd]$});              # B/b/D/d = data section
   next unless ($len > 0);                          # Vars have positive length
						     
   next if ($name eq 'stdin@@GLIBC_2.2.5');         # Ignore specific files
   next if ($name eq 'stdout@@GLIBC_2.2.5');
   next if ($name eq 'stderr@@GLIBC_2.2.5');
   next if ($name =~ m{^completed.\d+$} && 0+$len == 1);
   next if ($name =~ m{^dtor_idx.\d+$} && 0+$len == 8);

   if ($gFlag && $name =~ m{^[a-zA-Z0-9_]+$} && $len <= $gFlag) {
      print STDERR "Global variable: $name\n";
   }
   if ($GFlag && $name =~ m{^[a-zA-Z0-9_]+$} && $len > $GFlag) {
      print STDERR "Global array: $name\[$len\]\n";
   }
   if ($SFlag && $name =~ s{^([a-zA-Z0-9_]+)\.\d+$}{$1} && $len > $SFlag) {
      print STDERR "Static local array: $1\[$len\]\n";
   }
}
