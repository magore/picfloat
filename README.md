# picfloat
  * PICFLOAT is a open source floating point library for midrange PIC processors from Microchip
This is a clone of my SourceForge.txt with some recent updates
   
___


## Credits
    All files under directory test/psim-0.4 are covered under a separate
    copyright - Please read COPYING under test/psim-0.4 for more details
    The file tests/eparanoia.c under directory tests is covered under a 
    separate copyright - Please read the file for more details


### Copyright
 * Copyright (C) 1991,1997,1998,1999,2000,2001,2002,2003 Mike Gore All rights reserved.
 
 * Redistribution and use in source and binary forms, with or without 
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the <ORGANIZATION> nor the names of its contributors 
   may be used to endorse or promote products derived from this software 
   without specific prior written permission.
 
___


### Disclaimer
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 POSSIBILITY OF SUCH DAMAGE

___


### Contact 
  Mike Gore
  Infowrite consulting
  405 Midwood Cres.
  Waterloo Ont
  N2L 5N4
  Email:       magore@sympatico.ca - or - magore@uwaterloo.ca

## PIC Floating point library
  * This is a floating point library that I have been working a bit at
a time over the last 12 years. My original goal was to make the
library as small as possible and to form the core of an interpreter.
As a result <b>there is no bank select code</b> all code and data fit within a single bank

  * Many of the functions names are chosen from their C counter parts. All 
floating point calculations are done to 40bits (actually 8bit exponent, 
1bit sign and 32bit mantissa).  The user can load and store either these 
40bit numbers or use IEEE32 bit format to save space. Although targeted 
for the PIC very few processor assumptions have been made. Most of the 
machine dependent assumptions are kept to MACRO.H and the remaining W, 
FSR/IND and table operation can be very simply modified for most other 
processors 

### Testing
  * The basic math functions have been tested against PARANOIA. PARANOIA used calls to the PIC simulator to test the PIC math functions. This allowed the use of a complext testing environment written in C to verify the results against the simulated PIC.
  * This does not guarantee there are not any bugs - please feel free to report and or fix any bugs you may find.

___

## Files

  * ac32.asm ac32.h     - 32bit accumulator
  * atof.asm atof.h     - atof - ascii to float conversion 
  * atoi.asm atoi.h     - atoi - ascii to integer conversion 
  * bcd32.asm bcd32.h   - 32bit binary to decimal conversions
  *fp.asm fp.h          - Floating point core
  *fpfn.asm fpfn.h      - Trig,Log,Exp functions
  *fpsup.asm fpsup.h    - Floating point extras
  *ftoa.asm ftoa.h      - FTOA floating to ascii
  *io.asm io.h          - Debug output for simulator PSIM
  *macro.h              - PIC Macros
  *math.h               - EQU defines of floating point consants
  *math_tab.h           - Floating point constants for math functions
  *mem.h                - Memory move code
  *mkdocs               - Extract documentation for source files (hack)
  *picfp.asm            - Floating point test MAIN
  *sourceforge.txt      - Discription of initial sourceforge project picfloat
  *stack.h              - work in progress stack functions
  *table0.h             - Table load code for math_tab.h
  *table.h              - General table load code
  *tablesup.h           - Table load macros for math functions and support

## Directories:
  * ./utils             - Utilities to make floating point constants

  * ./tests             - Test suite programs

  * ./tests/psim-0.4    - Modified PSIM simulator
                    see: http://sourceforge.net/projects/psim

  * ./tests/psim-0.4/psim.patch 
                        - my patch file with diffs from original PSIM simulator

  * ./tests/paranoia    - Modified version of PARANOIA test suite
    * see: http://www.moshier.net

  * ./tests/baseline    - Outputs from tests fptest,fpint,fppow and eparanoi

___
