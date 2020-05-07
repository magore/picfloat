;File:	fpfn.h
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
#ifndef	FPFN_H
#define	FPFN_H

;	INCLUDE "macro.h"


	cblock
	FParg1:6, FParg2:6, FPquad, FPflag, FPflag2, FP_cnt
	endc

POLY_FParg1	MACRO
	CALL	_POLY_FParg1
	ENDM

FP_SQRT	MACRO
	CALL	_FP_SQRT
	ENDM

FP_QUAD	MACRO
	CALL	_FP_QUAD
	ENDM

FP_TAN	MACRO
	CALL	_FP_TAN
	ENDM

FP_ATAN	MACRO
	CALL	_FP_ATAN
	ENDM

FP_SIN	MACRO
	CALL	_FP_SIN
	ENDM

FP_ASIN	MACRO
	CALL	_FP_ASIN
	ENDM

FP_ACOS	MACRO
	CALL	_FP_ACOS
	ENDM

FP_COS	MACRO
	CALL	_FP_COS
	ENDM

FP_LOG	MACRO
	CALL	_FP_LOG
	ENDM

FP_EXP	MACRO
	CALL	_FP_EXP
	ENDM
;	=========================================================================
M_RAD_DEG   ; 1.7453292520e-02 (temp real)
    retlw   0x13 ; (0) LSB
    retlw   0x35 ; (1)
    retlw   0xfa ; (2)
    retlw   0x8e ; (3) MSB
    retlw   0x79 ; (4) EXP
    retlw   0x00 ; (5) SIGN


M_PI_2	; 1.5707963268e+00 (temp real) residual: -6.5874417032e-11:
	retlw	0xa2 ; (0) LSB
	retlw	0xda ; (1) 
	retlw	0x0f ; (2) 
	retlw	0xc9 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_2_PI	; 6.3661977237e-01 (temp real) residual: -7.3601125195e-11:
	retlw	0x6e ; (0) LSB
	retlw	0x83 ; (1) 
	retlw	0xf9 ; (2) 
	retlw	0xa2 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_4_PI	; 1.2732395447e+00 (temp real) residual: -1.0720224708e-10:
	retlw	0x6e ; (0) LSB
	retlw	0x83 ; (1) 
	retlw	0xf9 ; (2) 
	retlw	0xa2 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LN2	; 6.9314718056e-01 (temp real) residual: 4.1954439922e-11:
	retlw	0xf8 ; (0) LSB
	retlw	0x17 ; (1) 
	retlw	0x72 ; (2) 
	retlw	0xb1 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_SQRT2	; 1.4142135624e+00 (temp real) residual: -1.5752288363e-11:
	retlw	0x34 ; (0) LSB
	retlw	0xf3 ; (1) 
	retlw	0x04 ; (2) 
	retlw	0xb5 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_HALF	; 5.0000000000e-01 (temp real) residual: 0.0000000000e+00:
	retlw	0x00 ; (0) LSB
	retlw	0x00 ; (1) 
	retlw	0x00 ; (2) 
	retlw	0x80 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

;	=========================================================================
;	ENDIF FPFN_H
#endif
