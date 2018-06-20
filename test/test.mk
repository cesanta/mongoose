# Copyright (c) 2014 Cesanta Software Limited
# All rights reserved
#
# = Requires:
#
# - SRC_DIR = path to directory with source files
# - AMALGAMATED_SOURCES = path to amalgamated C file(s)
# - PROG = name of main unit test source file
#
# - CFLAGS = default compiler flags
# - LDFLAGS = default linker flags
#
# = Parameters:
#
# - V=1 -> show commandlines of executed commands
# - TEST_FILTER -> test name (substring match)
# - CMD -> run wrapped in cmd (e.g. make test_cxx CMD=lldb)
#
# = Useful targets
#
# - compile:   perform a very fast syntax check for all dialects
# - presubmit: suggested presubmit tests
# - cpplint:   run the linter
# - lcov:      generate coverage HTML in test/lcov/index.html
# - test_asan: run with AddressSanitizer
# - test_valgrind: run with valgrind

# OSX clang doesn't build ASAN. Use brew:
#  $ brew tap homebrew/versions
#  $ brew install llvm36 --with-clang --with-asan
CLANG:=clang

PEDANTIC=$(shell gcc --version 2>/dev/null | grep -q clang && echo -pedantic)

###

DIALECTS=cxx ansi c99 c11
SPECIALS=asan gcov valgrind

# Each test target might require either a different compiler name
# a compiler flag, or a wrapper to be invoked before executing the test
# they can be overriden here with <VAR>_<target>
#
# Vars are:
# - CC: compiler
# - CFLAGS: flags passed to the compiler, useful to set dialect and to disable incompatible tests
# - LDFLAGS: flags passed to the compiler only when linking (e.g. not in syntax only)
# - SOURCES: non-test source files. To be overriden if needs to build on non amalgamated files
# - CMD: command wrapper or env variables required to run the test binary

CMD=MallocLogFile=/dev/null

CC_cxx=$(CXX)
CFLAGS_cxx=-x c++

CFLAGS_ansi=-ansi
CFLAGS_c99=$(PEDANTIC) -std=c99

# Path to `gcov` binary
GCOV?=gcov

CFLAGS_gcov=$(PEDANTIC) -std=c99 -fprofile-arcs -ftest-coverage
#SOURCES_gcov=$(addprefix $(SRC_DIR)/, $(SOURCES))
SOURCES_gcov=$(AMALGAMATED_SOURCES)

CC_asan=$(CLANG)
CFLAGS_asan=-fsanitize=address -fcolor-diagnostics -std=c99 -DNO_DNS_TEST -UMG_ENABLE_SSL
CMD_asan=ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer ASAN_OPTIONS=allocator_may_return_null=1,symbolize=1 $(CMD)

CMD_valgrind=valgrind

###

SHELL := /bin/bash

SUFFIXES=$(DIALECTS) $(SPECIALS)

ALL_PROGS=$(foreach p,$(SUFFIXES),$(PROG)-$(p))
ALL_TESTS=$(foreach p,$(SUFFIXES),test_$(p))
SHORT_TESTS=$(foreach p,$(DIALECTS),test_$(p))

all: clean compile $(SHORT_TESTS) coverage
alltests: $(ALL_TESTS) lcov cpplint

# currently both valgrind and asan tests are failing for some test cases
# it's still useful to be able to run asan/valgrind on some specific test cases
# but we don't enforce them for presubmit until they are stable again.
presubmit: $(SHORT_TESTS) cpplint

.PHONY: clean clean_coverage lcov valgrind docker cpplint test_sources_resolved
ifneq ($(V), 1)
.SILENT: $(ALL_PROGS) $(ALL_TESTS)
endif

compile:
	@make $(foreach p,$(DIALECTS),$(PROG)-$(p)) CFLAGS_EXTRA="$(CFLAGS_EXTRA) -fsyntax-only" LDFLAGS=

# A target which is only needed to VPATH-resolve $(TEST_SOURCES).
# Other targets which need exact (VPATH-resolved) paths to $(TEST_SOURCES),
# need to depend on this phony target, and then use $(TEST_SOURCES_RESOLVED)
# instead.
test_sources_resolved: $(TEST_SOURCES)
	$(eval TEST_SOURCES_RESOLVED=$^)

# HACK: cannot have two underscores
$(PROG)-%: Makefile test_sources_resolved $(or $(SOURCES_$*), $(AMALGAMATED_SOURCES)) data/cgi/index.cgi
	@echo -e "CC\t$(PROG)_$*"
	$(or $(CC_$*), $(CC)) $(CFLAGS_$*) $(TEST_SOURCES_RESOLVED) $(or $(SOURCES_$*), $(AMALGAMATED_SOURCES)) -o $(PROG)_$* $(CFLAGS) $(LDFLAGS)

$(ALL_TESTS): test_%: Makefile $(PROG)-%
	@echo -e "RUN\t$(PROG)_$* $(TEST_FILTER)"
	@$(or $(CMD_$*), $(CMD)) ./$(PROG)_$* $(TEST_FILTER)

coverage: Makefile clean_coverage test_gcov test_sources_resolved
	@echo -e "RUN\tGCOV"
	@$(GCOV) -p $(notdir $(TEST_SOURCES_RESOLVED)) $(notdir $(GCOV_SOURCES)) >/dev/null

test_leaks: Makefile
	$(MAKE) test_valgrind CMD_valgrind="$(CMD_valgrind) --leak-check=full"

lcov: clean coverage
	@echo -e "RUN\tlcov"
	@lcov -q -o lcov.info -c -d . 2>/dev/null
	@genhtml -q -o lcov lcov.info

cpplint:
	@echo -e "RUN\tcpplint"
	@cpplint.py --verbose=0 --extensions=c,h $(SRC_DIR)/*.[ch] 2>&1 >/dev/null| grep -v "Done processing" | grep -v "file excluded by"

clean: clean_coverage
	@echo -e "CLEAN\tall"
	@rm -rf $(PROG) $(PROG)_* lcov.info *.txt *.exe *.obj *.o a.out *.pdb *.opt $(EXTRA_CLEAN_TARGETS)

clean_coverage:
	@echo -e "CLEAN\tcoverage"
	@rm -rf *.gc* *.dSYM index.html
