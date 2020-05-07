# Makefile for PSIM package
# Copyright 1997, 2000 Eric L. Smith <brouhaha@users.sourceforge.net>
#
# PSIM is free software; you can redistribute it and/or modify it under the
# terms of the GNU General Public License version 2 as published by the Free
# Software Foundation.  Note that I am not granting permission to redistribute
# or modify PSIM under the terms of any later version of the General Public
# License.
# 
# This program is distributed in the hope that it will be useful (or at
# least amusing), but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program (in the file "COPYING"); if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# $Id: Makefile,v 1.1.1.1 2003/12/14 03:40:05 magore Exp $

# -----------------------------------------------------------------------------
# You may want or need to change the following definitions.
# -----------------------------------------------------------------------------
# 2002,2003 Mike Gore
# Added Changes to allow the PSIM core to be used by external programs
# removed all simulator code out of main.c and moved to psim.c
# -----------------------------------------------------------------------------

TEST=TRUE

CC=gcc
SIM_OPTIONS = -DSIM_INST_SET 
# VIDEO = 1
LIBS = -lm

DEBUG_OPTIONS = -g

ifdef VIDEO
LDFLAGS = -L/usr/X11R6/lib
LIBS = -lX11 -lm
CFLAGS = $(DEBUG_OPTIONS) $(SIM_OPTIONS) -DVIDEO -I/usr/X11R6/include
else
CFLAGS = $(DEBUG_OPTIONS) $(SIM_OPTIONS)
endif

# -----------------------------------------------------------------------------
# You shouldn't have to change anything below this point, but if you do please
# let me know why so I can improve this Makefile.
# -----------------------------------------------------------------------------

PACKAGE = psim
VERSION = 0.4
DSTNAME = $(PACKAGE)-$(VERSION)

MISC = COPYING Makefile README TODO

HEADERS = ihexin.h pic.h display.h

SOURCES = psim.c debug.c display.c ihexin.c main.c picadc.c picdis.c picinst.c \
	picio.c picmem.c pictmr0.c

ifdef VIDEO
OBJECTS = psim.o picinst.o picmem.o picdis.o picio.o picadc.o pictmr0.o \
      display.o debug.o ihexin.o
else
OBJECTS = psim.o picinst.o picmem.o picdis.o picio.o picadc.o pictmr0.o \
      debug.o ihexin.o
endif

DISTFILES = $(MISC) $(SOURCES) $(HEADERS)
all:	psim

# Compile simulator to run built in test in pic code file itself
psim:	psim.a 
	$(CC) $(LDFLAGS) main.c -o $@ psim.a $(LIBS)

# Build a library of psim object files
psim.a: $(OBJECTS)
	ar rc psim.a $(OBJECTS)
	ranlib psim.a

clean:
	-rm -f $(OBJECTS) psim.a psim

dist: $(DISTFILES)
	-rm -rf $(DSTNAME)
	mkdir $(DSTNAME)
	for f in $(DISTFILES); do ln $$f $(DSTNAME)/$$f; done


picsim.tar.gz: $(MISC) $(SOURCES) $(HEADERS)
	tar -czf picsim.tar.gz $(MISC) $(SOURCES) $(HEADERS)


ifdef VIDEO
main.o: pic.h display.h
else
main.o: pic.h
endif


picinst.o: pic.h

picdis.o: pic.h

picmem.o: pic.h ihexin.h

picio.o: pic.h

picadc.o: pic.h

pictmr0.o: pic.h

display.o: pic.h

debug.o: pic.h

ihexin.o: ihexin.h
