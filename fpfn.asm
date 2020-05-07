;File:	fpfn.asm
;===============================================================
;	PIC Floating point library
;===============================================================
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
;===============================================================
;===============================================================
;	***** Please Use TABS = 4 when viewing this file ****
;===============================================================
#ifndef	FPFN_ASM
#define	FPFN_ASM
;Notes: 
;	Chebyshev coefficents obtained from Computer Approximations, Hart, et al
;	Table values were chosen for 8 to 9 digit results
;
;   Functions are computed with 32bit mantissa and 8bit signed exponent for
;	greater internal precision (called temporary real - TR),
;	results can then be stored in this formate - or in IEEE 32bit floating 
;	point number (8bit signed exponent, sign bit and 23 bit mantissa)
;	
; =================================================================
; We depend on math_tab.h
; =================================================================

; =================================================================
;_POLY_FParg1
;	Call TABLE_SET_BASE first!
;
;	P += *table_ptr
;	P *= FParg1
;
;	FParg = *table_ptr	; Constant
;	FPac += FParg		; P += Constant
;	FPac *= FParg1		; P *= FParg1
_POLY_FParg1:	; P += *table_ptr, P *= FParg1
	TABLE_LD		; FParg = Constant
	FP_ADD			; P += Constant
	TRarg_LOAD	FParg1		; P *= FParg1
	FP_MUL
	ENDSUB
; =================================================================


; =================================================================

_FP_SQRT:	; FPac = SQRT(FPac)
	JMP_BSET	FPac_SIGN,7,_fp_sqrtx		; FPac = 0.0
	TST8	FPac_EXP
	JMP_NZ	_fp_sqrt1
_fp_sqrtx
	JMP	_FPac_ZERO

_fp_sqrt1
	FP_FREXP		;	x = fp_frexp(x, &EXP);
	movwf	FPflag		; EXP
	JMP_BCLR	FPflag,0,_fp_sqrt2
	INC8	FPac_EXP	; x *= 2.0
_fp_sqrt2
	TRac_STORE	FParg	; FParg = FPac u = x
	TRac_STORE	FParg1	; FParg1 = x

	LOAD8I	FP_cnt,5
_fp_sqrt_loop
	FPac_SWAP		; FParg = u
	TRac_LOAD	FParg1	; x
;	u += x / u;		; U(i+1) = (U(i) + X / U(i)) / 2.0;
;	u /= 2.0;
	; FPac = x, FParg = u
	FP_DIV			; (Fpac = x) / (FParg = u)
	FP_ADD			; Fpac = x / u + u
	FPac_DEC_EXP		; u /= 2
	DJNZ8	FP_cnt,_fp_sqrt_loop

	ASH8_LSB	FPflag	; signed shift EXP >>= 1
	movfw	FPflag
	FP_LDEXP		; u = fp_ldexp
	ENDSUB
;===============================================================

;===============================================================
_FP_QUAD:	; Find 45 degree section of a circle, Assuming 45deg = 1.0
	MOV8	FPflag,FPac_SIGN
	CLR8	FPac_SIGN
	FP_SPLIT		; FPac = quad, FParg = fraction(xp)
	FP_U32			; AC32 = int(quad)

	MOV8	FPquad,AC32_LSB
	AND8I	FPquad,0x07	; quad &= 0x07
	TRarg_STORE	FPac	; Fparg = FPac = fracton(xp)
_FP_quadX
	ENDSUB

M_SIN_SCALE	equ	M_2_PI	; 6.3661977237e-01 (temp real)

SIN_P06	; 5.4589878320e-08 (temp real) residual: 6.0723196419e-18:
	retlw	0xbb ; (0) LSB
	retlw	0x34 ; (1) 
	retlw	0x76 ; (2) 
	retlw	0xea ; (3) MSB
	retlw	0x66 ; (4) EXP
	retlw	0x00 ; (5) SIGN

