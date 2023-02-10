#!/bin/bash
#
# This script is used by the Github Actions
#   o must be run with one parameter: a glob pattern
#   o write "MATCH=1" (changed files match a pattern) or "MATCH=0" to $GITHUB_OUTPUT
#
# Usage in github actions:
#  <job_name>:
#    runs-on: ubuntu-latest
#    steps:
#    - uses: actions/checkout@v3
#      with: { fetch-depth: 2 }         # Important for git diff to work
#    - id: check
#      run: /bin/bash check.sh '.*'
#    - if: steps.check.outputs.MATCH == 1
#      run: .....

# List all files changed by the last commit, and compare with the passed
# pattern argument. If at least one file matches the pattern, write
# MATCH=1. If no files match, write MATCH=0.
for FILE in $(git --no-pager diff --name-only HEAD~1 HEAD) ; do
  if [[ "$FILE" =~ $1 ]] ; then
    echo FILE "$FILE" matches "$1"    # Log for debugging
    echo MATCH=1 >> $GITHUB_OUTPUT    # Set output
    exit 0                            # And exit early
  else
    echo FILE "$FILE" DOES NOT match "$1"  # Log for debugging
  fi
done
echo MATCH=0 >> $GITHUB_OUTPUT
