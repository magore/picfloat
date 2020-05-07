;File:	ac32.h
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
#ifndef	AC32_H
#define	AC32_H

;	INCLUDE "macro.h"
;	=========================================================================
;
;	We build on macro.h to make common macros for our 32bit accumulator machine
;
;	Almost all of of the macros here work on AC32 , while ARG32 and TMP32
;	have only a few entries. The idea is reduce the ammount of code
;	used as much as possible. So, having subroutines only makes sense if
;	they are CALLed more then once!
;

;Primary	32 bit ACC 

;	AC32,ARG32, TMP32 with extra two bytes for FP overlays
;	Note: !!!!! we assume that AC32 and TMP32 are next to each other in code!!!!!
	cblock 
	AC32:6, TMP32:6, ARG32:6
	endc

;	tmp vars used by AC32 and ATOF
	cblock
	ML_int, ML_cnt, ML_tmp
	endc

AC32_MSB	equ AC32+3
AC32_LSB	equ AC32+0

ARG32_MSB	equ ARG32+3
ARG32_LSB	equ ARG32+0

TMP32_MSB	equ TMP32+3
TMP32_LSB	equ TMP32+0

AC16	equ AC32
AC8	equ	AC32

ARG16	equ	ARG32
ARG8	equ	ARG32

TMP16	equ	TMP32
TMP8	equ	TMP32

U32_MAX	equ	0xffffffff
I32_MAX	equ	0x7fffffff
I32_MIN equ 0x80000000


	
;	=========================================================================
;	IMMEDIATE data on AC32
;
;	Notes: using tables instead would reduce - if you use alot of these!
; ADD argument to AC32
AC32_LOADI	MACRO	AA
	IN_LOADI	AC32,AA,4
	ENDM

; ADD argument to AC32 constant
AC32_ADDI	MACRO	AA
	IN_ADDI	AC32,AA,4
	ENDM

; SUB argument from AC32
AC32_SUBI	MACRO	AA
	IN_SUBI	AC32,AA,4
	ENDM

;	=========================================================================
;	LOAD/STORE MACROS for AC32,ARG32,TMP32
;
;
; Load AC32
AC32_LOAD	MACRO	AA
	movlw	AA
	CALL	_AC32_LOAD
	ENDM

; Load ARG32
ARG32_LOAD	MACRO	AA
	movlw	AA
	CALL	_ARG32_LOAD
	ENDM

; Load TMP32
TMP32_LOAD	MACRO	AA
	movlw	AA
	CALL	_TMP32_LOAD
	ENDM

; Store AC32
AC32_STORE	MACRO	AA
	movlw	AA
	CALL	_AC32_STORE
	ENDM

; Store ARG32
ARG32_STORE	MACRO	AA
	movlw	AA
	CALL	_ARG32_STORE
	ENDM

; Store TMP32
TMP32_STORE	MACRO	AA
	movlw	AA
	CALL	_TMP32_STORE
	ENDM

;	=========================================================================
;	Unary operations on AC32 only
;
; Clear AC32
AC32_CLR	MACRO
	CALL	_AC32_CLR
	ENDM

; COM AC32
AC32_COM	MACRO
	CALL	_AC32_COM
	ENDM

; DEC AC32
AC32_DEC	MACRO
	CALL	_AC32_DEC
	ENDM

; INC AC32
AC32_INC	MACRO
	CALL	_AC32_INC
	ENDM

; NEG AC32
AC32_NEG	MACRO
	CALL	_AC32_NEG
	ENDM

; AC32 = U32_MAX
AC32_UMAX	MACRO
	CALL	_AC32_UMAX
	ENDM

; AC32 = I32_MAX
AC32_IMAX	MACRO
	CALL	_AC32_IMAX
	ENDM

; AC32 = I32_MIN
AC32_IMIN	MACRO
	CALL	_AC32_IMIN
	ENDM

; Shift AC32 right towards LSB
AC32_SH_LSB	MACRO
	CALL	_AC32_SH_LSB
	ENDM

; Shift AC32 Left towards LSB with carry
AC32_SH_LSBC	MACRO
	CALL	_AC32_SH_LSBC
	ENDM

; Shift AC32 left towards MSB
AC32_SH_MSB	MACRO
	CALL	_AC32_SH_MSB
	ENDM

; Shift AC32 left towards MSB with carry
AC32_SH_MSBC	MACRO
	CALL	_AC32_SH_MSBC
	ENDM

; Test AC32 setiing zero status
AC32_TST	MACRO
	CALL	_AC32_TST
	ENDM

;	=========================================================================
;	Binary Operations on AC32 and ARG32
;
; ADD AC32 and ARG32
AC32_ADD	MACRO
	CALL	_AC32_ADD
	ENDM

; ADD AC32 and ARG32 with carry
AC32_ADDC	MACRO
	CALL	_AC32_ADDC
	ENDM

