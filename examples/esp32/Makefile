THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../..)

all: example

example:
	docker run --rm -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf idf.py build

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb mongoose mongoose_* mongoose.* build sdkconfig
