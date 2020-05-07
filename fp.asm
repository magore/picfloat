;File:	fp.asm
;================================================================
;	PIC Floating point library
;================================================================
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
;================================================================
;================================================================
;	***** Please Use TABS = 4 when viewing this file ****
;================================================================
#ifndef	FP_ASM
#define	FP_ASM

;	INCLUDE "macro.h"
;	INCLUDE "fp.h"

;================================================================
;	TEMP REAL <EXP> <SIGN> <MANTISSA>
;	          7..0  7..0   31..0
;
;	IEEE 32   <SIGN> <EXP>  <MANTISSA>
;	            31   30..23 22..0
;
;================================================================

;================================================================

_FP_ERROR: 	; Floating Point Error handler, defined by user
#ifdef FP_DEBUG
	TST8	FP_err
	JMP_Z	_FP_ERROR_X
	JMP	_FP_ERROR_X
	DB_PUTCI	'F'
	DB_PUTCI	'P'
	DB_PUTCI	':'
	JMP_BCLR	FP_err,FP_OVER,_FP_ERROR_1
	movlw 	'O'
_FP_ERROR_1
	JMP_BCLR	FP_err,FP_UNDER,_FP_ERROR_2
	movlw	'U'
_FP_ERROR_2
	JMP_BCLR	FP_err,FP_DOM,_FP_ERROR_3
	movlw	'D'
_FP_ERROR_3
	JMP_BCLR	FP_err,FP_PANIC,_FP_ERROR_4
	movlw	'E'
_FP_ERROR_4
	DB_PUTCW
	DB_PUTCI '\n'
_FP_ERROR_X
#endif
	ENDSUB
;================================================================

;================================================================
_IEEE_TO_TRac: 	; Convert IEEE32 number in AC32 to Temporary REAL
	MOV8	FPac_EXP,AC32+3
	MOV8	FPac_SIGN,AC32+3
	AND8I	FPac_SIGN,0x80
	SH8_MSB	FPac_EXP
	JMP_BCLR	AC32+2, 7, _IEEE_TO_TRac1
	BSET	FPac_EXP,0
_IEEE_TO_TRac1
	MOV8	AC32+3,AC32+2
	MOV8	AC32+2,AC32+1
	MOV8	AC32+1,AC32+0
	CLR8	AC32
	BSET	AC32+3,7		; Restore Implied Bit
	ENDSUB

;================================================================
;
;	Convert TRac Temporary REAL into AC32 IEEE format number inplace
;	Source FPac, Target AC32
;
_TRac_TO_IEEE: 	; Convert Temporary REAL into IEEE32 format number in AC32 
;	Make sure we are normalized
	FPac_NORM
;	FPac_RND
; Check for overflow
	CMP8I	FPac_EXP,MAXEXP_TR
	JMP_NZ	_TRac_TO_IEEE_0
	FPac_OVER		; Overflow
	DEC8	FPac_EXP
_TRac_TO_IEEE_0
	BCLR	AC32+3,7		; Reset Implied BIT
	SH8_LSB	FPac_EXP
	OR8		FPac_EXP,FPac_SIGN
	JMP_NC	_TRac_TO_IEEE_1
	BSET	AC32+3,7		; FPac_EXP LSB over implied bit
_TRac_TO_IEEE_1
	MOV8	AC32+0,AC32+1
	MOV8	AC32+1,AC32+2
	MOV8	AC32+2,AC32+3
	MOV8	AC32+3,FPac_EXP
	ENDSUB

;================================================================
;
;	Convert ARG32 IEEE32 format number into TRarg Temporary REAL inplace
;
;
_IEEE_TO_TRarg:	; Convert IEEE32 number in ARG32 to Temporary REAL
	MOV8	FParg_EXP,ARG32+3
	MOV8	FParg_SIGN,ARG32+3
	AND8I	FParg_SIGN,0x80
	SH8_MSB	FParg_EXP
	JMP_BCLR	ARG32+2, 7, _IEEE_TO_TRarg1
	BSET	FParg_EXP,0
_IEEE_TO_TRarg1
	MOV8	ARG32+3,ARG32+2
	MOV8	ARG32+2,ARG32+1
	MOV8	ARG32+1,ARG32+0
	CLR8	ARG32
	BSET	ARG32+3,7		; Restore Implied Bit
	ENDSUB

