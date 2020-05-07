;File:	ftoa.asm
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
;	
#ifndef	FTOA_ASM 
#define	FTOA_ASM

;	INCLUDE "macro.h"
;	INCLUDE "fp.h"


;	==========================================================================
;	Fpac_fac10: Find Exponent Base 10 of Number
;	Return: FTOA_exp = Exponent Base 10 , 1.0 <= FPac < 10.0

; 
_FPac_FAC10:	; FTOA_exp = (INT8) LOG10(FPac)
	CLR8	FTOA_exp
	CLR8	ML_int
	TST8	FPac_EXP
	JMP_Z	_FPac_FAC10_X

	; Round IEEE digits (8)
	; FPac_RND

;	==========================================================================
;	Factor FPac by 10s to restrict the range to 1.0 <= FPac < 10.0
;	Notes: 
;	We do a quick test with exponents only. Keep in mind that 
;	even when Exponents are equal the mantissas can differ by upto 
;	nearly 2.0 - so take care with the tests
;
;	while(FPac > 1.0) FPac /= 10.0
_FPac_FAC10_DIV10
	; IF FPac < 1.0
	CMP8I	FPac_EXP,0x7F	; FPac <= 1.0 ?  (FPac_EXP <= 0x7F)
	JMP_LE	_FPac_FAC10_MUL10	; Yes 0x7F > FPac_EXP
	; FPac > 1.0
	INC8	FTOA_exp
	FPac_DIV10
	JMP	_FPac_FAC10_DIV10

;	while(FPac < 1.0) FPac *= 10.0
_FPac_FAC10_MUL10
	CMP8I	FPac_EXP,0x7F	; FPac >= 1.0 ? (FPac_EXP >= 0x7F)
	JMP_GE	_FPac_FAC10_X
	; FPac < 1.0
	DEC8	FTOA_exp
	FPac_MUL10
	JMP	_FPac_FAC10_MUL10

;	10.0 > FPac >= 1.0
_FPac_FAC10_X
	ENDSUB

;	==========================================================================
;
_PUT_EXP:	;	Display FTOA_exp as Floating Point exponent [+|-]NN
	; DISPLAY -/+ SIGN
	JMP_BCLR FTOA_exp,7,_PUT_EXP_P
	PUTCI	'-'
	NEG8	FTOA_exp
	JMP	_PUT_EXP_MAIN
_PUT_EXP_P
	PUTCI	'+'
;	Factor 10s
_PUT_EXP_MAIN
	CLR8	FTOA_cnt

_PUT_EXP_10S
	SUB8I	FTOA_exp,10
    JMP_B   _PUT_EXP_1S
	INC8	FTOA_cnt
	JMP	_PUT_EXP_10S
_PUT_EXP_1S
;	Display 10s
	ADD8I	FTOA_cnt,'0'
	PUTC	FTOA_cnt
;	Factor 1s
	ADD8I	FTOA_exp,'0'+10
	PUTC	FTOA_exp
	ENDSUB

;	==========================================================================
;	Convert FPac into ASCII

_FTOA:		; Convert FPac to ASCII
	FPac_FAC10
;	Display Sign
	LOAD8I	FTOA_cnt,' '
	JMP_BCLR	FPac_SIGN,7,_ftoa_main ;  < 0 ?
	LOAD8I	FTOA_cnt,'-'
_ftoa_main
	PUTC	FTOA_cnt

;	==================================================
;	Do not use BCD code for int part

	LOAD8I	FTOA_cnt,9	; digits
_ftoa_loop
	; Put a digit
	FP_SPLIT	; FPac = int, FParg = frac
	FP_U32		; AC32 = int(FPac)
	ADD8I	AC32_LSB,'0'
	PUTC	AC32_LSB
	FPac_SWAP	; FPac = frac
	FPac_MUL10	; frac *= 10
	;
	CMP8I	FTOA_cnt,9	; First Digit ?
	JMP_NZ	_ftoa_dec
	PUTCI	'.'	; Display Decimal
_ftoa_dec
	DJNZ8	FTOA_cnt,_ftoa_loop

	PUTCI	'E'
	PUT_EXP

_ftoa_x
	ENDSUB


;	============================================================
;	ENDIF FTOA_ASM
#endif
