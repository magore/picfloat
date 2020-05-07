;File:	fp.h
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
;	***** Please Use TABS = 4 when viewing this file ****
;	========================================================================
#ifndef	FP_H
#define	FP_H

;	INCLUDE "macro.h"
;
;	Title:	FP support Macros
;
;	Author:	Mike Gore
;
;	Jan 30, 1991
;	Jun , 1997
;	Jan , 2002
;
;
;	Temp Real <biased exponent><sign><mantissa>
;	7 .. 0	   7..0  31 .. 0
;	+BIAS	 	1^ = 1 @ bit 31
;
;	We make some assumptions about the following constants
;

;	=======================================================================

;	Floating point Status and tmp vars
	cblock 
	FP_err,FP_bits,ML_EXP:2,FP_tmp
	endc

;	Assumes that Mantissa offset is 0 and that 2 extra bytes reserved
FPac	equ	AC32
TRac	equ	AC32
FParg	equ	ARG32
TRarg	equ	ARG32
FPtmp	equ	TMP32
TRtmp	equ	TMP32

MAN	equ	0
MAN_LSB	equ	0
MAN_MSB	equ	3
EXP	equ	4
SIGN	equ	5

FPac_EXP	equ	FPac+EXP
FPac_SIGN	equ	FPac+SIGN

FParg_EXP	equ	FParg+EXP
FParg_SIGN	equ	FParg+SIGN

FPtmp_EXP	equ	FPtmp+EXP
FPtmp_SIGN	equ	FPtmp+SIGN

;	=======================================================================
BITS_TR	equ	32		; Bits in Temp real mantissa
BIAS_TR	equ	0x7f	
;
MAXEXP_TR	equ	0xff
MINEXP_TR	equ	0x01
;
; Error BITS
FP_UNDER	equ	1	; Underflow
FP_OVER		equ	2	; Overflow
FP_DOM		equ 3	; Domain 
FP_PANIC	equ	7	; Programmer screwed up 8-)

;	Constants used in Floating conversion routines 
;
_fpc_rnd	equ	0x00000080	; Rounding used prior to FPac_STORE

_fpc_ovr	equ 0xffffffff	; FP Mantissa Overflow

_fpc_und	equ	0x80000000	; FP Mantissa Underflow

_i32c_ovr	equ	0x7fffffff	; Max INT, (2**31)-1

_i32c_und	equ	0x80000000	; Min INT, (2**31)


;	=======================================================================
; Display FP error Status Bits
FP_ERROR	MACRO
	CALL	_FP_ERROR
	ENDM

;	=======================================================================
; Clear and FP error Status Bits
FP_CLR_ERROR	MACRO
	CLR8	FP_err
	ENDM

;	=======================================================================
;	LOAD IEEE32  inline
IEEE_LOADI	MACRO	FP, arg
	LOAD8I	FP+MAN+0,	0
	LOAD8I	FP+MAN+1,	arg & 0xff
	LOAD8I	FP+MAN+2,	(arg >> 8) & 0xff
	LOAD8I	FP+MAN+3,	((arg >> 16) & 0x7f) | 0x80
	LOAD8I	FP+EXP,	(arg >> (16 + 7)) & 0xff
	LOAD8I	FP+SIGN,	(arg >> 24) & 0x80
	ENDM

;	=======================================================================
;	LOAD FPac with TEMP REAL pointed to by AA
TRac_LOAD	MACRO	AA
	movlw	AA
	CALL	_TRac_LOAD
	ENDM

;	STORE FPac with TEMP REAL pointed to by AA
TRac_STORE	MACRO	AA
	movlw	AA
	CALL	_TRac_STORE
	ENDM

;	LOAD FParg with TEMP REAL pointed to by AA
TRarg_LOAD	MACRO	AA
	movlw	AA
	CALL	_TRarg_LOAD
	ENDM

;	STORE FParg with TEMP REAL pointed to by AA
TRarg_STORE	MACRO	AA
	movlw	AA
	CALL	_TRarg_STORE
	ENDM

;	=======================================================================
IEEE_TO_TRac	MACRO
	CALL	_IEEE_TO_TRac
	ENDM

TRac_TO_IEEE	MACRO
	CALL	_TRac_TO_IEEE
	ENDM

IEEE_TO_TRarg	MACRO
	CALL	_IEEE_TO_TRarg
	ENDM

TRarg_TO_IEEE	MACRO
	CALL	_TRarg_TO_IEEE
	ENDM

