# Make file for picfp.asm - Floating point main with testing code


SOURCES = \
ac32.asm  atoi.asm   fp.asm    fpsup.asm  io.asm   math_tab.h  table0.h \
ac32.h    atoi.h     fpfn.asm  fpsup.h    io.h     mem.h       table.h \
atof.asm  bcd32.asm  fpfn.h    ftoa.asm   macro.h  picfp.asm   tablesup.h \
atof.h    bcd32.h    fp.h      ftoa.h     math.h   stack.h 


all:	builtin docs baseline

# Compile all builtin functions and run simulator on them
# Save results in tests/builtin
builtin: utils $(SOURCES) Makefile
# Compile builtin ATOF/FTOA testing and start the simulator
	gpasm picfp.asm	-DATOF_TEST -DFP_DEBUG
	@$(MAKE) -C tests run_psim >tests/builtin/atof_test.txt
# Compile builtin SQRT/LOG/EXP testing and start the simulator
	gpasm picfp.asm	-DBASIC_TEST
	@$(MAKE) -C tests run_psim >tests/builtin/basic_test.txt
# Compile builtin TRIG SIN/COS/TAN testing and start the simulator
	gpasm picfp.asm	-DTRIG_TEST
	@$(MAKE) -C tests run_psim >tests/builtin/trig_test.txt

# Compile empty builtin and run all compiled simulator tests 
# Save results in tests/baseline
.PHONY: baseline
baseline: utils Makefile
	gpasm picfp.asm -DFP_DEBUG
	@$(MAKE) -C tests $@ baseline

# Extract documentation headers
.PHONY: docs
docs: 
	./mkdocs docs/headers.txt 


# Make utilities
.PHONY: utils
utils: 
	@$(MAKE) -C tests/psim-0.4 all
	@$(MAKE) -C utils all

clean:
	-rm -f picfp.hex picfp.cod picfp.lst
	@$(MAKE) -C tests/psim-0.4 $@
	@$(MAKE) -C tests $@
	@$(MAKE) -C utils $@