SIN_P05	; -3.5955981533e-06 (temp real) residual: -1.3857289610e-16:
	retlw	0xea ; (0) LSB
	retlw	0xe7 ; (1) 
	retlw	0x4b ; (2) 
	retlw	0xf1 ; (3) MSB
	retlw	0x6c ; (4) EXP
	retlw	0x80 ; (5) SIGN

SIN_P04	; 1.6043892991e-04 (temp real) residual: 1.4566771183e-14:
	retlw	0x50 ; (0) LSB
	retlw	0x7f ; (1) 
	retlw	0x3b ; (2) 
	retlw	0xa8 ; (3) MSB
	retlw	0x72 ; (4) EXP
	retlw	0x00 ; (5) SIGN

SIN_P03	; -4.6817533261e-03 (temp real) residual: 3.8736548691e-13:
	retlw	0xb6 ; (0) LSB
	retlw	0x64 ; (1) 
	retlw	0x69 ; (2) 
	retlw	0x99 ; (3) MSB
	retlw	0x77 ; (4) EXP
	retlw	0x80 ; (5) SIGN

SIN_P02	; 7.9692626107e-02 (temp real) residual: 3.0452446120e-12:
	retlw	0x37 ; (0) LSB
	retlw	0xe3 ; (1) 
	retlw	0x35 ; (2) 
	retlw	0xa3 ; (3) MSB
	retlw	0x7b ; (4) EXP
	retlw	0x00 ; (5) SIGN

SIN_P01	; -6.4596409750e-01 (temp real) residual: 3.5542790933e-11:
	retlw	0x31 ; (0) LSB
	retlw	0xe7 ; (1) 
	retlw	0x5d ; (2) 
	retlw	0xa5 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x80 ; (5) SIGN

SIN_P00	; 1.5707963268e+00 (temp real) residual: -6.5874417032e-11:
	retlw	0xa2 ; (0) LSB
	retlw	0xda ; (1) 
	retlw	0x0f ; (2) 
	retlw	0xc9 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN


;===============================================================
; COS(X) = SIN(X+PI/2)
;===============================================================

_FP_COS:	; FPac = COS(FPac)
; The scale factor reduces PI/2 to 1.0
 	TRarg_TABLE_LD	M_SIN_SCALE 	; TRarg = SIN_SCALE
	FP_MUL							; xp *= SIN_SCALE
	FP_QUAD
	
	JMP_BCLR	FPflag,7,_FP_cos_0 	; if(FPflag & 0x80) FPquad-- else FPquad++
; -ve
	DEC8		FPquad
	JMP			_FP_SIN_COS
; +ve
_FP_cos_0
	INC8		FPquad
	JMP			_FP_SIN_COS

;===============================================================
;	SIN(X) = z*P(z**2), z = x * SIN_SCALE
;	3343 pg117,pg204
;===============================================================
; Notes
; The SIN expansion is prescaled so that PI/2 = 1
; The integer component therefore represents the circle "quad"
; The faction is is to computer the angle in this quad
;===============================================================

_FP_SIN:	; FPac = SIN(FPc)
; The scale factor reduces PI/2 to 1.0
 	TRarg_TABLE_LD	M_SIN_SCALE 	; TRarg = SIN_SCALE
	FP_MUL							; xp *= SIN_SCALE
	FP_QUAD
; xp has the fractional part of angle

; SIN and COS common code
_FP_SIN_COS
	JMP_BCLR	FPquad,0,_FP_SIN_COS0 	; if(FPquad & 1) xp = 1 - xp
; QUAD == 1 or QUAD == 3
; xp = 1 - xp
; Note: There is distructive cancelation when X is near 1 (1 = PI/2 scaled),
;		However; we note that ideally SIN(xp=(2-sigma)) ~= sigma for very small 
;		angles. But now say that sigma is the truncation error due to the
;		limited resolution of the FP number system in use - (about 1 part 
;		in 2**-32) then it is clear that the truncation error impact is 
;		very small - in fact very near sigma itself - not bad.

	XOR8I		FPac_SIGN,0x80
	FParg_ONE
	FP_ADD
