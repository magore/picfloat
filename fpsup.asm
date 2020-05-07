;File:	fpsup.asm
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
#ifndef	FPSUP_ASM
#define	FPSUP_ASM

;	Note: n = BITS_TR keeps all bits
_FP_KEEP_MSB:	;	Keep the N MSB bits of argument, W = n
	movwf	ML_tmp
	LOAD8I	ML_cnt,BITS_TR
	SUB8	ML_cnt,ML_tmp	; ML_cnt = BITS_TR - n
	JMP_LE	_FP_KEEP_MSB_X

	MOV8	ML_tmp,ML_cnt

	
_FP_KEEP_MSB_LOOP1	
	AC32_SH_LSB
	DJNZ8	ML_cnt,_FP_KEEP_MSB_LOOP1

_FP_KEEP_MSB_LOOP2
	AC32_SH_MSB
	DJNZ8	ML_tmp,_FP_KEEP_MSB_LOOP2
_FP_KEEP_MSB_X
	ENDSUB
	
;	Note: n = BITS_TR resets all bits
_FP_RESET_MSB:	; Reset the N MSB bits of argument , W = n
	movwf	ML_tmp
	MOV8	ML_cnt,ML_tmp
	JMP_Z	_FP_RESET_MSB_X
	
_FP_RESET_MSB_LOOP1	
	AC32_SH_MSB
	DJNZ8	ML_cnt,_FP_RESET_MSB_LOOP1

_FP_RESET_MSB_LOOP2
	AC32_SH_LSB
	DJNZ8	ML_tmp,_FP_RESET_MSB_LOOP2
_FP_RESET_MSB_X
	ENDSUB


;	Note: n = BITS_TR keeps all bits
_FP_KEEP_LSB:	; Keep the N LSB bits of argument, W = N
	movwf	ML_tmp		; ML_cnt = BITS_TR - n
	LOAD8I	ML_cnt,BITS_TR
	SUB8	ML_cnt,ML_tmp	; W = ML_cnt
	JMP	_FP_RESET_MSB

;
;	Note: n = BITS_TR resets all bits
;
_FP_RESET_LSB:	; Reset the N LSB bits of argument, W = N
	movwf	ML_tmp		; ML_cnt = BITS_TR - n
	LOAD8I	ML_cnt,BITS_TR
	SUB8	ML_cnt,ML_tmp	; W = ML_cnt
	JMP	_FP_KEEP_MSB

;FP_MKINT(W)  
;
;	W = XEXP
;	Reset all bits in AC32 < XEXP.
;	Keep all bits in AC32 >= XEXP.
;
;	Example [ Numbers shown at TR format 31 bit mantissa ]
;	Source 1.5 = 0x7f,0, 0x60 0x00 0x00 0x00
;	if XEXP = 0 then
;	Result 1.0 = 0x7f,0, 0x40 0x00 0x00 0x00
;	Same as FPac = (float) (int) FPc
;	
_FP_MKINT:	; Reset the fraction bits in FPac less or equal to radix set by W
;	MSB BITS to keep = (FPac.EXP - BIAS_TR - xexp) + 1)

	movwf	ML_tmp		; XEXP = W

	FPac_TST
	JMP_Z	_FP_MKINT_X

	MOV8	ML_cnt,FPac_EXP	; ML_cnt = FPac_EXP - BIAS_TR - XEXP + 1
	SUB8I	ML_cnt,BIAS_TR; ; Remove Bias
	SUB8	ML_cnt,ML_tmp	
	INC8	ML_cnt

	JMP_Z	_FPac_CLR	; <= 0 ?
	JMP_BSET ML_cnt,7,_FPac_CLR

	movfw	ML_cnt
	JMP	_FP_KEEP_MSB
_FP_MKINT_X
	ENDSUB

;	
;	_FP_MKFRAC
;	Reset all bits in AC32  > XEXP
;	Keep all bits in AC32  <= XEXP
;	Example [ Numbers shown at TR format 31 bit mantissa ]
;	Source 1.5 = 0x7f,0, 0x60 0x00 0x00 0x00
;	if XEXP = 0 then
;	Result 0.5 = 0x7e,0, 0x40 0x00 0x00 0x00
;	
;	
;	W = XEXP
_FP_MKFRAC:	; Keep the fraction bits in FPac less or equal to radix set by W
;	MSB BITS to reset = (FPac.EXP - BIAS_TR - XEXP)+1
	movwf	ML_tmp		; XEXP = W

	FPac_TST			; 0 ?
	JMP_Z	_FP_MKFRAC_X

	MOV8	ML_cnt,FPac_EXP	; ML_cnt = FPac_EXP - BIAS_TR - XEXP + 1
	SUB8I	ML_cnt,BIAS_TR; ; Remove Bias
	SUB8	ML_cnt,ML_tmp	
	INC8	ML_cnt

	JMP_BSET	ML_cnt,7,_FP_MKFRAC_X ; < 0 ?

	movfw	ML_cnt
	FP_RESET_MSB
	JMP	_FPac_NORM
_FP_MKFRAC_X
	ENDSUB

; ==================================================================
; Example SPLIT function using FP_MKFRAC and FP_MKINT
; See FP.ASM for builtin SPLIT
; FP_SPLIT()
;	SPlit up Integer and fractional parts of a FLOAT
;	FPac == INT, FParg == Fraction
;
; FP_SPLIT(FPac)	FPac = integer part, FParg = fractional part
;_FP_SPLIT
;	FPac_TST
;	TRac_STORE	FParg	; FParg == FPac, Save
;	movlw	0
;	FP_MKFRAC
;	FPac_SWAP	
;	movlw	0
;	JMP	_FP_MKINT

#endif