;================================================================
;
;	Convert TRarg Temporary REAL into ARG32 IEEE format number inplace
;	Source FParg, Target ARG32
;
_TRarg_TO_IEEE:	; Convert Temporary REAL into IEEE32 format number in ARG32 
;	Rounding Done first
	FPac_SWAP
	TRac_TO_IEEE
	JMP	_FPac_SWAP
;================================================================
;	Load FPac with temporary real, W points to address
_TRac_LOAD: 	; Load 40bit temp real in FPac, W points to source address
	movwf	mem_from
	LOAD8I	mem_to,TRac
	JMP	MEM_MOV6

_TRac_STORE: 	; Save 40bit temp real in FPac, W points to target address
	movwf	mem_to
	LOAD8I	mem_from,TRac
	JMP	MEM_MOV6

_TRarg_LOAD: 	;  Load 40bit temp real in FPac, W points to source address
	movwf	mem_from
	LOAD8I	mem_to,TRarg
	JMP	MEM_MOV6

_TRarg_STORE: 	; Save 40bit temp real in FPac , W points to target address
	movwf	mem_to
	LOAD8I	mem_from,TRarg
	JMP	MEM_MOV6

;================================================================
_FPac_SWAP:	; Swap FPac and FParg
	SWAP8		FPac_SIGN,FParg_SIGN
	SWAP8		FPac_EXP,FParg_EXP
	JMP		_AC32_SWAP

;	Clear FPac
_FPac_CLR:	; FPac = 0
	CLR8		FPac_EXP
	CLR8		FPac_SIGN
	JMP		_AC32_CLR

;	Clear FParg
_FParg_CLR:	; FParg = 0
	CLR8		FParg_EXP
	CLR8		FParg_SIGN
	JMP		_ARG32_CLR

_FParg_ONE:	; FParg = 1
	FParg_CLR
	LOAD8I		FParg_EXP,BIAS_TR
	LOAD8I		ARG32_MSB,0x80	; Implied bit
	ENDSUB

_FPac_ONE:	; FPac = 1
	FPac_CLR
	LOAD8I		FPac_EXP,BIAS_TR
	LOAD8I		AC32_MSB,0x80	; Implied bit
	ENDSUB
;================================================================
;
_FPac_TST:	; Return SGN(FPac)
	TST8	FPac_EXP
	JMP_Z	FPac_TST_CLR
;	AC32_TST
	IN_TST	AC32,4
	JMP_Z	FPac_TST_CLR
	JMP_BSET	FPac_SIGN,7,FPac_TST_M
;	PLUS
	RET8I	1
;	MINUS
FPac_TST_M	
	RET8I	0xff
;	ZERO
FPac_TST_CLR
	CLR8	FPac_SIGN
	CLR8	FPac_EXP
;	AC32_CLR
	IN_CLR	AC32,4
	RET8I	0	
	
;
_FParg_TST:	; Return SGN(FParg)
	TST8	FParg_EXP
	JMP_Z	_FParg_TST_CLR
;	ARG32_TST
	IN_TST	ARG32,4
	JMP_Z	_FParg_TST_CLR
	JMP_BSET	FParg_SIGN,7,_FParg_TST_M
	RET8I	1
_FParg_TST_M	
	RET8I 0xff
_FParg_TST_CLR
	CLR8	FParg_SIGN
	CLR8	FParg_EXP
;	ARG32_CLR
	IN_CLR	ARG32,4
	RET8I	0	
	
;================================================================
;
;	FP_CMP()	Compare values of FPac with FParg 
;	Return:
;	Using Borrow (NOT Carry)
;	ac - arg
;	NB,Z ac == arg	JMP_Z
;	B, NZ   ac < arg	JMP_L
;	B, XZ   ac <= arg	JMP_LE
;	NB,NZ ac > arg	JMP_G
;	NB,XZ ac >= arg	JMP_GE
_FP_CMP:	; Return Status (FPac - FParg)
	CMP8	FPac_SIGN,FParg_SIGN
	JMP_Z	_FP_CMPU		; Signs Equal
	JMP_BCLR	FPac_SIGN,7,_FP_CMP_L	; +AC, -ARG
