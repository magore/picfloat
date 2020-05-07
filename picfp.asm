;File:	picfp.asm
;	========================================================================
;	PIC Floating point library
;	========================================================================
;	Copyright (C) 1991,1997,1998,1999,2000,2001,2002,2003 Mike Gore 
;	All rights reserved.
;	
;	Redistribution and use in source and binary forms, with or without 
;	modification, are permitted provided that the following conditions are met: 
;	
;	* Redistributions of source code must retain the above copyright notice, 
;	this list of conditions and the following disclaimer.
;	* Redistributions in binary form must reproduce the above copyright notice,
;	this list of conditions and the following disclaimer in the documentation
;	and/or other materials provided with the distribution.
;	* Neither the name of the <ORGANIZATION> nor the names of its contributors 
;	may be used to endorse or promote products derived from this software 
;	without specific prior written permission.
;	
;	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
;	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
;	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
;	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
;	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
;	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
;	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
;	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
;	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
;	POSSIBILITY OF SUCH DAMAGE
;
;	Contact Information
;	Mike Gore
;	Infowrite Consulting
;	405 Midwood Cres.
;	Waterloo Ont
;	N2L 5N4
;	Phone: 519-884-4943 home
;	Fax: 519-885-0548 fax
;	Email: magore@sympatico.ca - or - magore@uwaterloo.ca
;	========================================================================
;	========================================================================
;	***** Please Use TABS = 4 when viewing this file ****
;	========================================================================
;	=========================================================================
;	Floating point code tester
;	=========================================================================
	RADIX DEC
base	= 0x0A

	LIST	p=16F628, X=ON
;	LIST	p=P18F452, X=ON
  __config _WDT_OFF

	INCLUDE	"p16f628.inc"

;	INCLUDE "P18F452.inc"



	NOEXPAND

	cblock 0x20
	endc

;	======================================================================
;	Main Macros must be included first!

	NOLIST
	INCLUDE "macro.h"
	LIST

;	======================================================================
;	MAIN INIT CODE
;	======================================================================
	ORG 0
	JMP	init

;	Memory functions	- mem copy
	INCLUDE	"mem.h"

;	Table load functions - loading from ROM
	INCLUDE	"table.h"

;	AC,ARG,FPac,FParg table load functions - loading from ROM
	INCLUDE	"tablesup.h"

;	Debugg I/O targeted for PSIM simulator
	INCLUDE "io.h"

;	======================================================================
;	(Floating point and 32bit math core variables)
;	32 Bit core math
	INCLUDE	"ac32.h"
;	Floating point core
	INCLUDE "fp.h"
;	Floating point extras - not used
;	INCLUDE "fpsup.h"
;	Trig Functions
	INCLUDE "fpfn.h"


;	Floating to ascii conversions
	INCLUDE "ftoa.h"

;	Debugg I/O targeted for PSIM simulator
	INCLUDE "io.asm"
;	======================================================================
;	32 bit core
	INCLUDE	"ac32.asm"
;	Floating point core
	INCLUDE "fp.asm"
;	Floating point extras - not used
;	INCLUDE "fpsup.asm"
;	Trig Functions
	INCLUDE "fpfn.asm"
;	Floating to ascii conversions
	INCLUDE "ftoa.asm"

;	Temp floating variables for this test file
	cblock
	FParg3:6, FParg4:6, FParg5:6, FP_loop
	endc

; ======================================================================
;  Setup PIC registers and I/O functions
_PIC_INIT:
;	Set option to NO weak B-pull-ups and RTCC with prescale of 64
	movlw	0xC1		; Week B Pull Up Off, Prescale == 0x64, + INT
;	option
	banksel	OPTION_REG
	movwf	OPTION_REG

; bank0
	bcf	STATUS,RP0
	bcf	STATUS,RP1

; *** These I/O Statements are needed by the debugger ***
; Set I/O to default states prior to user setup
	INIT_IO	
; Set Output to PSIM output trap
	SET_PUTC	_DEBUG_PUTC
; Set Input to Table Load Function
	SET_GETC	_TABLE_LDW
	ENDSUB


; ======================================================================
#ifdef	BASIC_TEST
M_PI    ; 3.1415926536e+00 (temp real)
    retlw   0xa2 ; (0) LSB
    retlw   0xda ; (1)
    retlw   0x0f ; (2)
    retlw   0xc9 ; (3) MSB
    retlw   0x80 ; (4) EXP
    retlw   0x00 ; (5) SIGN

M_TENTH ; 1.0000000000e-01 (temp real)
    retlw   0xcd ; (0) LSB
    retlw   0xcc ; (1)
    retlw   0xcc ; (2)
    retlw   0xcc ; (3) MSB
    retlw   0x7b ; (4) EXP
    retlw   0x00 ; (5) SIGN
#endif

#ifdef ATOF_TEST
;	======================================================================
;	ASCII to integer conversions
	INCLUDE "atoi.h"
	INCLUDE "atoi.asm"
;	ASCII to floating conversions
	INCLUDE "atof.h"
	INCLUDE "atof.asm"

; Test Full range
test0:   

; Test whitespae 
	dt  " -1.23456789e+12",0