_FP_SIN_COS0
	JMP_BCLR	FPquad,1,_FP_SIN_COS1 	; if(FPquad & 2) FPflag ^= 0x80
	XOR8I		FPflag,0x80
_FP_SIN_COS1
	TRac_STORE	FParg1		; FParg1 = xp
	TABLE_SET_TRarg			; TRarg = Default Table load
	TABLE_SET_SOURCE	SIN_P06
	FPac_CLR				; p = 0
	LOAD8I	FP_cnt,6		; 6 terms
_FP_SIN_COS_LOOP
	POLY_FParg1				; p += Constant, p *= xp
	FP_MUL					; p *= xp
	DJNZ8	FP_cnt,_FP_SIN_COS_LOOP
	POLY_FParg1				; p += Constant, p *= xp
	CLR8	FPac_SIGN		; xp = fp_fabs(p);
	
; if(FPflag & 0x80) xp = -xp
	XOR8	FPac_SIGN,FPflag
	ENDSUB


; =================================================================
;	TAN(x*PI/4) = x * P(x*x)/Q(x*x)
;	
;	Formula 4242, pg216
;	TAN 
;	0 90   90+       180- 180   270-  270      360-
;	0,1,1,INF | -INF,-1,-1,-0 | 0,1,1,INF | INF,-1,-1,-0
;	
;	We can compute TAN 0 to 45 degrees directly 
;	
;	IDENTITIES
;	TAN(X) = TAN(X MOD 180)
;	Y = X MOD 45
;	QUAD 0 if(X >= 0 && < 45)
;	TAN	= TAN(Y);
;	QUAD 1 if(X >= 45 && < 90)
;	TAN	= 1/TAN(45-Y);
;	QUAD 2 if(X >= 90 && < 135)
;	TAN	= -1/TAN(Y);
;	QUAD 3 if(X >= 135 && < 180)
;	TAN	= -TAN(45-Y));
;	
;	NOTE SAME TAN_SCALE BASIS IS USED IN COMPUTATION, KINDA NICE!
;	
;===============================================================
M_TAN_SCALE	equ	M_4_PI	; 1.2732395447e+00 (temp real)
;
M_TANP1	; -1.2528888728e+01 (temp real)
	retlw	0x07 ; (0) LSB
	retlw	0x54 ; (1)
	retlw	0x76 ; (2)
	retlw	0xc8 ; (3) MSB
	retlw	0x82 ; (4) EXP
	retlw	0x80 ; (5) SIGN

M_TANP2	; 2.1184936966e+02 (temp real)
	retlw	0x4a ; (0) LSB
	retlw	0x70 ; (1)
	retlw	0xd9 ; (2)
	retlw	0xd3 ; (3) MSB
	retlw	0x86 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_TANQ1	; 1.0000000000e+00 (temp real)
	retlw	0x00 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x00 ; (2)
	retlw	0x80 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_TANQ2	; -7.1414530935e+01 (temp real)
	retlw	0x66 ; (0) LSB
	retlw	0x3d ; (1)
	retlw	0xd4 ; (2)
	retlw	0x8e ; (3) MSB
	retlw	0x85 ; (4) EXP
	retlw	0x80 ; (5) SIGN

M_TANQ3	; 2.6973501312e+02 (temp real)
	retlw	0xe9 ; (0) LSB
	retlw	0x14 ; (1)
	retlw	0xde ; (2)
	retlw	0x86 ; (3) MSB
	retlw	0x87 ; (4) EXP
	retlw	0x00 ; (5) SIGN

_FP_TAN:	; FPac = TAN(FPac)
 	TRarg_TABLE_LD	M_TAN_SCALE	; TRarg = M_TAN_SCALE

	FP_MUL
	FP_QUAD
	JMP_BCLR	FPquad,0,_FP_tan_1 
	FParg_ONE			; FParg = 1.0
	FPac_SWAP
	FP_SUB			; xp = 1.0 - xp