;
;+AC,	-ARG
_FP_CMP_G	; AC < ARG
	SET_L
	ENDSUB
;
;-AC,	+ARG
_FP_CMP_L	; AC > ARG
	SET_G
	ENDSUB
;
_FP_CMPU:	; Return Status Unsigned (FPac - FParg)
	CMP8	FPac_EXP,FParg_EXP
	JMP_NZ	_FP_CMPX
	AC32_CMP	; AC32,ARG32
_FP_CMPX
	ENDSUB
	

;================================================================
;	FPac_RND() Round Value in FPac prior to IEEE storage
; FPac = FPac_RND(FPac) - round FPac prior to conversion to IEEE
; Round to nearest
; When LSB == 0x80 break tie with round to even LSbit in LSB+1
;
_FPac_RND:	; Round Value in FPac prior to IEEE storage
; Zero ?
	AC32_TST 
	JMP_Z	_FPac_RNDX
;
	CMP8I	AC32_LSB,0x80	; Check for special case
	JMP_NZ	_FPac_RND_VAL	; Round normally

; If the lower bit of AC32_LSB+1 == 0 and AC32_LSB == 0x80 then chop
	JMP_BCLR	AC32_LSB+1,0,_FPac_RNDX	
;
_FPac_RND_VAL
	AC32_ADDI	_fpc_rnd
	JMP_NC	_FPac_RNDX
	AC32_SH_LSBC
	JMP	_FPac_INC_EXP
_FPac_RNDX
	ENDSUB

;================================================================

; FPac_INC_EXP() FPac *= 2
_FPac_INC_EXP:	; Increment FPac_EXP if nonzero, test for overflow
	TST8	FPac_EXP
	JMP_Z	_FPac_INC_EXPX
	INC8	FPac_EXP
	JMP_Z	_FPac_OVER
_FPac_INC_EXPX
	ENDSUB

; FPac_DEC_EXP() FPac /= 2
_FPac_DEC_EXP:	; Decrement FPac_EXP if nonzero, test for underflow
	TST8	FPac_EXP
	JMP_Z	_FPac_DEC_EXPX
	DEC8	FPac_EXP
	JMP_Z	_FPac_UNDER
_FPac_DEC_EXPX
	ENDSUB

	

;================================================================
;	FPac_norm() Normalize Floating point ACC
;
_FPac_NORM:	; Normalize Floating Point FPac by shifting implied bit to MSB
	FPac_TST
	JMP_Z	_FPac_normx
_FPac_norm_loop
	JMP_BSET	AC32_MSB, 7, _FPac_normx
	AC32_SH_MSB			; AC32 <<= 1
	DJNZ8	FPac_EXP, _FPac_norm_loop
	JMP		_FPac_UNDER	; FPac_EXP == 0, underflow
_FPac_normx
	ENDSUB

;
;	Return zero in FPac
;
_FPac_ZERO:	; FPac = 0
	JMP	_FPac_CLR
	
;
;	Return overflow in FPac
;
_FPac_OVER:	; FPac = infinity
	LOAD8I	FPac_EXP,MAXEXP_TR	; MAX Exponent
	AC32_LOADI	_fpc_ovr		; Do inline to save stack
;	DEBUG
	BSET	FP_err,FP_OVER
	JMP		_FP_ERROR
;
;	Return underflow in FPac
;
_FPac_UNDER:	; FPac = 1/infinity
	LOAD8I	FPac_EXP,MINEXP_TR	; MIN Exponent
; TODO BUG	
	AC32_LOADI	_fpc_und		; Do inline to save stack
;	DEBUG
	BSET	FP_err,FP_UNDER
	JMP		_FP_ERROR

;================================================================
;
;	Floating point CODE
;	FP_sub	FPac = FPac - FParg
;	FP_add	FPac = FPac + FParg
;	FP_mul	FPac = FPac * FParg
;	FP_div	FPac = FPac / FParg
;
;================================================================

;================================================================
;
_FP_SUB:	; FPac -= FParg
	TRarg_STORE	FPtmp	; FPtmp = FParg
	XOR8I		FParg_SIGN,80h	; FParg_SIGN = -FParg_SIGN
	JMP		_FP_add_num	; Now add them
	;