;	=======================================================================
FPac_SWAP	MACRO
	CALL	_FPac_SWAP
	ENDM

FPac_CLR	MACRO
	CALL	_FPac_CLR
	ENDM

FPac_ONE	MACRO
	CALL	_FPac_ONE
	ENDM

FParg_CLR	MACRO
	CALL	_FParg_CLR
	ENDM

FParg_ONE	MACRO
	CALL	_FParg_ONE
	ENDM

FPac_TST	MACRO
	CALL	_FPac_TST
	ENDM
FParg_TST	MACRO
	CALL	_FParg_TST
	ENDM
FP_CMP	MACRO
	CALL	_FP_CMP
	ENDM

FPac_RND	MACRO
	CALL	_FPac_RND
	ENDM

FPac_INC_EXP	MACRO
	CALL	_FPac_INC_EXP
	ENDM

FPac_DEC_EXP	MACRO
	CALL	_FPac_DEC_EXP
	ENDM

FPac_NORM	MACRO
	CALL	_FPac_NORM
	ENDM

FPac_ZERO	MACRO
	CALL	_FPac_ZERO
	ENDM

FPac_OVER	MACRO
	CALL	_FPac_OVER
	ENDM

FP_ALIGN	MACRO
	CALL	_FP_ALIGN
	ENDM

FP_SUB	MACRO
	CALL	_FP_SUB
	ENDM

FP_ADD	MACRO
	CALL	_FP_ADD
	ENDM

;   AC32 += ARG32, renormalize on overflow, inlined to save stack
AC32_ADD_NORMAL	MACRO
	local	next
    AC32_ADD            ; AC32 + ARG32
    JMP_NC  next
    AC32_SH_LSBC        ; Restore overflow, AC32 >> 1
    FPac_INC_EXP        ; ++fpac_EXP
next
	ENDM

FP_MUL	MACRO
	CALL	_FP_MUL
	ENDM

FP_DIV	MACRO
	CALL	_FP_DIV
	ENDM

;	=====================================================================

ITOF8	MACRO
	CALL	_ITOF8
	ENDM
ITOF16	MACRO
	CALL	_ITOF16
	ENDM
ITOF24	MACRO
	CALL	_ITOF24
	ENDM
ITOF32	MACRO
	CALL	_ITOF32
	ENDM
NTOF8	MACRO
	CALL	_NTOF8
	ENDM
NTOF16	MACRO
	CALL	_NTOF16
	ENDM
NTOF24	MACRO
	CALL	_NTOF24
	ENDM
NTOF32	MACRO
	CALL	_NTOF32
	ENDM
UTOF8	MACRO
	CALL	_UTOF8
	ENDM
UTOF16	MACRO
	CALL	_UTOF16
	ENDM
UTOF24	MACRO
	CALL	_UTOF24
	ENDM
UTOF32	MACRO
	CALL	_UTOF32
	ENDM
FP_U32	MACRO
	CALL	_FP_U32
	ENDM
FP_I32	MACRO
	CALL	_FP_I32
	ENDM
FP_I32_MAX	MACRO
	CALL	_FP_I32_MAX
	ENDM
FP_I32_MIN	MACRO
	CALL	_FP_I32_MIN
	ENDM
FP_SPLIT	MACRO
	CALL	_FP_SPLIT
	ENDM
FP_FREXP	MACRO
	CALL	_FP_FREXP
	ENDM
FP_INVERT	MACRO
	CALL	_FP_INVERT
	ENDM
FP_LDEXP	MACRO
	CALL	_FP_LDEXP
	ENDM
FP_FMOD	MACRO
	CALL	_FP_FMOD
	ENDM
FP_INC_AC	MACRO
	CALL	_FP_INC_AC
	ENDM
FP_DEC_AC	MACRO
	CALL	_FP_DEC_AC
	ENDM
FP_CEIL	MACRO
	CALL	_FP_CEIL
	ENDM
FP_FLOOR	MACRO
	CALL	_FP_FLOOR
	ENDM

;	=====================================================================
FPac_MUL5	MACRO
	CALL	_FPac_MUL5
	ENDM

FPac_MUL10	MACRO
	CALL	_FPac_MUL10
	ENDM

FPac_DIV5	MACRO
	CALL	_FPac_DIV5
	ENDM

FPac_DIV10	MACRO
	CALL	_FPac_DIV10
	ENDM

;	============================================================================
;	ENDIF FP_H
#endif
