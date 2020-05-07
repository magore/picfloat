CC=gcc
CFLAGS=-W
LIBS=-ml

PROGS= mknum mktab ieee2tr

all:	$(PROGS)

OBJS= prtnum.o cvtnum.o

ieee2tr:	ieee2tr.c num.a
	gcc ieee2tr.c num.a -o ieee2tr

mknum:	mknum.c num.a
	gcc mknum.c num.a -o mknum

mktab:	mktab.c num.a
	gcc mktab.c num.a -o mktab

num.a:	$(OBJS)
		ar rc num.a $(OBJS)
		ranlib num.a

prtnum.o:	prtnum.c prtnum.h

cvtnum.o:	cvtnum.c cvtnum.h

clean:
	rm -f $(PROGS) num.a $(OBJS)
