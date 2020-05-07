;File:	math_tab.h
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
#ifndef	MATH_TAB_H
#define	MATH_TAB_H

table0_start

M_RAD_DEG	; 1.7453292520e-02 (temp real)
	retlw	0x13 ; (0) LSB
	retlw	0x35 ; (1)
	retlw	0xfa ; (2)
	retlw	0x8e ; (3) MSB
	retlw	0x79 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_DEG_RAD	; 5.7295779513e+01 (temp real)
	retlw	0xd3 ; (0) LSB
	retlw	0xe0 ; (1)
	retlw	0x2e ; (2)
	retlw	0xe5 ; (3) MSB
	retlw	0x84 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_PI	; 3.1415926536e+00 (temp real)
	retlw	0xa2 ; (0) LSB
	retlw	0xda ; (1)
	retlw	0x0f ; (2)
	retlw	0xc9 ; (3) MSB
	retlw	0x80 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_PI_4	; 7.8539816340e-01 (temp real)
	retlw	0xa2 ; (0) LSB
	retlw	0xda ; (1)
	retlw	0x0f ; (2)
	retlw	0xc9 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_1_PI	; 3.1830988618e-01 (temp real)
	retlw	0x6e ; (0) LSB
	retlw	0x83 ; (1)
	retlw	0xf9 ; (2)
	retlw	0xa2 ; (3) MSB
	retlw	0x7d ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_2_SQRTPI	; 1.1283791671e+00 (temp real)
	retlw	0x82 ; (0) LSB
	retlw	0xba ; (1)
	retlw	0x6e ; (2)
	retlw	0x90 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN


M_E	; 2.7182818285e+00 (temp real)
	retlw	0x59 ; (0) LSB
	retlw	0x54 ; (1)
	retlw	0xf8 ; (2)
	retlw	0xad ; (3) MSB
	retlw	0x80 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LOG2E	; 1.4426950409e+00 (temp real)
	retlw	0x29 ; (0) LSB
	retlw	0x3b ; (1)
	retlw	0xaa ; (2)
	retlw	0xb8 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LOG10E	; 4.3429448190e-01 (temp real)
	retlw	0xa9 ; (0) LSB
	retlw	0xd8 ; (1)
	retlw	0x5b ; (2)
	retlw	0xde ; (3) MSB
	retlw	0x7d ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_1_LN2	; 1.4426950409e+00 (temp real)
	retlw	0x29 ; (0) LSB
	retlw	0x3b ; (1)
	retlw	0xaa ; (2)
	retlw	0xb8 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LN10	; 2.3025850930e+00 (temp real)
	retlw	0xde ; (0) LSB
	retlw	0x8d ; (1)
	retlw	0x5d ; (2)
	retlw	0x93 ; (3) MSB
	retlw	0x80 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_SQRT1_2	; 7.0710678119e-01 (temp real)
	retlw	0x34 ; (0) LSB
	retlw	0xf3 ; (1)
	retlw	0x04 ; (2)
	retlw	0xb5 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ZERO	; 0.0000000000e+00 (temp real)
	retlw	0x00 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x00 ; (2)
	retlw	0x00 ; (3) MSB
	retlw	0x00 ; (4) EXP
	retlw	0x00 ; (5) SIGN


M_ONE	; 1.0000000000e+00 (temp real)
	retlw	0x00 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x00 ; (2)
	retlw	0x80 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_TWO	; 2.0000000000e+00 (temp real)
	retlw	0x00 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x00 ; (2)
	retlw	0x80 ; (3) MSB
	retlw	0x80 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_TEN	; 1.0000000000e+01 (temp real)
	retlw	0x00 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x00 ; (2)
	retlw	0xa0 ; (3) MSB
	retlw	0x82 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_TENTH	; 1.0000000000e-01 (temp real)
	retlw	0xcd ; (0) LSB
	retlw	0xcc ; (1)
	retlw	0xcc ; (2)
	retlw	0xcc ; (3) MSB
	retlw	0x7b ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_TEN5	; 1.0000000000e+06 (temp real)
	retlw 0x00 ; (0) LSB
	retlw 0x00 ; (1)
	retlw 0x24 ; (2)
	retlw 0xf4 ; (3) MSB
	retlw 0x92 ; (4) EXP
	retlw 0x00 ; (5) SIGN

M_TEN7	; 1.0000000000e+08 (temp real)
	retlw 0x00 ; (0) LSB
	retlw 0x20 ; (1)
	retlw 0xbc ; (2)
	retlw 0xbe ; (3) MSB
	retlw 0x99 ; (4) EXP
	retlw 0x00 ; (5) SIGN

table0_end


;	=====================================================================
;	ENDIF MATH_TAB_H
#endif
