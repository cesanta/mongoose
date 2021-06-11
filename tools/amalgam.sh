#!/bin/bash

set -x -e

tools/amalgam.py --prefix=MG --license=LICENSE $(cat src/mongoose.h.manifest) > mongoose.h
tools/amalgam.py --prefix=MG --license=LICENSE --public-header=mongoose.h $(cat src/mongoose.c.manifest) > mongoose.c