;================================================================
;
_FP_ADD:	; FPac += FParg
	TRarg_STORE	FPtmp	; FPtmp = FParg
	;
_FP_add_num
;================================================================
;
;	Align FPac and FParg radix points using the largest number.
;	- This will cause both exponents to be equal in value.
;	- If alignment results in total loss of mantissa then 
;	simply clear the mantissa.
;	Notes
;	Shifting a number towards it's LSB requires an adding
;	one to the exponent to restore the impied radix alignment. 
;	This explains why we always shift the smallest number.
;
;	For example, say we have two number 1 & 3 
;	N = BIAS_TR
;	Before alignment
;	1 = .10...	exp N
;	3 = .11...	exp N+1
;	After alignment
;	1 = .01...	exp N+1
;	3 = .11...	exp N+1
;
;	This alignment is required for addition and subtraction
;	
;
;	Case 1	If the exponents match return, nothing left to do.
;	Case 1a	If any exponent is zero, we are done
;
;	Case 2 If the exponents don't match, then:
;	if(FPac_EXP > FParg_EXP) {	/* Whose smallest */
;		F_SWAP();			/* Swap AC and ARG ! */
;	}
;	FP_bits = FParg_EXP - FPac_EXP;	/* Find the difference */
;	if(FP_bits < BITS_TR) {
;		AC32 >>= FP_bits;
;	}
;	else {
;		AC32 = 0;		/* Don't bother with big shifts */
;	}
;	FPac_EXP = FParg_EXP;	/* New exponent */
;================================================================
; START ALIGNMENT
;
_FP_ALIGN
	FPac_TST				
	JMP_NZ	_FP_align1
;	if(!FPac) FPac = FParg
; 		return FParg
	TRarg_STORE	TRac	; TRac = TRarg
	ENDSUB					; return TRac
;
_FP_align1
	FParg_TST
	JMP_NZ	_FP_align2
;	if(!FParg) 
; 		return FParg
	ENDSUB
;
; We now know that both TRac and TRarg are non zero
_FP_align2
	CMP8 FPac_EXP,FParg_EXP
	JMP_Z	_FP_align_X		; If same then nothing more to do
	JMP_LE	_FP_align3
	FPac_SWAP
_FP_align3
	MOV8	ML_EXP,FParg_EXP
	SUB8	ML_EXP,FPac_EXP
	CMP8I	ML_EXP,BITS_TR		; ML_EXP < BITS_TR ?
	JMP_L	_FP_align_ac; 		; Align TRac
; ML_EXP >= BITS_TR	
	AC32_CLR					; ML_EXP >= BITS_TR, AC32 = 0
	JMP	_FP_align_X
; Note ML_EXP <= BITS_TR so we only need the lower 8bits of ML_EXP
_FP_align_ac
	AC32_SH_LSB							; AC32 >>= ML_EXP, Align radix points
	DJNZ8	ML_EXP,_FP_align_ac
_FP_align_X
	MOV8	FPac_EXP , FParg_EXP
;
; DONE ALIGNMENT
; =======================================================

	CMP8	FPac_SIGN,FParg_SIGN; Check signs
	JMP_NZ	_FP_add_sub		
	;
_FP_add_add	; Signs are same 
	AC32_ADD_NORMAL	; AC32 += ARG32, normalize if carry
	JMP		_FP_add_x
	;
_FP_add_sub	; Signs differ, AC32 -= ARG32
	AC32_SUB			; AC32-ARG32
	JMP_GE	_FP_add_x	; AC32 => ARG32
	;
	AC32_NEG			; AC32 = -AC32
	XOR8I	FPac_SIGN,80h	; FPac_SIGN = -FPac_SIGN
	;
_FP_add_x	; Check sign
	TRarg_LOAD	FPtmp		; FParg = FPtmp, RESTORE FParg
	JMP		_FPac_NORM	; Make normal