; CMP AC32 and ARG32 setting status
AC32_CMP	MACRO
	CALL	_AC32_CMP
	ENDM

; SWAP AC32 and ARG32 
AC32_SWAP	MACRO
	CALL	_AC32_SWAP
	ENDM

; SUB AC32 and ARG32
AC32_SUB	MACRO
	CALL	_AC32_SUB
	ENDM

AC32_SUBB	MACRO
	CALL	_AC32_SUBB
	ENDM

;	=========================================================================
;	AC16 Convert I8,U8 bit numbers to 16bit form
;
; I8_TO_I16() Sign extend AC32 as 8bit to 16bit number
I8_TO_I16	MACRO
	CALL	_I8_TO_I16
	ENDM
; U8_TO_I16() extend AC32 as 8bit to 16bit number
U8_TO_U16	MACRO
	CALL	_U8_TO_U16
	ENDM
;	=========================================================================
;	AC32 Convert I8,I16,I24 and U8,U16,U24 bit numbers to 32bit form
;
; I8_TO_I32() Sign extend AC32 as 8bit to 32bit number
I8_TO_I32	MACRO
	CALL	_I8_TO_I32
	ENDM

; I16_TO_I32() Sign extend AC32 as 16bit to 32bit number
I16_TO_I32	MACRO
	CALL	_I16_TO_I32
	ENDM

; I24_TO_I32() Sign extend AC32 as 24bit to 32bit number
I24_TO_I32	MACRO
	CALL	_I24_TO_I32
	ENDM

; U8_TO_U32() extend AC32 as 8bit to 32bit number
U8_TO_U32	MACRO
	CALL	_U8_TO_U32
	ENDM

; U16_TO_U32() extend AC32 as 16bit to 32bit number
U16_TO_U32	MACRO
	CALL	_U16_TO_U32
	ENDM

; U24_TO_U32() extend AC32 as 24bit to 32bit number
U24_TO_U32	MACRO
	CALL	_U24_TO_U32
	ENDM

; N8_TO_N32() extend AC32 as negative 8bit to 32bit number
N8_TO_N32	MACRO
	CALL	_N8_TO_N32
	ENDM

; N16_TO_N32() extend AC32 as negative 16bit to 32bit number
N16_TO_N32	MACRO
	CALL	_N16_TO_N32
	ENDM

; N16_TO_N32() extend AC32 as negative 16bit to 32bit number
N24_TO_N32	MACRO
	CALL	_N24_TO_N32
	ENDM

;	=======================================================================
;	ARG MACROS
;

; Shift ARG32 Left towards LSB 
ARG32_SH_LSB	MACRO
	CALL	_ARG32_SH_LSB
	ENDM

; Shift ARG32 Left towards LSB with carry
ARG32_SH_LSBC	MACRO
	CALL	_ARG32_SH_LSBC
	ENDM

; Shift ARG32 Right towards MSB 
ARG32_SH_MSB	MACRO
	CALL	_ARG32_SH_MSB
	ENDM

; Shift ARG32 Right towards MSB with carry
ARG32_SH_MSBC	MACRO
	CALL	_ARG32_SH_MSBC
	ENDM

; TEST ARG32 and set zero status
ARG32_TST	MACRO
	CALL	_ARG32_TST
	ENDM

; CLEAR ARG32 
ARG32_CLR	MACRO
	CALL	_ARG32_CLR
	ENDM
;	=========================================================================
;	TMP MACROS
;

; Shift TMP32 Left towards LSB 
TMP32_SH_LSB	MACRO
	CALL	_TMP32_SH_LSB
	ENDM

; Shift TMP32 Left towards LSB with carry
TMP32_SH_LSBC	MACRO
	CALL	_TMP32_SH_LSBC
	ENDM

; Shift TMP32 Right towards MSB 
TMP32_SH_MSB	MACRO
	CALL	_TMP32_SH_MSB
	ENDM

; Shift TMP32 Right towards MSB with carry
TMP32_SH_MSBC	MACRO
	CALL	_TMP32_SH_MSBC
	ENDM

; TEST TMP32 and set zero status
TMP32_TST	MACRO
	CALL	_TMP32_TST
	ENDM

; CLEAR TMP32
TMP32_CLR	MACRO
	CALL	_TMP32_CLR
	ENDM
;	=========================================================================
; MULTIPLY AC32 by 10
AC32_MUL10	MACRO
	CALL	_AC32_MUL10
	ENDM

; DIVIDE AC32 and ARG32 unsigned
AC32_UDIV	MACRO
	CALL	_AC32_UDIV
	ENDM

; MULTIPLY AC32 and ARG32 unsigned
AC32_UMUL	MACRO
	CALL	_AC32_UMUL
	ENDM

;	======================================================================
;	AC32_H
#endif
