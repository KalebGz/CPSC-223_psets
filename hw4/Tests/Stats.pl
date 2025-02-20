#!/bin/bash
#
#    % Stats.pl [-POS[,LEN]] FILE*
#
# Run QsortH to sort the specified files and report the #lines, #compares,
# #addD's, #pushD's, and #remD's required.
#
# If -POS[,LEN] is specified, the correctness test is applied without it.

TESTS=/c/cs223/Hwk4/Tests
export LC_ALL=C

STDOUT=/tmp/Qsort.stdout.$$
STDERR=/tmp/Qsort.stderr.$$
trap "/bin/rm -f $STDOUT $STDERR" 0 1 2 3 9 15

########################
# Capture optional [-POS[,LEN]]
POSLEN=
case $1 in
  -*) POSLEN=$1; shift
esac

########################
# Count the number of lines in the files
echo "File(s): $*"
echo

NLIN=`sort $* | wc -l`
echo "#lines = $NLIN"

########################
# Verify that QsortH can sort correctly
./QsortH $*                  2>$STDERR  >$STDOUT

sort -c $STDOUT              2>>$STDERR
sort $* | diff -q - $STDOUT  2>>$STDERR >&2

if (test -s $STDERR -o -v DEBUG_SCRIPT) then
  echo "WARNING: QsortH failed to sort $* correctly."
  echo ----- ; cat $STDERR ; echo -----
  exit
fi

########################
# Count #compares required
LD_PRELOAD=$TESTS/libxCompare.so \
 ./QsortH $POSLEN $*         2>$STDERR  >$STDOUT
NCMP=`cat $STDERR`
echo "#compares = $NCMP"
echo

LIMIT=$((2 * ($NCMP + $NLIN)))
echo "2*(#compares+#lines) = $LIMIT"

LIMIT=$((4 * ($NCMP + $NLIN)))
echo "4*(#compares+#lines) = $LIMIT"
echo

########################
# Sort unsorted file
XDEQUE_SUMMARY=                  \
 ./QsortH $POSLEN $*         2>$STDERR  >$STDOUT

perl -p -e 's{^xDeque: Total }{}' $STDERR
