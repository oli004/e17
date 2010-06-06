#!/bin/sh

set -e

. ./SCRIPTS/_list_snap.sh

OUT="$PWD/SNAPSHOTS"

for I in $PROJ; do
  ./SCRIPTS/_pre_snap.sh "$I" "$VA" "$VB"
done
for I in $PROJ; do
  ./SCRIPTS/_dist.sh "$I" "$OUT"
done
for I in $PROJ; do
  ./SCRIPTS/_post_snap.sh "$I" "$VA" "$VB"
done

exit 0
## upload
DATE=`date +'%F'`
SVR="www.enlightenment.org"
DST="/var/www/download/snapshots"
SSH="ssh -p 995"
SCP="ssh -P 995"

$SSH "$SVR" mkdir "$DST/$DATE"
$SSH "$SVR" rm -rf "$DST/LATEST"
$SSH "$SVR" ln -s "$DATE" "$DST/LATEST"
$SCP "$OUT/"* "$SVR:$DST/LATEST/"

exit 0
