# This file is part of Mongoose project, http://code.google.com/p/mongoose
# $Id: Makefile 473 2009-09-02 11:20:06Z valenok $

PROG=	mongoose

all:
	@echo "make (linux|bsd|solaris|mac|windows|mingw)"

# Possible COPT values: (in brackets are rough numbers for 'gcc -O2' on i386)
# -DHAVE_MD5		- use system md5 library (-2kb)
# -DNDEBUG		- strip off all debug code (-5kb)
# -DDEBUG		- build debug version (very noisy) (+7kb)
# -DNO_CGI		- disable CGI support (-5kb)
# -DNO_SSL		- disable SSL functionality (-2kb)
# -DCONFIG_FILE=\"file\" - use `file' as the default config file
# -DHAVE_STRTOUI64	- use system strtoui64() function for strtoull()


##########################################################################
###                 UNIX build: linux, bsd, mac, rtems
##########################################################################

CFLAGS=		-W -Wall -std=c99 -pedantic -O2 -fomit-frame-pointer $(COPT)
MAC_SHARED=	-flat_namespace -bundle -undefined suppress
LINFLAGS=	-ldl -pthread $(CFLAGS)
LIB=		_$(PROG).so

linux:
	$(CC) $(LINFLAGS) mongoose.c -shared -fPIC -fpic -o $(LIB)
	$(CC) $(LINFLAGS) mongoose.c main.c -o $(PROG)

bsd:
	$(CC) $(CFLAGS) mongoose.c -shared -pthread -fpic -fPIC -o $(LIB)
	$(CC) $(CFLAGS) mongoose.c main.c -pthread -o $(PROG)

mac:
	$(CC) $(CFLAGS) $(MAC_SHARED) mongoose.c -pthread -o $(LIB)
	$(CC) $(CFLAGS) mongoose.c main.c -pthread -o $(PROG)

solaris:
	gcc $(CFLAGS) mongoose.c -pthread -lnsl \
		-lsocket -fpic -fPIC -shared -o $(LIB)
	gcc $(CFLAGS) mongoose.c main.c -pthread -lnsl -lsocket -o $(PROG)


##########################################################################
###            WINDOWS build: Using Visual Studio or Mingw
##########################################################################

# Using Visual Studio 6.0. To build Mongoose:
#  o  Set VC variable below to where VS 6.0 is installed on your system
#  o  Run "PATH_TO_VC6\bin\nmake windows"

VC=	q:\vc6
#DBG=	/Zi /DDEBUG /Od /DDEBUG
DBG=	/DNDEBUG /Os
CL=	$(VC)\bin\cl.exe  /MD /TC /nologo /W4 /c $(DBG) /I $(VC)/include
LINK=	$(VC)\bin\link.exe /incremental:no /libpath:$(VC)\lib ws2_32.lib
OUT=	c:\out
DLL=	$(OUT)\_$(PROG).dll
EXE=	$(OUT)\$(PROG).exe

windows:
	$(CL) main.c /Fd$(OUT)\main.pdb /Fo$(OUT)\main.obj
	$(CL) mongoose.c /Fd$(OUT)\mongoose.pdb /Fo$(OUT)\mongoose.obj
	$(LINK) $(OUT)\mongoose.obj /DLL /DEF:win32\dll.def /out:$(DLL)
	$(LINK) $(OUT)\main.obj $(OUT)\mongoose.obj /out:$(EXE)

# Build for Windows under MinGW
#MINGWDBG= -DDEBUG -O0
MINGWDBG= -DNDEBUG -Os
MINGWOPT= -W -Wall -mthreads -Wl,--subsystem,console $(MINGWDBG) -DHAVE_STDINT
mingw:
	gcc $(MINGWOPT) mongoose.c -lws2_32 \
		-shared -Wl,--out-implib=$(PROG).lib -o _$(PROG).dll
	gcc $(MINGWOPT) mongoose.c main.c -lws2_32 -ladvapi32 -o $(PROG).exe


##########################################################################
###            Manuals, cleanup, test, release
##########################################################################

man:
	groff -man -T ascii mongoose.1 | col -b > mongoose.txt
	groff -man -T ascii mongoose.1 | less

# "TEST=unit make test" - perform unit test only
# "TEST=embedded" - test embedded API by building and testing test/embed.c
# "TEST=basic_tests" - perform basic tests only (no CGI, SSI..)
test: do_test
do_test:
	perl test/test.pl $(TEST)

release: clean
	F=mongoose-`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`.tgz ; cd .. && tar --exclude \*.hg --exclude \*.svn --exclude \*.swp --exclude \*.nfs\* --exclude win32 -czf x mongoose && mv x mongoose/$$F

clean:
	rm -rf *.o *.core $(PROG) *.obj $(PROG).txt *.dSYM *.tgz
