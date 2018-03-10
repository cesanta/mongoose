#!/usr/bin/python

import sys

for line in open(sys.argv[1]):
    i = 0
    while i < len(line):
        n = len(line) - i
        if n > 80: n = 80
        l = line[i:i+n]
        if n == 80:
            l += '\\'
        print l
        i += n