_FP_tan_1
	TABLE_SET_TRarg	; TRarg = Default Table load
	TRac_STORE	FParg1		; FParg1 = xp
	FPac_CLR			; p = 0
	TABLE_SET_SOURCE	M_TANP1
	POLY_FParg1		; p += Constant, p *= xp
	FP_MUL			; p *= xp
	POLY_FParg1		; p += Constant, p *= xp
	TRac_STORE	FParg2	; p

	TABLE_SET_SOURCE	M_TANQ1
	FPac_CLR		; p = 0
	POLY_FParg1		; q += Constant, q *= xp
	FP_MUL			; q *= xp
	POLY_FParg1		; q += Constant, q *= xp
	FP_MUL			; q *= xp

	TABLE_LD		; FParg = Constant
	FP_ADD			; q += Constant
	TRarg_LOAD	FParg2			; FPac = q, FParg = p

	AND8I	FPquad,3
	
	CMP8I	FPquad,1			; FPquad == 1 
	JMP_Z	_fp_tan_2			; q / p	
	CMP8I	FPquad,2			; FPquad == 2 ?
	JMP_Z	_fp_tan_2			; q / p
	FPac_SWAP					; p / q
_fp_tan_2
	FP_DIV	
	JMP_BCLR	FPquad,1,_fp_tan_3	; FP_quad & 4
	XOR8I	FPac_SIGN,0x80		; xp = -xp
_fp_tan_3
	XOR8	FPac_SIGN,FPflag	; xp ^= FPflag
	ENDSUB

;===============================================================
;	
;	ATAN(x) = x * P(x*x)/Q(x*x)
;	5093, pg240
;===============================================================
M_ATANP1	; 1.7630401244e-01 (temp real)
	retlw	0xfe ; (0) LSB
	retlw	0x09 ; (1)
	retlw	0x89 ; (2)
	retlw	0xb4 ; (3) MSB
	retlw	0x7c ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ATANP2	; 5.6710794516e+00 (temp real)
	retlw	0x9d ; (0) LSB
	retlw	0x7b ; (1)
	retlw	0x79 ; (2)
	retlw	0xb5 ; (3) MSB
	retlw	0x81 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ATANP3	; 2.2376096451e+01 (temp real)
	retlw	0xdb ; (0) LSB
	retlw	0x3e ; (1)
	retlw	0x02 ; (2)
	retlw	0xb3 ; (3) MSB
	retlw	0x83 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ATANP4	; 1.9818457042e+01 (temp real)
	retlw	0x35 ; (0) LSB
	retlw	0x33 ; (1)
	retlw	0x8c ; (2)
	retlw	0x9e ; (3) MSB
	retlw	0x83 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ATANQ1	; 1.0000000000e+00 (temp real)
	retlw	0x00 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x00 ; (2)
	retlw	0x80 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ATANQ2	; 1.1368190430e+01 (temp real)
	retlw	0xa6 ; (0) LSB
	retlw	0x1b ; (1)
	retlw	0xe4 ; (2)
	retlw	0xb5 ; (3) MSB
	retlw	0x82 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ATANQ3	; 2.8982246397e+01 (temp real)
	retlw	0x00 ; (0) LSB
	retlw	0xa4 ; (1)
	retlw	0xdb ; (2)
	retlw	0xe7 ; (3) MSB
	retlw	0x83 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_ATANQ4	; 1.9818457060e+01 (temp real)
	retlw	0x37 ; (0) LSB
	retlw	0x33 ; (1)
	retlw	0x8c ; (2)
	retlw	0x9e ; (3) MSB
	retlw	0x83 ; (4) EXP
	retlw	0x00 ; (5) SIGN


_FP_ATAN:	; FPac = ATAN(FPac)
	CLR8	FPquad
	JMP_BCLR	FPac_SIGN,7,_FP_atanp
	CLR8	FPac_SIGN
	BSET	FPquad,2	; FPquad |= 4
_FP_atanp
	FParg_ONE		; FParg = 1.0
	FP_CMP
	JMP_L	_FP_atan1	; xp < 1.0
	FPac_SWAP		; FPac = 1.0, FParg = xp
	FP_DIV
	BSET	FPquad,1	; FPquad |= 2
