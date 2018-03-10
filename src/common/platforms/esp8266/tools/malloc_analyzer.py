#!/usr/bin/python

# Analyzers output of firmware compiler with -DESP_ENABLE_MALLOC_TRACES

import re
import sys

allocs = {}

for l in sys.stdin:
    m = re.match(r'(ca|ma|re|fr) (\S+)\s*(\S*)\s*(\S*)', l)
    if not m: continue
    op = m.group(1)
    if op in ('ca', 'ma'):
        allocs[m.group(2)] = long(m.group(3))
    else:
        if m.group(2) in allocs:
            del allocs[m.group(2)]
        if op == 're':
            allocs[m.group(3)] = long(m.group(4))

for k, v in sorted(allocs.iteritems()):
    print k, v
