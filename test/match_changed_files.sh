#!/bin/bash
#
# Usage: $0 PATTERN
# If files in the last commit match PATTERN, exit 0. Else, exit 1
#
# Usage in github actions:
#  <job_name>:
#    runs-on: ubuntu-latest
#    steps:
#    - uses: actions/checkout@v3
#      with: { fetch-depth: 2 }         # Important for git diff to work
#    - run: if ./match_changed_files.sh '.*' ; then echo RUN=1 >> $GITHUB_ENV ; fi
#    - if: ${{ env.RUN == 1 }}
#      run: echo yohoo!

CHANGED_FILES=`git --no-pager diff --name-only HEAD~1 HEAD | xargs`
echo "Pattern: [$1], files: $CHANGED_FILES"
for FILE in $CHANGED_FILES; do [[ "$FILE" =~ $1 ]] && exit 0; done
exit 1 