;================================================================
;
;	Since we have only 8bits we can
;	guess at the overflow or under flow by
;	Compute (FPac_EXP + (FParg_EXP - BIAS_TR))
;
;	Biased number 40 - 3F = 01 NC 
;	Biased number 40 - 40 = 00 Z overflow
;	Biased number 40 - 41 = FF C overflow
;
;	Biased number C1 + 40 = 01 C overflow
;	Biased number C1 + 3F	= 00 C,Z overflow
;	Biased number 80 + 7F = FF
_FP_MUL:	; FPac *= FParg
	TST8	FPac_EXP				; Check for FPac == 0
	JMP_Z	_FPac_ZERO
	TST8	FParg_EXP
	JMP_Z	_FPac_ZERO				; Check for FParg == 0
	XOR8	FPac_SIGN,FParg_SIGN	; Sign of result

; New FPac_EXP = (FPac_EXP - BIAS_TR) + (FParg_EXP - BIAS_TR) - BIAS_TR
;     = FPac_EXP + FParg_EXP - BIAS_TR

    MOV8_16U    ML_EXP, FPac_EXP    ; unsigned
    ADD8_16U    ML_EXP, FParg_EXP   ; Add 
	SUB16I		ML_EXP, BIAS_TR		; restore BIAS
; Check for underflow <= 0
	JMP_L		_FPac_UNDER
; Check for overflow >= 256
	JMP_BSET	ML_EXP+1,0,_FPac_OVER
	MOV8		FPac_EXP,ML_EXP

;	FPac_EXP = FPac_EXP + (FParg_EXP - BIAS_TR)
_FP_mul_main
	AC32_STORE	TMP32			; FPtmp = FPac
	AC32_CLR
	LOAD8I	FP_bits,BITS_TR		; FP_bits = sizeof(mantissa)
_FP_mul3						; do { ... } while(--FP_bits);
	TMP32_SH_LSB				; TMP32 & 1 ?, Discard the bit we test
	JMP_NC		_FP_mul4		; Branch if Carry = 0
	AC32_ADD					; AC32+ARG32
;
;	If addition caused a carry it will be restored next
_FP_mul4
	AC32_SH_LSBC			; AC32>>=1
	DJNZ8	FP_bits,_FP_mul3	; while(--FP_bits)
;
;	Result, don't forget the post shift!
;
	JMP_BSET	AC32_MSB,7,_FP_mul5	; Was a Carry left in the MSB ?
	AC32_SH_MSB			; No, AC32<<1,align,EXP correct
	JMP		_FPac_NORM	

_FP_mul5	; Aligned, now correct EXP 
	INC8	FPac_EXP		; Allow for the Carry 
	JMP_Z	_FPac_OVER	; Check for overflow
	JMP		_FPac_NORM	

;================================================================
;
_FP_DIV:	; FPac /= FParg
	TST8	FPac_EXP
	JMP_Z	_FPac_ZERO	; 0 / FParg ?
	TST8	FParg_EXP
	JMP_Z	_FPac_OVER		; FPac / 0 ?
;
	XOR8	FPac_SIGN,FParg_SIGN; Sign of result


; New FPac_EXP = (FPac_EXP - BIAS_TR) + BIAS_TR - (FParg_EXP - BIAS_TR)
;              = FPac_EXP + BIAS_TR - FParg_EXP

    MOV8_16U    ML_EXP, FPac_EXP    ; unsigned
	ADD16I		ML_EXP, BIAS_TR		; restore BIAS
    SUB8_16U    ML_EXP, FParg_EXP   ; subtract
; Check for underflow <= 0
	JMP_L		_FPac_UNDER
; Check for overflow >= 256
	JMP_BSET	ML_EXP+1,0,_FPac_OVER
	MOV8		FPac_EXP,ML_EXP

	AC32_SH_LSB			; FPac >> 1,	Make room for a Carry bit
	ARG32_SH_LSB		; FParg >> 1, Realign ARG32 with AC32
	LOAD8I	FP_bits,BITS_TR	; Number of bits in result
_FP_div3	; do { ... } while(--FP_bits);
	TMP32_SH_MSB			; Make room for next bit
	AC32_CMP
	JMP_L _FP_div4	; AC32 < ARG32 ?
	AC32_SUB			; AC32 - ARG32
	BSET	TMP32_LSB,0		; Mark our success in TMP32
_FP_div4	; AC32 < AC32
	AC32_SH_MSB			; Discard the MSB we just tested
	DJNZ8	FP_bits,_FP_div3	; while(--FP_bits);
