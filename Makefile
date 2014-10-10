#
# Created by gmakemake (Ubuntu Sep  7 2011) on Fri Apr 18 23:28:36 2014
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CC =		gcc
CFLAGS =	-Wall -ggdb -std=c99

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	dlList.c dlListExtended.c dlTest.c mrEd.c testdllist.c
PS_FILES =	
S_FILES =	
H_FILES =	dlList.h dlListExtended.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	dlList.o dlListExtended.o 

#
# Main targets
#

all:	dlTest mrEd testdllist 

dlTest:	dlTest.o $(OBJFILES)
	$(CC) $(CFLAGS) -o dlTest dlTest.o $(OBJFILES) $(CLIBFLAGS)

mrEd:	mrEd.o $(OBJFILES)
	$(CC) $(CFLAGS) -o mrEd mrEd.o $(OBJFILES) $(CLIBFLAGS)

testdllist:	testdllist.o $(OBJFILES)
	$(CC) $(CFLAGS) -o testdllist testdllist.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

dlList.o:	dlList.h dlListExtended.h
dlListExtended.o:	dlList.h dlListExtended.h
dlTest.o:	dlList.h dlListExtended.h
mrEd.o:	dlList.h dlListExtended.h
testdllist.o:	dlList.h dlListExtended.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) dlTest.o mrEd.o testdllist.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf dlTest mrEd testdllist 
