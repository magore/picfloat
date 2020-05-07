========================================================================
Note:
    All files under directory test/psim-0.4 are covered under a separate
    copyright - Please read COPYING under test/psim-0.4 for more details
    The file tests/eparanoia.c under directory tests is covered under a 
    separate copyright - Please read the file for more details
========================================================================

; ========================================================================
; PIC Floating point library
; ========================================================================
;  Copyright (C) 1991,1997,1998,1999,2000,2001,2002,2003 Mike Gore 
;  All rights reserved.
; 
; Redistribution and use in source and binary forms, with or without 
; modification, are permitted provided that the following conditions are met:
; 
; * Redistributions of source code must retain the above copyright notice, 
;   this list of conditions and the following disclaimer.
; * Redistributions in binary form must reproduce the above copyright notice,
;   this list of conditions and the following disclaimer in the documentation
;   and/or other materials provided with the distribution.
; * Neither the name of the <ORGANIZATION> nor the names of its contributors 
;   may be used to endorse or promote products derived from this software 
;   without specific prior written permission.
; 
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
; POSSIBILITY OF SUCH DAMAGE
;
;  Contact Information
;  Mike Gore
;  Infowrite consulting
;  405 Midwood Cres.
;  Waterloo Ont
;  N2L 5N4
;  Phone:       519-884-4943 home
;  Fax:         519-885-0548 fax
;  Email:       magore@sympatico.ca - or - magore@uwaterloo.ca
; ========================================================================
; ========================================================================
; ***** Please Use TABS = 4 when viewing this file ****
; ========================================================================

The basic math functions have been tested against PARANOIA but this
does not guarantee there are not any bugs - please feel free to report and
or fix any bugs you may find.

Runing tests:
	PARANOIA tests:	make run_eparanoi
			- Paranoia test suite

	Trig tests:		make run_fptest
			- SIN,COS,TAN tests (tests a large number of internal functiosn)

	Native test:	make run_psim
			- Runs the built in tests of picfp.asm

	Integer/Fractional split test:	make run_fpint
			- FP_SPLIT tests

    To run the builtin tests go to the tests directory and run make run_psim
    Type the command "g" at the command prompt to view the output. This
    output is 100% the result of code inside the psim.hex program

    To run tests that call math functions inside psim.hex but compare
    results on using the hos machine run the command make run_eparanoi 
    (PARANOIA test suite) - or make run_fptest to test trig functions.
	
fptest.c		- test trig functions using pic math library and PSIM 
				simulator
fppow.c			- some tests of LOG,POW,EXP using pic math library and PSIM
				simulator
fpint.c 		- some tests of floating/integer conversions using pic math 
				library and PSIM simulator
GETSYMS			- Extract entry point addesses from listing file (hack)
picfp.c			- Host Interface functions for pic math library. Interfaces
				PSIM simulator calls to user programs
picfp.h			- #defines created by GETSYMS that define entry point
				addresses


./tests/psim-0.4 - Modified PSIM simulator 
				 see: http://sourceforge.net/projects/psim
./tests/psim-0.4/psim.patch - my patch file with diffs from original 
				 PSIM simulator

./tests/paranoia - Modified version of paranoia test suite
				see: http://www.moshier.net

./baseline		- Outputs from fptest and eparanoi
