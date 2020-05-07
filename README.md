# picfloat
  * PICFLOAT is a open source floating point library for midrange PIC processors from Microchip
This is a clone of my SourceForge project txt with some minor recent updates
   
___


## Credits
    All files under directory test/psim-0.4 are covered under a separate
    copyright - Please read COPYING under test/psim-0.4 for more details
    The file tests/eparanoia.c under directory tests is covered under a 
    separate copyright - Please read the file for more details


### License: BSD
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

## PIC Floating point library - PIC Assembler code
### Background
I started working on this project back in 1990. I started with a macro library to make using the PIC easier to use. I felt that having a generic base of multi-byte functions would allow making math libraries mush easier to do. I had the floating point core written in 1991. The project was revisited over the years to include most of the common C floating point math functions. I had planned to have the library as part of an interpreter with the higher level functions and data kept in NVRAM. This has much to do with why there is <b>no banking code in this project</b> (It would be easy to have this code and a small interpreter fit inside of 2K code if many of the functions were moved to NVRAM)


### Overview
  * Many of the functions names are chosen from their C counter parts. All 
floating point calculations are done to 40bits (actually 8bit exponent, 
1bit sign and 32bit mantissa).  The user can load and store either these 
40bit numbers or use IEEE32 bit format to save space. Although targeted 
for the PIC very few processor assumptions have been made. Most of the 
machine dependent assumptions are kept to MACRO.H and the remaining W, 
FSR/IND and table operation can be very simply modified for most other 
processors 

___

### Testing
  * The basic math functions have been tested against PARANOIA. PARANOIA used calls to the PIC simulator to test the PIC math functions. This allowed the use of a complext testing environment written in C to verify the results against the simulated PIC.
  * I have run this code against the PARANOIA test suite by Steve Moshier using a modified version of the PIC simulator PSIM by Eric Smith See support links

### Format: IEEE 32bit single and 40bit format 
  * 8bit signed exponent,32bit mantissa, 1bit sign 
  * Calculations are done in 40bit format including trig,log, etc

### Main features
  * small size - all of it - and testing code fits inside 2K code!
  * heavy use of macros (over 300) makes code readable and portable
  * only assume one 8bit register called W (easy to change) - and limited use of an  index register
  * testing is done with paranoia math test suite

### Target PIC Processors: 14bit processors (initial port)

### Support Utilities
   * Includes programs to generate tables or constants from floating point numbers that can be included in assembler code

___

### Details
#### Macros
  * Define all core multibyte functions (1 to N byte size!) to make building function easy
  * Arithmetic, Logical, Shifts, Compares, Load (Direct and Constant), Store, Bit Operations, Looping, Increment, Decrement, Jumps
  * Macros are also used to invoke all function calls (for readability and mainly to allow modification of calling methods on various PICs)
#### 32bit math
  * 32bit integer functions based on macro headers including Multiply, Divide, Conversions to/from 8, 16, 24 and 32bit signed/unsigned values
I/O: 32bit BCD: convert up to 32bit numbers to BCD and output results in ASCII, ATOI convert ASCII to 32bit
#### Floating point core
  * Conversions (IEEE to/from 40bit temporary real) Load/Store ( to/from IEEE or 40bit temporary real)  Arithmetic, Increment, Decrement,
  * Conversions (to/from 8, 16, 24, 32bit integers signed/unsigned)
#### Floating point functions
  * Poly, Sqrt, Sin, Cos, Tan, Asin, Acos, Atan, Log, Exp, Pow, Test (Zero/Nonzero), Split(Real,Integer), Floor, Ceil, LDEXP, FREXP, FMOD, INVERT(1/X)
#### Floating Point I/O
  * FTOA - Binary to ASCII floating and  ATOF ASCII to floating conversions supporting scientific notation
#### Table functions
  * 32bit math, 40 and 32bit Floating point load
#### Memory functions
  * memory copy
#### PIC banking code
  * <b>there is no bank select code</b> all code and data fit within a single bank

#### AND MUCH MORE!


### Wish List
  * I hope others can work on and expand on this project. Some ideas for areas improvement could be Some method to compile in only functions that are used

  * perhaps a preprocessor stage.  Use of index register for argument passing and computations
  * Add a stack
  * Better error reporting
  * Consider alternate range reduction methods for TRIG functions
  * Add statistical testing for host/simulator testing of TRIG,LOG,EXP functions
  * We compute the mantissa to 32bits so add double support to host testing functions - the float limit hides much of the real accuracy of the functions 

### Comments
  * I believe that prior to this project that PIC assembler support for floating point functions, and good macro support in general, has been seriously lacking.
  * Keep in mind the limited numeric resolution resulting from the number formats themselves when doing computations! Take a small number, say delta, and add another number that is larger in magnitude by 2**(bits of precision) - the delta contribution is lost.. A good example of this problem occurs when passing two angles near 90 degrees that differ by a small ammont - if you are depending on measuring small changes in these angles your results will  have large unexpected errors - so take time to understand this limit and rework your computations as required. No amount of design given to a mathematical functions can compensate from being badly used. (But please feel free to fix any bugs you may find in my code 8-)

___

### Misc
  * If anyone is interested in tools to make interpreters I would suggest using the COCOR compiler construction toolset by Frankie Arzu
  * comes with easy to use examples


### Support Links

  * PARANOIA - Steve Moshiers home page
  * PSIM  Source Forge
  * GPUTILS - Source Forge
  * http://www.moshier.net
  * http://sourceforge.net/projects/psim
  * http://gputils.sourceforge.net/

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
