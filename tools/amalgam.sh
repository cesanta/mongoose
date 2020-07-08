#!/bin/bash

set -x -e

tools/amalgam.py --prefix=MG --license=LICENSE $(cat src/mongoose.h.manifest) > mongoose.h
tools/amalgam.py --prefix=MG --public-header=mongoose.h --license=LICENSE $(cat src/mongoose.c.manifest) > mongoose.c
