;File:	ac32.asm
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
;	AC32 Subroutines go here
;
;	We build on macro.h to make common macros for our 32bit accumulator machine
;
;	Almost all of of the macros here work on AC32 , while ARG32 and TMP32
;	have only a few entries. The idea is reduce the ammount of code
;	used as much as possible. So, having subroutines only makes sense if
;	they are called more then once!
;
;	=========================================================================

#ifndef	AC32_ASM
#define	AC32_ASM

;	INCLUDE "macro.h"
;	INCLUDE "ac32.h"
;	INCLUDE "mem.h"
;	==================================================================================

;	=========================================================================
;	LOAD/STORE subroutines for AC32,ARG32,TMP32

_AC32_LOAD:	; Load AC32 = (W)
	movwf	mem_from
	LOAD8I	mem_to,AC32
	JMP	MEM_MOV4

_ARG32_LOAD:	; Load ARG32 = (W)
	movwf	mem_from
	LOAD8I	mem_to,ARG32
	JMP	MEM_MOV4

_TMP32_LOAD:	; Load TMP32 = (W)
	movwf	mem_from
	LOAD8I	mem_to,TMP32
	JMP	MEM_MOV4

_AC32_STORE:	; Store (W) = AC32
	movwf	mem_to
	LOAD8I	mem_from,AC32
	JMP	MEM_MOV4

_ARG32_STORE:	; Store (W) = ARG32
	movwf	mem_to
	LOAD8I	mem_from,ARG32
	JMP	MEM_MOV4

_TMP32_STORE:	; Store (W) = TMP32
	movwf	mem_to
	LOAD8I	mem_from,TMP32
	JMP	MEM_MOV4


;	=========================================================================
;	Subroutines: Unary operations on AC32 only 
;
_AC32_CLR:	; AC32 = 0
	IN_CLR		AC32,4
	ENDSUB

_AC32_COM:	; AC32 = ~AC32
	IN_COM		AC32,4
	ENDSUB

_AC32_DEC:	; AC32 -= 1
	IN_DEC		AC32,4
	ENDSUB

_AC32_INC:	; AC32 += 1
	IN_INC		AC32,4
	ENDSUB

_AC32_NEG:	; AC32 - AC32
	AC32_COM
	JMP		_AC32_INC

_AC32_UMAX:	; AC32 = U32_MAX
	CALL	_AC32_CLR
	JMP		_AC32_COM

_AC32_IMIN:	; AC32 = I32_MIN
	CALL	_AC32_CLR
	BSET	AC32_MSB,7
	ENDSUB

_AC32_IMAX:	; AC32 = I32_MAX
	CALL	_AC32_CLR
	AC32_COM
	BCLR	AC32_MSB,7
	ENDSUB


_AC32_SH_LSB:	; AC32 >>= 1
	CLRC
_AC32_SH_LSBC:	; AC32 >>= 1 with carry
	IN_SH_LSBC	AC32,4
	ENDSUB

_AC32_SH_MSB:	; AC32 <<= 1
	CLRC
_AC32_SH_MSBC:	; AC32 <<= 1 with carry
	IN_SH_MSBC	AC32,4
	ENDSUB

;
_AC32_TST:	; Test AC32 == 0 ?
	IN_TST		AC32,4
	ENDSUB

;	=========================================================================
;	Subroutines: Binary Operations on AC32 and ARG32
;
_AC32_ADD:	; AC32 += ARG32
	CLRC
_AC32_ADDC:	; AC32 += ARG32 with Carry
	IN_ADDC		AC32,ARG32,4
	ENDSUB

_AC32_CMP:	; Compare AC32 and ARG32, Set Status as if AC32 - ARG32
	IN_CMP		AC32,ARG32,4
	ENDSUB

_AC32_SWAP:	; SWAP AC32 and ARG32 
	IN_SWAP		AC32,ARG32,4
	ENDSUB

_AC32_SUB:	; AC32 -= ARG32
	CLRB
_AC32_SUBB:	; AC32 -= ARG32 with borrow
	IN_SUBB		AC32,ARG32,4
	ENDSUB

;	=========================================================================
;	Subroutines: CONVERSION FUNCTIONS on AC32, signed and unsigned
;
;	Convert I8,I16,I24 and U8,U16,U24 bit numbers to 32bit form
;
_I8_TO_I16:	; Sign extend AC32 as 8bit to 16bit number
	JMP_BSET	AC32+0,7,_N8_TO_16
_U8_TO_U16:	; Unsigned extend AC32 as 8bit to 16bit unsigned number
	LOAD8I AC32+1,0
	ENDSUB
_N8_TO_16
	LOAD8I AC32+1,0ffh
	ENDSUB

_I8_TO_I32:	; Sign extend AC32 as 8bit to 32bit number
	JMP_BSET	AC32+0,7,_N8_TO_32
	JMP _U8_TO_U32

_I16_TO_I32:	; Sign extend AC32 as 16bit to 32bit number
	JMP_BSET	AC32+1,7,_N16_TO_32
	JMP _U16_TO_U32

_I24_TO_I32:	; Sign extend AC32 as 24bit to 32bit number

	JMP_BSET	AC32+2,7,_N24_TO_32
	JMP _U24_TO_U32

