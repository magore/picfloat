#=========================================================================
# 2002,2003 (C) Mike Gore

LIBS = ./psim-0.4/psim.a ../utils/num.a -lm

PROGS = eparanoi fptest fppow fpint
# FPBIG controls floating point format 40bit TR or 32bit when not defined
CFLAGS= -I./psim-0.4/ -I./ 
CC=gcc

all:	$(PROGS)

# GET TARGET TEST FILE
dummy: ../picfp.hex ../picfp.lst
	cp -f ../picfp.lst .
	cp -f ../picfp.hex .
	cp -f psim-0.4/psim .
	./GETSYMS picfp.lst picfp.h

# Compile simulator to run PARANOIA TEST
# Setting FPBIG here will break things due to lack of guard digits
eparanoi:	dummy
	$(CC) $(CFLAGS) -DPIC_TEST -DPOW_BUG -DPIC_FPE paranoia/eparanoi.c $(LDFLAGS) -o $@ $(LIBS)
    
# Compile simulator to test trig tests
fptest:		dummy
	$(CC) $(CFLAGS) -DFPBIG fptest.c $(LDFLAGS) -o $@ $(LIBS)

# Compile simulator to run floating/integer tests
fpint:		dummy
	$(CC) $(CFLAGS) -DFPBIG fpint.c $(LDFLAGS) -o $@ $(LIBS)

# Compile simulator to run pow,log,exp tests
fppow:		dummy
	$(CC) $(CFLAGS) -DFPBIG -DPIC_TEST -DPOW_BUG fppow.c $(LDFLAGS) -o $@ $(LIBS)

# Run simulator to test floating/integer tests
run_fpint:	fpint
	./fpint picfp.hex

# Run simulator to test pow,log,exp tests
run_fppow:	fppow
	./fppow picfp.hex

# Run simulator to test trig tests
run_fptest:	fptest
	./fptest picfp.hex

# Run simulator to test PARANOI tests
run_eparanoi: eparanoi
	./eparanoi picfp.hex

# Runs all tests into baseline files
baseline:	all 
	-./eparanoi picfp.hex >baseline/eparanoi.txt
	-./fptest picfp.hex	>baseline/fptest.txt
	-./fppow picfp.hex	>baseline/fppow.txt
	-./fpint picfp.hex	>baseline/fpint.txt

# Run the simulator on our internal asm code
run_psim:	dummy
	./psim picfp.hex <cmds

clean:
	rm -f $(PROGS) picfp.hex picfp.lst picfp.lst picfp.h psim