;
	ARG32_SH_MSB			; Restore ARG32
	AC32_LOAD	TMP32		; AC32 = result
	JMP	_FPac_NORM	; Make normal

;================================================================
;
;	Integer/Floating conversions
;	
;	TEMP REAL <EXP><SIGN><MANTISSA>
;	7..0 7..0  31..0
;
;	IEEE 32   <SIGN><EXP><MANTISSA>
;	31 30..23 22..0
;
;================================================================
_ITOF8:		; FPac = (INT8) AC32
	JMP_BCLR	AC32,7,_UTOF8
	AC32_NEG
	JMP		_NTOF8
	
_ITOF16:	; FPac = (INT16) AC32
	JMP_BCLR	AC32+1,7,_UTOF16
	AC32_NEG
	JMP		_NTOF16
	
_ITOF24:	; FPac = (INT24) AC32
	JMP_BCLR	AC32+2,7,_UTOF24
	AC32_NEG
	JMP		_NTOF24

_ITOF32:	; FPac = (INT32) AC32
	JMP_BCLR	AC32+3,7,_UTOF32
	AC32_NEG
	JMP		_NTOF32
	
	
_NTOF8:		; FPac = - (INT8) AC32
	CLR8	AC32+1
_NTOF16:	; FPac = - (INT16) AC32
	CLR8	AC32+2
_NTOF24:	; FPac = - (INT24) AC32
	CLR8	AC32+3
_NTOF32:	; FPac = - (INT32) AC32
	BSET	FPac_SIGN,7
	LOAD8I	FPac_EXP,BIAS_TR+BITS_TR-1	; Assign current radix
	JMP		_FPac_NORM	; Normalize
	

_UTOF8:		; FPac = (UINT8) AC32
	CLR8	AC32+1
_UTOF16:	; FPac = (UINT16) AC32
	CLR8	AC32+2
_UTOF24:	; FPac = (UINT24) AC32
	CLR8	AC32+3
_UTOF32:	; FPac = (UINT32) AC32
	CLR8	FPac_SIGN
	LOAD8I	FPac_EXP,BIAS_TR+BITS_TR-1	; Assign current radix
	JMP		_FPac_NORM	; Normalize


;================================================================
;	Floating point to unsigned 32bit int
_FP_U32:	; (UINT32) AC32 = FPac
	SUB8I	FPac_EXP,BIAS_TR	; FPac_EXP < BIAS_TR (1.0)?
	JMP_L	_AC32_CLR			; Yes, then AC32 = 0

	INC8	FPac_EXP			; FPac_EXP = FPac - BIAS_TR + 1

	CMP8I	FPac_EXP, BITS_TR	; > BITS_TR?
	JMP_G	_AC32_UMAX			; Yes, then AC32 = AC32_U32_MAX

; We have <= BITS_TR bits 
; Compute Fractional bits to shift out, Shift = BITS_TR - FPac_EXP
	NEG8	FPac_EXP
	ADD8I	FPac_EXP, BITS_TR
	JMP_Z	_FP_U32_X		; No shifts

_FP_U32_L
	AC32_SH_LSB	; AC32	>>= BITS;
	DJNZ8	FPac_EXP,_FP_U32_L

_FP_U32_X
	ENDSUB

;================================================================
;	Floating point to 32bit signed int
_FP_I32:	; (INT32) AC32 = FPac
	FP_U32
; Check MSB for overflow
	JMP_BSET	AC32_MSB,7,_AC32_UMAX
; No Overflow, Check SIgn
	JMP_BSET 	FPac_SIGN,7,_AC32_NEG
	ENDSUB
	;
; Overflow, Now check for +/- overflow
	JMP_BCLR	FPac_SIGN,7,_AC32_IMAX	; + IMIN
	JMP			_AC32_IMIN				; - IMAX

;================================================================
;FP_SPLIT
;
;	Example [ Numbers shown at TR format 32 bit mantissa ]
;	Source 1.5 = 0x7f,0, 0xC0 0x00 0x00 0x00
;
;	Interger 1.0 = 0x7f,0, 0x80 0x00 0x00 0x00
;	Interger 0.5 = 0x7e,0, 0x80 0x00 0x00 0x00
; ==================================================================

