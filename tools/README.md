# Amalgamation

Mongoose is distributed as two files, `mongoose.c` and `mongoose.h` for ease of integration.
However, when developing Mongoose itself, it can be quite difficult to work with them.
Internally, these files are an _amalgamation_ of source and header modules.
This directory contains utilities to split and re-constitute amalgamated files.

Here's how `mongoose.c` can be split into its consituent parts:
```
$ tools/unamalgam.py mongoose.c
=> src/mg_internal.h
=> src/common/mg_mem.h
...
```

This produces directories and files under `mongoose/` and `common/` that are easeier to work with.
It also produces `mongoose.c.manifest` which can later be used to reconstruct the file back:
```
$ tools/amalgam.py --prefix=MG --public-header=mongoose.h $(cat mongoose.c.manifest) > mongoose.c
```

The same applies to `mongoose.h`, except `--public-header` should be omitted during amalgamation.

`tools/amalgam.sh` can be used to assemble `mongoose.c` and `mongoose.h`.
