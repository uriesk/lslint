#!/bin/sh
failed=.
if [ -e ./test.total.txt ] ; then
    rm -f ./test.total.txt
fi

for f in scripts/*.lsl scripts/*/*.lsl ; do
  printf "%40s ... " "$f"
  if [ "${f%.mono.lsl}" != "$f" ] ; then
    ./lslint -m -\# -A "$f" > ./test.run.txt 2>&1
  elif [ "${f%.lso.lsl}" != "$f" ] ; then
    ./lslint -m- -\# -A "$f" > ./test.run.txt 2>&1
  else
    # test in both modes
    ./lslint -m -\# -A "$f" > ./test.run.txt 2>&1
    ./lslint -m- -\# -A "$f" > ./test.run.txt 2>&1
  fi
  if [ $? != 0 ] ; then
      echo "FAILED"
      echo "" >> ./test.total.txt
      echo "****************" >> ./test.total.txt
      echo '***>' "$f" >> ./test.total.txt
      echo "" >> ./test.total.txt
       cat ./test.run.txt >> ./test.total.txt
      failed=$failed.
  else
      echo "passed"
  fi
done

rm -f ./test.run.txt

if [ $failed != . ] ; then
  cat ./test.total.txt
  rm -f ./test.total.txt
  exit 1
fi
