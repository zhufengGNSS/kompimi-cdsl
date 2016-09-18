#!/bin/bash

# Release build
echo 'CFLAGS=-O3 -DNDEBUG' > Makefile.custom
make clean all

# Profiling build
echo 'CFLAGS=-O3 -DNDEBUG -pg' > Makefile.custom
make clean all

# Compliance checking for C89, C99, C++, GNU dialects
echo 'CFLAGS=-Wall -std=iso9899:1990   -pedantic -g -O2' > Makefile.custom
make clean all
echo 'CFLAGS=-Wall -std=iso9899:199409 -pedantic -g -O2' > Makefile.custom
make clean all
echo 'CFLAGS=-Wall -std=iso9899:1999   -pedantic -g -O2' > Makefile.custom
make clean all
echo 'CFLAGS=-Wall -std=iso9899:1999   -pedantic -g -O2' > Makefile.custom
make clean all
echo 'CFLAGS=-Wall -std=gnu99          -pedantic -g -O2' > Makefile.custom
make clean all
echo 'CC=g++' > Makefile.custom
echo 'CFLAGS=-Wall -std=c++98          -pedantic -g -O2' >> Makefile.custom
make clean all
echo 'CC=g++' > Makefile.custom
echo 'CFLAGS=-Wall -std=gnu++98        -pedantic -g -O2' >> Makefile.custom
make clean all
