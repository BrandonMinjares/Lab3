BASEDIR=$(dirname "$0")
CODE=`shuf -i 1024-2048 -n 1`
echo "############## Return Code: $CODE ##################"
cat $BASEDIR/child-simple.in | sed "/return 81/s//return $CODE/" > $BASEDIR/child-simple.c
for f in `grep -l CODE $BASEDIR/*.ck.in`  
do
  echo $f
  cat $f | sed "/CODE/s//$CODE/" > `echo "${f%.*}"`
done