; FP_SPLIT()
;	SPlit up Integer and fractional parts of a FLOAT 
;	FPac == INT, FParg == Fraction
;
_FP_SPLIT:	; Split FPac: FPac=integer, FParg=fraction, (leave all floats)
	TRac_STORE 		FParg
	FPac_TST			; == 0 ?
	JMP_Z	_FP_SPLIT_X	; If so nothing left to do

; Find Radix point
; ML_cnt = FPac_EXP - BIAS_TR - 1
	MOV8	ML_cnt,FPac_EXP	
	SUB8I	ML_cnt,BIAS_TR	; Remove Bias
	INC8	ML_cnt
	JMP_Z	_FPac_ZERO 				; == 0? (All Fractional)
    JMP_BSET	ML_cnt,7,_FPac_ZERO ; < 0 ? (All fractional)
	;
; Part or none fractional
							; Test Number of intergal bits (ML_cnt) 
	CMP8I	ML_cnt,BITS_TR	; ML_cnt >= BITS_TR ? Then none fractional
	JMP_GE	_FParg_CLR		; If true then none fractional

	ARG32_CLR				; Clear fractional part to start with
;
; Some Fractional
; ML_cnt < BITS_TR Some integer and some fractional
; ML_cnt = Integer Bits to Keep
;
; Integer Part,    Keep the ML_cnt MSB bits of argument
; Fractional Part, Keep BITS_TR - ML_cnt bits or argument

; Compute new fractional EXP (ie less then the Integer bits to keep)
	SUB8	FParg_EXP,ML_cnt	; FParg_EXP -= ML_cnt

; Compute Fractional bits to discard = BITS_TR - ML_cnt
	NEG8	ML_cnt				; ML_cnt = BITS_TR - ML_cnt
    ADD8I   ML_cnt,BITS_TR

; Note we get here only if ML_cnt  != 0
;
    MOV8    ML_tmp,ML_cnt		; Save shift count
_FP_SPLIT_LOSE_LSB
    AC32_SH_LSB					; Discard Fractional bits
	ARG32_SH_LSBC				; Restore Fractional bits
    DJNZ8   ML_cnt,_FP_SPLIT_LOSE_LSB
    
_FP_SPLIT_LOSE_MSB
    AC32_SH_MSB					; Restore Integer. also renormalize
    DJNZ8   ML_tmp,_FP_SPLIT_LOSE_MSB

; Note - since FPac is nonzero (or we would net be here) it is normalized
; We have to normalize FParg
	FPac_SWAP
	FPac_NORM		; TODO takes 2 stack levels
	FPac_SWAP

_FP_SPLIT_X
    ENDSUB

; FP_FREXP()
;	Split fractional and exponent parts out of FPac
;	NOTE: 0.5 <= Fraction < 1.0 which is HALF the IEEE normal 
;	Return Exponent
;	
_FP_FREXP:	; FP_FREXP(FPac) W = exponent, 0.5<=FPac< 1.0,HALF the IEEE normal 
	TST8	FPac_EXP
	JMP_NZ	_FP_FREXP1
;	TODO call clear ?
	CLR8	ML_tmp
	JMP	_FP_FREXP_X
_FP_FREXP1
	MOV8	ML_tmp,FPac_EXP
	SUB8I	ML_tmp, BIAS_TR
	INC8	ML_tmp
	LOAD8I	FPac_EXP,BIAS_TR-1
_FP_FREXP_X
	movfw	ML_tmp
	ENDSUB

;	FPac = 1.0 / FPac
_FP_INVERT:	; FPac = 1.0 / FPac
	FParg_ONE
	FPac_SWAP
	JMP		_FP_DIV

;FP_LDEXP()
;	Add exponent to FPac, W = XEXP
_FP_LDEXP:	; FP_LDEXP(W)	FPac.EXP += W, Add exponent W to FPac
	movwf	ML_tmp
;	TODO call clear ?
	; FPac_TST
	JMP_Z	_FP_LDEXP_X

	JMP_BCLR	ML_tmp,7,_FP_LDEXP_P	; > 0