;	Non Negative Numbers
_U8_TO_U32:	; Unsigned extend AC32 as 8bit to 32bit unsigned number
	LOAD8I AC32+1,0
_U16_TO_U32:	; Unsigned extend AC32 as 16bit to 32bit unsigned number
	LOAD8I AC32+2,0
_U24_TO_U32:	; Unsigned extend AC32 as 24bit to 32bit unsigned number
	LOAD8I AC32+3,0
	ENDSUB

;	Sign Extend Negative Numbers
_N8_TO_32
	LOAD8I AC32+1,0ffh
_N16_TO_32
	LOAD8I AC32+2,0ffh
_N24_TO_32
	LOAD8I AC32+3,0ffh
	ENDSUB


;	=========================================================================
;	=========================================================================


;	=================================================================
;	ARG SUBROUTINES
;
_ARG32_SH_LSB:	; ARG32 >>= 1
	CLRC
_ARG32_SH_LSBC:	; ARG32 >>= 1 with carry
	IN_SH_LSBC	ARG32,4
	ENDSUB

_ARG32_SH_MSB:	; ARG32 <<= 1
	CLRC
_ARG32_SH_MSBC:	; ARG32 <<= 1 with carry
	IN_SH_MSBC	ARG32,4
	ENDSUB

_ARG32_TST:	; Test ARG32 == 0 ?
	IN_TST		ARG32,4
	ENDSUB

_ARG32_CLR:	; ARG32 = 0
	IN_CLR		ARG32,4
	ENDSUB

;	=========================================================================


;	=================================================================
;	TMP SUBROUTINES
;

_TMP32_SH_LSB:	; TMP32 >>= 1
	CLRC
_TMP32_SH_LSBC:	; TMP32 >>= 1 with carry
	IN_SH_LSBC	TMP32,4
	ENDSUB

_TMP32_SH_MSB:	; TMP32 <<= 1
	CLRC
_TMP32_SH_MSBC:	; TMP32 <<= 1 with carry
	IN_SH_MSBC	TMP32,4
	ENDSUB

_TMP32_TST:	; Test TMP32 == 0 ?
	IN_TST		TMP32,4
	ENDSUB

_TMP32_CLR:	; TMP32 = 0
	IN_CLR	TMP32,4
	ENDSUB

;	===================================================================


;	==========================================================================	
;
;
;	AC32_MUL10 Fast FIXED POINT * 10
;
;
_AC32_MUL10:	; AC32 *= 10 (fast)
	AC32_STORE	ARG32
	AC32_SH_MSB
	AC32_SH_MSB
	AC32_ADD
	AC32_SH_MSB
	ENDSUB

#ifdef TODO
;	=========================================================================
;	TODO UNTESTED
;	32BIT Unsigned Divide
;
;	AC32 = AC32 / ARG32
;	TMP32 = AC32 % ARG32
;	Used: AC32,TMP32,ML_cnt
;
_AC32_UDIV:	; AC32 = AC32 / ARG32, TMP32 = AC32 % ARG32
	LOAD8I ML_cnt,32	; 32 Bits
	TMP32_CLR			; Remainder = 0
_AC32_UDIV_LOOP
	AC32_SH_MSB			; AC32 = Number we divide
	TMP32_SH_MSBC		; TMP32 = Remainder
	IN_SUB	TMP32,ARG32,4	; TMP32 -= ARG32
	JMP_L	_AC32_UDIV_ADD	; TMP32 < 0 ? ARG32 > TMP32
	BSET	AC32_LSB,0		; TMP32 >= 0, AC32 |= 1
	DJNZ8	ML_cnt,_AC32_UDIV_LOOP
	ENDSUB
_AC32_UDIV_ADD
	IN_ADD	TMP32,ARG32,4	; yes TMP32 was < 0 so restore
	DJNZ8	ML_cnt,_AC32_UDIV_LOOP
	ENDSUB

;	=========================================================================
;	TODO UNTESTED
;	32BIT Unsigned Multiply, 64bit result
;
;	TMP32:AC32 = AC32 * ARG32
;	Used: AC32,TMP32,ML_cnt
;	Notes: If you want a 32bit result call TMP32_TST afterwords
;	a result of NZ flags overflow
;
_AC32_UMUL:	; TMP32:AC32 = AC32 * ARG32 (64bit result)
	LOAD8I ML_cnt,32	; 32 Bits
	TMP32_CLR	; Clear upper 32 bits of result
_AC32_UMUL_LOOP
;	ROTATE ARG32 RIGHT, so after 32 bits ARG32 will be restored!
;	Carry determines if we add
	ARG32_SH_LSB	; Carry = LSB
	JMP_NC	_AC32_UMUL_ADD
	BSET	ARG32_MSB,7	; Restore MSB
_AC32_UMUL_ADD
	IN_ADD	AC32,TMP32,4	; AC32 += TMP32
_AC32_UMUL_RESULT
	AC32_SH_LSBC	; MSB = Carry	
	TMP32_SH_LSBC	; MSB = Carry
	DJNZ8	ML_cnt,_AC32_UMUL_LOOP
	ENDSUB

#endif

;	================================================================
;	END INCLUDE AC32.ASM
#endif

