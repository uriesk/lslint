#!/bin/sh
failed=.
if [ -e ./test.total.txt ] ; then
    rm -f ./test.total.txt
fi

for f in scripts/*.lsl scripts/*/*.lsl ; do
  printf "%40s ... " "$f"
  if [ "${f#scripts/mono/}" != "$f" ] ; then
    ./lslint -m -\# -A "$f" > ./test.run.txt 2>&1
  elif [ "${f#scripts/lso/}" != "$f" ] ; then
    ./lslint -m- -\# -A "$f" > ./test.run.txt 2>&1
  elif [ "${f#scripts/preproc/}" != "$f" ] ; then
    ./lslint -i -\# -A "$f" > ./test.run.txt 2>&1
  elif [ "${f#scripts/uep/}" != "$f" ] ; then
    ./lslint -u -\# -A "$f" > ./test.run.txt 2>&1
  elif [ "${f#scripts/switch/}" != "$f" ] ; then
    ./lslint -w -\# -A "$f" > ./test.run.txt 2>&1
  else
    # test in both mono and lso modes
    ./lslint -m -\# -A "$f" > ./test.run.txt 2>&1 \
    && ./lslint -m- -\# -A "$f" > ./test.run.txt 2>&1
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