;	< 0
	NEG8	ML_tmp		
	SUB8	FPac_EXP,ML_tmp	; FPac_EXP - (-XEXP)
	JMP_L	_FPac_UNDER
	JMP	_FP_LDEXP_X

;	> 0
_FP_LDEXP_P	
	ADD8	FPac_EXP,ML_tmp	; FPac_EXP + XEXP
	JMP_C	_FPac_OVER
	JMP_Z	_FPac_OVER
_FP_LDEXP_X
	ENDSUB

;FP_MOD()
_FP_FMOD:	; FP_FMOD(FPac,FParg) FPac = Fraction(FPac/FParg)*FParg
	FP_DIV	;	FPac/FParg
; Save FParg
	TRarg_STORE	FPtmp
	FP_SPLIT		; FPac = Integer Part, FParg = Fractional Part
	FPac_SWAP		; FPac = Fractional Part
	TRarg_LOAD FPtmp
	JMP	_FP_MUL	; (((FPac/FParg) % 1.0) * FParg


;FP_INC()
;	Add ONE to FPac
;
_FP_INC_AC:	; FPac += 1.0
	FParg_ONE
	JMP	_FP_ADD

;FP_DEC()
;	Subtract ONE from FPac
;
_FP_DEC_AC:	; FPac -= 1.0
	FParg_ONE
	JMP	_FP_SUB

; FP_CEIL() 
;	CEIL returns the smallest integer greater than or equal to x.  
;	truncates toward plus infinity.
_FP_CEIL:	; FPac = CEIL(FPac)
	FP_SPLIT
	JMP_BSET	FPac_SIGN, 7, _FP_CEIL_X
	ARG32_TST	; Fractional part
	JMP_NZ	_FP_INC_AC
_FP_CEIL_X
	ENDSUB	


;FLOOR() 
;	FLOOR returns the largest integer less than or equal to x.
;	truncates toward minus infinity.
;
_FP_FLOOR:	; FPac = FLOOR(FPac)
	FP_SPLIT
	JMP_BCLR	FPac_SIGN, 7, _FP_FLOOR_X
	ARG32_TST	; Fractional part ?
	JMP_NZ	_FP_DEC_AC
_FP_FLOOR_X
	ENDSUB	

;================================================================
;
;
;	FPmul_10 Fast floating point multiply by 10
;
;	/* FPac_10 *= 10; */
;
;	Fpac_MAN >> 2;
;	AC32 *= 5;
;	FPac_EXP += 3;
;
;	
_FPac_MUL10:	; FPac *= 10  (fast!)
	TST8	FPac_EXP
	JMP_Z	_FPac_MUL10_X
	AC32_STORE	ARG32	; Divide by 4, Multiply by 5
	AC32_SH_LSB
	AC32_SH_LSB
	AC32_ADD_NORMAL	; FPac += FParg, normalize if carry
	JMP_Z	_FPac_OVER
	ADD8I FPac_EXP,3      ; Multiply by 8
	JMP_C	_FPac_OVER ; Overflow
_FPac_MUL10_X
	ENDSUB
	

;================================================================
_FPac_DIV10:	; FPac /= 10 (fast!)
	TST8	FPac_EXP
	JMP_Z	_FPac_DIV5_X
	DEC8	FPac_EXP	; Divide by 2
;__FP__AC_DIV5:	__FPac /= 5 
;
;	1/5= (1/4 - 1/16 +1/64 -1/256 ....)
_FPac_DIV5:	; FPac /= 5 (fast!)
	TST8	FPac_EXP
	JMP_Z	_FPac_DIV5_X
	LOAD8I	FP_bits,8
	AC32_STORE	ARG32
	AC32_CLR
_FPac_DIV5_LOOP
;	AC += 1/4
	ARG32_SH_LSB ; Make room for extra Carry bit
	ARG32_SH_LSB ; Make room for extra Carry bit
	AC32_ADD
;	AC -= 1/16
	ARG32_SH_LSB ; Make room for extra Carry bit
	ARG32_SH_LSB ; Make room for extra Carry bit
	AC32_SUB
	DJNZ8	FP_bits,_FPac_DIV5_LOOP
_FPac_DIV5_X
	FPac_NORM			; Make normal
	ENDSUB


;================================================================
;	ENDIF FP_ASM	
#endif