_FP_atan1

	TRac_STORE	FParg1		; FParg1 = xp

	TABLE_SET_TRarg	; TRarg = Default Table load
	TABLE_SET_SOURCE	M_ATANP1
	FPac_CLR			; p = 0
	LOAD8I	FP_cnt,3
	POLY_FParg1		; p += Constant, p *= xp
	FP_MUL			; p *= xp
	POLY_FParg1		; p += Constant, p *= xp
	FP_MUL			; p *= xp
	POLY_FParg1		; p += Constant, p *= xp
	FP_MUL			; p *= xp
	POLY_FParg1		; p += Constant, p *= xp
	TRac_STORE	FParg2		; p

	TABLE_SET_SOURCE	M_ATANQ1
	FPac_CLR			; q = 0
	POLY_FParg1		; q += Constant, q *= xp
	FP_MUL			; q *= xp
	POLY_FParg1		; q += Constant, q *= xp
	FP_MUL			; q *= xp
	POLY_FParg1		; q += Constant, q *= xp
	FP_MUL			; q *= xp
	TABLE_LD		; q += Constant
	FP_ADD		

	TRarg_LOAD	FParg2		; FPac = q, FParg = p

	FPac_SWAP		; FPac = p, FParg = q
	FP_DIV	
	CLR8	FPac_SIGN	; FPac = abs(FPac)

	JMP_BCLR	FPquad,1,_FP_atan2
	XOR8I	FPac_SIGN,0x80	; xp = -xp
 	TRarg_TABLE_LD	M_PI_2 ; TRarg = M_PI/2
	FP_ADD		; xp += M_PI_2 
_FP_atan2
	JMP_BCLR	FPquad,2,_FP_atan3
	XOR8I	FPac_SIGN,0x80	; xp = -xp
_FP_atan3
	ENDSUB

;===============================================================
;	ASIN(x)
;	Use ATAN to compute
;===============================================================

_FP_ASIN:	; FPac = ASIN(FPac)
	LOAD8I	FPflag2,1
	JMP	_FP_asincos


;===============================================================
;	ACOS(x)
;	Use ATAN to compute
;===============================================================
_FP_ACOS:	; FPac = ACOS(FPac)
	LOAD8I	FPflag2,0
	JMP	_FP_asincos


_FP_asincos
	CLR8	FPquad;
	JMP_BCLR	FPac_SIGN,7,_fp_asinacosp
	CLR8	FPac_SIGN
	BSET	FPquad,1
