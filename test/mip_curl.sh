curl -s $*/a.txt -o /tmp/a.txt && diff /tmp/a.txt data/a.txt && rm /tmp/a.txt &&
curl -s -H Expect: $*/body --data-binary @./Makefile -o /tmp/Makefile && diff /tmp/Makefile ./Makefile && rm /tmp/Makefile
