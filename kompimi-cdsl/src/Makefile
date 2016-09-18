# Originally distributed as part of the Kompimi C Data Structure Library.
# For updates, see: https://sourceforge.net/projects/kompimi-cdsl/
#
# All content in this document is granted into the public
# domain. Where this is not legally possible, the copyright owner
# releases all rights. This notice may be modified or removed.

ROOTDIR=..
DERIVEDDIR=$(ROOTDIR)/derived
OBJDIR=$(DERIVEDDIR)/obj
BINDIR=$(DERIVEDDIR)/bin
DOCDIR=$(DERIVEDDIR)/docs

# Currently used only for doc generation
SOURCES=dynamic_array.c \
	list.c \
        tests/dynamic_array_test.c \
	tests/list_test.c

HEADERS=dynamic_array.h \
	list.h \
        config.h

CC=gcc
CFLAGS=-Wall -g -O2
LFLAGS=
LIBFLAGS=

-include Makefile.custom

all: testbins perftestbins docs

testbins: $(BINDIR)/tests/dynamic_array_test \
	  $(BINDIR)/tests/list_test

perftestbins: $(BINDIR)/tests/list_perf_test

runtests: testbins
	$(BINDIR)/tests/dynamic_array_test
	$(BINDIR)/tests/list_test

runperftests: perftestbins
	$(BINDIR)/tests/list_perf_test

clean:
	rm -f -r $(DERIVEDDIR)

docs: $(DOCDIR)/html/index.html

# Directory creator rules - uses temporary file to avoid spurious rebuilds

$(DERIVEDDIR)/made:
	mkdir $(DERIVEDDIR)
	touch $(DERIVEDDIR)/made

$(OBJDIR)/made: $(DERIVEDDIR)/made
	mkdir $(OBJDIR)
	touch $(OBJDIR)/made

$(OBJDIR)/tests/made: $(OBJDIR)/made
	mkdir $(OBJDIR)/tests
	touch $(OBJDIR)/tests/made

$(BINDIR)/made: $(DERIVEDDIR)/made
	mkdir $(BINDIR)
	touch $(BINDIR)/made

$(BINDIR)/tests/made: $(BINDIR)/made
	mkdir $(BINDIR)/tests
	touch $(BINDIR)/tests/made

$(DOCDIR)/made: $(DERIVEDDIR)/made
	mkdir $(DOCDIR)
	touch $(DOCDIR)/made

# Docs builder using doxygen

$(DOCDIR)/html/index.html: $(DOCDIR)/made $(SOURCES) $(HEADERS) doxygen.cfg
	cp doxygen.cfg $(DOCDIR)
	echo 'OUTPUT_DIRECTORY = $(DOCDIR)' >> $(DOCDIR)/doxygen.cfg
	echo 'INPUT = $(SOURCES) $(HEADERS)' >> $(DOCDIR)/doxygen.cfg
	doxygen $(DOCDIR)/doxygen.cfg

# Test binaries

OBJS=$(OBJDIR)/dynamic_array.o $(OBJDIR)/list.o

$(BINDIR)/tests/dynamic_array_test: $(BINDIR)/tests/made $(OBJS) $(OBJDIR)/tests/dynamic_array_test.o
	$(CC) $(LFLAGS) $(OBJS) $(OBJDIR)/tests/dynamic_array_test.o -o $(BINDIR)/tests/dynamic_array_test $(LIBFLAGS)

$(BINDIR)/tests/list_test: $(BINDIR)/tests/made $(OBJS) $(OBJDIR)/tests/list_test.o
	$(CC) $(LFLAGS) $(OBJS) $(OBJDIR)/tests/list_test.o -o $(BINDIR)/tests/list_test $(LIBFLAGS)

$(BINDIR)/tests/list_perf_test: $(BINDIR)/tests/made $(OBJS) $(OBJDIR)/tests/perf_test.o $(OBJDIR)/tests/list_perf_test.o $(OBJDIR)/tests/dllist.o
	$(CC) $(LFLAGS) $(OBJS) $(OBJDIR)/tests/perf_test.o $(OBJDIR)/tests/dllist.o $(OBJDIR)/tests/list_perf_test.o -o $(BINDIR)/tests/list_perf_test $(LIBFLAGS)

# Object files

$(OBJDIR)/dynamic_array.o: $(OBJDIR)/made dynamic_array.c dynamic_array.h config.h
	$(CC) $(CFLAGS) -c dynamic_array.c -o $(OBJDIR)/dynamic_array.o

$(OBJDIR)/list.o: $(OBJDIR)/made list.c list.h config.h
	$(CC) $(CFLAGS) -c list.c -o $(OBJDIR)/list.o

$(OBJDIR)/tests/dynamic_array_test.o: $(OBJDIR)/tests/made tests/dynamic_array_test.c dynamic_array.h config.h
	$(CC) $(CFLAGS) -c tests/dynamic_array_test.c -o $(OBJDIR)/tests/dynamic_array_test.o

$(OBJDIR)/tests/list_test.o: $(OBJDIR)/tests/made tests/list_test.c list.h config.h
	$(CC) $(CFLAGS) -c tests/list_test.c -o $(OBJDIR)/tests/list_test.o

$(OBJDIR)/tests/perf_test.o: $(OBJDIR)/tests/made tests/perf_test.c tests/perf_test.h
	$(CC) $(CFLAGS) -c tests/perf_test.c -o $(OBJDIR)/tests/perf_test.o

$(OBJDIR)/tests/dllist.o: $(OBJDIR)/tests/made tests/dllist.c tests/dllist.h
	$(CC) $(CFLAGS) -c tests/dllist.c -o $(OBJDIR)/tests/dllist.o

$(OBJDIR)/tests/list_perf_test.o: $(OBJDIR)/tests/made tests/list_perf_test.c list.h tests/perf_test.h tests/dllist.h config.h
	$(CC) $(CFLAGS) -c tests/list_perf_test.c -o $(OBJDIR)/tests/list_perf_test.o