; Test radix point issues
	dt  "+0.12e+12",0

	dt  "-0.12e-12",0


; Test limits
	dt  "3.4e38",0
; Test overflow
	dt  "1e39",0

	dt  "1.2e-38",0
; Test underflow
	dt  "1e-38",0

; Test +/-zero
	dt  "-0e-0",0
	dt  "0e-40",0
	dt  "+0"

; Done
	dt	26,0
		
; Should NOT get here 8-)
die:	
	JMP die

#endif

;================================================================
; Main Line
init:
	CALL	_PIC_INIT

	FP_CLR_ERROR

#ifdef ATOF_TEST
	TABLE_SET_SOURCE test0
	
PRINT_NUMS
	PUTS	"Input: "

	MOV16	_PUTS_PTR,table_ptr
	CALL	_PUTS
	PUT_NL

	ATOF

	PUTS	"Result:"

	FTOA

	PUT_NL

; Read past the EOS
	GETC
	JMP_Z	PRINT_NUMS	; Skip EOS
PRINT_NUM

#endif

;================================================================
; BASIC FLOATING POINT TESTS
#ifdef BASIC_TEST

;	Simple Test of FTOA and TABLE LOAD
;	PI
	PUTS	"PI: "

	TRac_TABLE_LD	M_PI

	FTOA
	PUT_NL

;	Simple Test of FP_I32 and ITOF32
;	Should print 3
	PUTS	"PI: (INT)"
	TRac_TABLE_LD	M_PI
	FP_I32
	ITOF32
	FTOA
	PUT_NL

	PUT_NL
;	================================================================
;	Test FTOA, SQRT, LOG and EXP over the range 0.0 to 2.0
;	================================================================
	FPac_CLR
	TRac_STORE	FParg3	; 0.0 Index

	TRac_TABLE_LD	 M_TENTH
	TRac_STORE	FParg4	; 0.1 Incrment
	LOAD8I		FP_loop,101	; Loop Count

ftoa_loop
;	Print Index
	TRac_LOAD	FParg3	; Index
	FTOA

	PUTS	", Sqrt:"

;	Print SQRT(Index)
	TRac_LOAD	FParg3	; Index
	FP_SQRT
	FTOA


	PUTS	", Log:"
;	Print LOG(Index)
	TRac_LOAD	FParg3	; Index
	FP_LOG
	FTOA

	PUTS	", Exp:"
;	Print EXP(Index)
	TRac_LOAD	FParg3	; Index
	FP_EXP
	FTOA
	PUT_NL

;	Advance Index
	TRac_LOAD	FParg3	; Index
	TRarg_LOAD	FParg4	; Increment
	FP_ADD
	TRac_STORE	FParg3	; Index += Increment

; Any Errors ?
	FP_ERROR
	FP_CLR_ERROR

;	Loop
	DJNZ8	FP_loop,ftoa_loop


#endif

#ifdef TRIG_TEST

;	================================================================
;	Trig Tests in 1 degree incremnets from 0.0 to 90.0 degrees
;
;	Print SIN() and Print ASIN() of this result, Newline
;
;	Print COS() and Print ACOS() of this result, Newline
;
;	Print TAN() and Print ATAN() of this result, Newline
;	================================================================

	FPac_CLR
	TRac_STORE	FParg3	; Save
	; Loop counter
	LOAD8I	FP_loop,91	;
sin_loop

	; Angle *= (RAD/DEG)
	TRac_TABLE_LD	M_RAD_DEG
	TRarg_LOAD	FParg3
	FP_MUL
	TRac_STORE	FParg4	; Save Angle

;	Print Angle
	PUTS	"RADS:\t\t\t  "

	FTOA
	PUT_NL

;	SIN
	PUTS	"SIN:"
	TRac_LOAD	FParg4	; Angle
	FP_SIN
	TRac_STORE	FParg5	; Save Result
	FTOA
	PUTS	", ASIN:"

;	ASIN RESULT
	TRac_LOAD	FParg5	; Restore Result
	FP_ASIN
	FTOA
	PUT_NL

; Any Errors ?
	FP_ERROR
	FP_CLR_ERROR


;	COS
	PUTS	"COS:"
	TRac_LOAD	FParg4	; Angle
	FP_COS
	TRac_STORE	FParg5	; Save Result
	FTOA
	PUTS	", ACOS:"
;	ACOS RESULT
	TRac_LOAD	FParg5	; Restore Result
	FP_ACOS
	FTOA
	PUT_NL

; Any Errors ?
	FP_ERROR
	FP_CLR_ERROR

;	TAN
	PUTS	"TAN:"
	TRac_LOAD	FParg4	; Angle
	FP_TAN
	TRac_STORE	FParg5	; Save Result
	FTOA
	PUTS	", ATAN:"
;	ATAN RESULT
	TRac_LOAD	FParg5	; Restore Result
	FP_ATAN
	FTOA
	PUT_NL

; Any Errors ?
	FP_ERROR
	FP_CLR_ERROR

	PUT_NL

	; FParg3 += 1.0
	TRac_LOAD	FParg3
	FP_INC_AC
	TRac_STORE	FParg3

	DJNZ8	FP_loop,sin_loop
#endif

;
picfp_exit
	JMP	picfp_exit

	END