_fp_asinacosp
	TRac_STORE	FParg2	; xp

	TRac_STORE	FParg	; xp * xp
	FP_MUL		; xp * xp

	FParg_ONE		
	FPac_SWAP
	FP_SUB			; 1 - xp * xp
	FP_SQRT			; FPac = fp_sqrt(1-xp*xp)
	TRarg_LOAD	FParg2	; FParg = xp

	TST8	FPflag2
	JMP_Z	_fp_asinacos1	; ACOS ? (fp_sqrt(1-xp*xp)/xp)
	FPac_SWAP		; ASIN ? (xp / fp_sqrt(1-xp*xp)
_fp_asinacos1
	FParg_TST		; tesing for divide by zero
	JMP_NZ	_fp_asincos3	; Nope

;	Divide by zero return +/- PI/2.0 based on FPquad
 	TRarg_TABLE_LD	M_PI_2	; TRarg = M_PI/2
	JMP_BCLR	FPquad,1,_fp_asincose
	LOAD8I	FParg_SIGN,0x80
_fp_asincose
	FPac_SWAP		; FPac = +/- M_PI
	ENDSUB

_fp_asincos3
	FP_DIV	; acos = atan(x / xp), asin=atan(xp / x)
	JMP_BCLR	FPquad,1,_fp_asincosx
	LOAD8I	FPac_SIGN,0x80
_fp_asincosx
	JMP	_FP_ATAN


;===============================================================
;	LOGe(X) = z*P(z**2), z = (x-1)/(x+1)
;	T2663 pg193 
;===============================================================
M_LOG1	; 3.4281977560e-01 (temp real)
	retlw	0xd9 ; (0) LSB
	retlw	0x12 ; (1)
	retlw	0x86 ; (2)
	retlw	0xaf ; (3) MSB
	retlw	0x7d ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LOG2	; 4.1153426427e-01 (temp real)
	retlw	0x7c ; (0) LSB
	retlw	0x9e ; (1)
	retlw	0xb4 ; (2)
	retlw	0xd2 ; (3) MSB
	retlw	0x7d ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LOG3	; 5.7708664292e-01 (temp real)
	retlw	0x42 ; (0) LSB
	retlw	0xf3 ; (1)
	retlw	0xbb ; (2)
	retlw	0x93 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LOG4	; 9.6179664833e-01 (temp real)
	retlw	0x1e ; (0) LSB
	retlw	0x4e ; (1)
	retlw	0x38 ; (2)
	retlw	0xf6 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_LOG5	; 2.8853900818e+00 (temp real)
	retlw	0x29 ; (0) LSB
	retlw	0x3b ; (1)
	retlw	0xaa ; (2)
	retlw	0xb8 ; (3) MSB
	retlw	0x80 ; (4) EXP
	retlw	0x00 ; (5) SIGN

;===============================================================
_FP_LOG ; FPac = LOG(FPac)
	FP_FREXP		;	x = fp_frexp(x, &EXP);
	movwf	FPflag				; EXP
 	TRarg_TABLE_LD	M_SQRT2 ; TRarg = SQRT(2)
	FP_MUL				; x *= SQRT(2)

;	Compute X = (X-1.0) / (X+1.0)
;	We use a trick to avoid saving X
;
;	X = X + 1.0
	FParg_ONE	
	FP_ADD
	TRac_STORE	FParg1	; X + 1.0

	; X = X - 2.0 (Really X = X - 1.0)
	FParg_ONE	
	INC8	FParg_EXP
	FP_SUB		; FPac = X - 1.0

	TRarg_LOAD	FParg1	; X + 1.0
	FP_DIV		;	X = (X-1.0)/(X+1.0)

	TRac_STORE	FParg1
	
	TABLE_SET_TRarg	; TRarg = Default Table load
	TABLE_SET_SOURCE	M_LOG1
	FPac_CLR		; FPac = 0

	LOAD8I	FP_cnt,4
_fp_log_loop
	POLY_FParg1		; r += Constant, r *=X 
	FP_MUL			; r *= X
	DJNZ8	FP_cnt,_fp_log_loop
	POLY_FParg1		; r += Constant, p *= X 

 	TRarg_TABLE_LD	M_HALF ; TRarg = 1/2
	FP_SUB			; r -= 0.5
	FPac_SWAP

	MOV8	AC32_LSB,FPflag		
	ITOF8
	FP_ADD			; r += EXP
 	TRarg_TABLE_LD	M_LN2 ; TRarg = Ln(2)
	FP_MUL
	ENDSUB

;===============================================================
;	EXP(X)
;	T1043 pg170 
;	
;	exp(X) = P(X) , 2 ** X/LN(2) == E ** X
;===============================================================
M_EXP1	; 2.1702255470e-04 (temp real)
	retlw	0x66 ; (0) LSB
	retlw	0x8c ; (1)
	retlw	0x90 ; (2)
	retlw	0xe3 ; (3) MSB
	retlw	0x72 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_EXP2	; 1.2439687821e-03 (temp real)
	retlw	0x79 ; (0) LSB
	retlw	0xaa ; (1)
	retlw	0x0c ; (2)
	retlw	0xa3 ; (3) MSB
	retlw	0x75 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_EXP3	; 9.6788409973e-03 (temp real)
	retlw	0x63 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x94 ; (2)
	retlw	0x9e ; (3) MSB
	retlw	0x78 ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_EXP4	; 5.5483341984e-02 (temp real)
	retlw	0x35 ; (0) LSB
	retlw	0x80 ; (1)
	retlw	0x42 ; (2)
	retlw	0xe3 ; (3) MSB
	retlw	0x7a ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_EXP5	; 2.4022983627e-01 (temp real)
	retlw	0x69 ; (0) LSB
	retlw	0xcf ; (1)
	retlw	0xfe ; (2)
	retlw	0xf5 ; (3) MSB
	retlw	0x7c ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_EXP6	; 6.9314698384e-01 (temp real)
	retlw	0xab ; (0) LSB
	retlw	0x14 ; (1)
	retlw	0x72 ; (2)
	retlw	0xb1 ; (3) MSB
	retlw	0x7e ; (4) EXP
	retlw	0x00 ; (5) SIGN

M_EXP7	; 1.0000000019e+00 (temp real)
	retlw	0x04 ; (0) LSB
	retlw	0x00 ; (1)
	retlw	0x00 ; (2)
	retlw	0x80 ; (3) MSB
	retlw	0x7f ; (4) EXP
	retlw	0x00 ; (5) SIGN

;===============================================================


_FP_EXP ; FPac = EXP(FPac)
 	TRarg_TABLE_LD	 M_LN2 ; TRarg = Ln(2)
	FP_DIV

	CLR8	FPquad;
	JMP_BCLR	FPac_SIGN,7,_fp_exp1
	CLR8	FPac_SIGN
	LOAD8I	FPquad,1
_fp_exp1

	FP_SPLIT	; FPac = integer(real) , FParg = fraction(xp)
	FP_U32		; AC32 = integer(fixed)
	MOV8	FPflag,AC32_LSB

	TRarg_STORE	FParg1

	TABLE_SET_TRarg	; TRarg = Default Table load
	TABLE_SET_SOURCE	M_EXP1
	FPac_CLR	; FPac = 0

	LOAD8I	FP_cnt,6
_fp_exp_loop
	POLY_FParg1		; r += Constant, r *=X 
	DJNZ8	FP_cnt,_fp_exp_loop
	TABLE_LD		; r += Constant
	FP_ADD

	movfw	FPflag
	FP_LDEXP

	JMP_BCLR	FPquad,0,_fp_expx
	FParg_ONE			; if(sign) r = 1.0 / r
	FPac_SWAP
	FP_DIV
_fp_expx
	ENDSUB

;===============================================================
;POW(x,y)	X**Y
;===============================================================
_FP_POW:	; FPac = POW(FPac,FParg)
;	X == 0 ?
	FPac_TST
	JMP_NZ	_FP_POW_X_NZ	; Nope
;	X == 0.0
;	if(Y != 0.0)
;	return((float) 0.0);
	FParg_TST		; Y != 0.0
	JMP_NZ	_FPac_ZERO	; return(0.0)
;	Y == 0
;	DOMAIN ERROR
_FP_POW_DOM
	OR8I	FP_err,FP_DOM	; Y == 0, DOMAIN ERROR
	JMP		_FPac_ZERO	; return(0.0);
;===============================================================
;	X != 0
;	else {
;	if(Y == (float) 0.0)
;	return(1.0);
;	}
_FP_POW_X_NZ
	FParg_TST
	JMP_Z	_FPac_ONE	; return(1.0)
;===============================================================
;	X != 0, Y != 0
;	See if Y is integer and if so use iexp
;
	TRac_STORE	FParg1		; Save X
	FPac_SWAP	; Y,X
	FP_SPLIT			; FPac = (float) INT(Y), FParg = FRAC(Y)
	FParg_TST			; FParg not an int ?
	JMP_NZ	_FP_POW_NTST	; nope
	; is a integer
	FP_I32			; AC32 = INT(Y)
	FPac_SWAP			; ARG32 = int(Y)
	TRac_LOAD	FParg1		; Restore X
	JMP	_FP_IPOW_INT	; was a integer
	;
_FP_POW_NTST
	TRac_LOAD	FParg1		; Restore X
;	X < 0 ?
	JMP_BSET	FPac_SIGN,7,_FP_POW_DOM
;===============================================================
;	compute pow(X,Y) = fp_exp(Y * fp_log(X));
;	X > 0
_FP_POW_FN
	TRarg_STORE	FParg2	; Y = FParg2, LOG uses FParg1
	FP_LOG		; FPac = log(X)
	TRarg_LOAD	FParg2
	FP_MUL		; Y * log(X)
	FP_EXP		; FPac = exp(Y * log(X))
	ENDSUB
	;

;===============================================================
;	Computer POW(X,Y) where Y is interger
;===============================================================
;	FPac = X ** Y ,	X = FPac, Y = ARG32
_FP_IPOW:	; FPac = IPOW(FPac,ARG32)
;	X == 0 ?
	FPac_TST
	JMP_NZ	_FP_IPOW_X_NZ	; Nope
;	X == 0.0 ?
;	if(Y != 0)
;		return((float) 0.0);
	ARG32_TST				; if(Y != 0)
	JMP_NZ	_FPac_ZERO		;	return(0.0)
;	Y == 0
;	DOMAIN ERROR
_FP_IPOW_DOM
	OR8I	FP_err,FP_DOM	; if(Y == 0) // DOMAIN ERROR
	JMP		_FPac_ZERO	    ;   return(0.0);
;===============================================================
;	X != 0
;	else {
;	if(Y == (float) 0.0)
;	return(1.0);
;	}
_FP_IPOW_X_NZ
	ARG32_TST		; Y == 0 ?
	JMP_Z	_FPac_ONE	; return(1.0)
	;
;===============================================================
;	FPac = FPac ** ARG32
;	All tests on Y have now been done
_FP_IPOW_INT
	CLR8	FPflag2
	TRac_STORE	FParg1	; save X
	FPac_SWAP		; AC32 = Y

;	AC32 = Y
;	FParg = X
	JMP_BCLR	AC32_MSB,7,_FP_IPOW_P
;	if(Y <0) {
;	Y = - Y;
;	--Y;
;	FPflag2 = 1;
;	}
	AC32_NEG		; Y = -Y
	AC32_DEC		; Y--
	BSET	FPflag2,0
	JMP	_FP_IPOW_INIT
	;
_FP_IPOW_P
;	else {
;	Y--;
;	}
	AC32_DEC	; Y--
	;
_FP_IPOW_INIT
	AC32_STORE	FParg2	; Y
	TRarg_LOAD	FParg1	; a = X
_FP_IPOW_MAIN
;	FParg = a
;	FParg1 = X
;	FParg2 = Y
;	while(Y) {
;	if(Y & 0x01)
;	X *= a;
;	if(Y >>= 1)
;	a *= a;
;	}
	AC32_LOAD	FParg2	; Y
	AC32_TST
	JMP_Z	_FP_IPOW_DONE
	JMP_BCLR	AC32_LSB,0,_FP_IPOW_EVEN
	;
	TRac_LOAD	FParg1	; X
	FP_MUL		; FParg = a
	TRac_STORE	FParg1 ; X *= a
	;
_FP_IPOW_EVEN
	AC32_LOAD	FParg2	; Y
	AC32_SH_LSB
	AC32_STORE	FParg2	; Y >>= 1
	AC32_TST
	JMP_Z	_FP_IPOW_DONE
	;
	TRac_LOAD	FParg	; a
	FP_MUL
	TRac_STORE	FParg	; a = a * a
	JMP	_FP_IPOW_MAIN
	;
_FP_IPOW_DONE
	TRac_LOAD	FParg1
	JMP_BCLR	FPflag2,0,_FP_IPOW_EXIT
; 1 / X
    FParg_ONE
    FPac_SWAP
    JMP     _FP_DIV

_FP_IPOW_EXIT
	ENDSUB

;	TODO - trivial
;	LOG10(X)
;	POW10(X)
;	SINH(x)
;	COSH(x)
;	TANH(x)
#endif
