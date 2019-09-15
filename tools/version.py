#!/usr/bin/env python
#
# This script extracts the mongoose version from mongoose.h, which is the master
# location for this information.
#

import argparse
import re
import sys
import os
version_re = re.compile(r'#define MG_VERSION \"(.*)\"')

parser = argparse.ArgumentParser(description='Extract the mongoose version')
parser.add_argument('file', default='mongoose.h',
                    nargs='?', help='mongoose header file')
parser.add_argument('--src_dir', type=str, default="src",
                    help='mongoose src dir')
parser.add_argument('--use_src', action='store_true',
                    help='get version from src dir')

if __name__ == '__main__':
    args = parser.parse_args()

    version_file = args.file
    if args.use_src:
        source_dir = args.src_dir
        version_file = os.path.join(source_dir, "mg_common.h")

    with open(version_file) as f:
        for l in f:
            m = version_re.match(l)
            if m:
                sys.stdout.write(m.group(1))
