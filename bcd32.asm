;File:	bcd32.asm
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
#ifndef	BCD32_ASM 
#define	BCD32_ASM

;	BCD32
;	Convert AC32 to a packed 10 digit BCD RESULT
;
;	USES FPac and FPtmp
;

;	BCD32

;	Overlay AC32 and TMP32
;	4+5+1+1+1 = 12 bytes
;	Memory Layout:
;	AC32
;	BCD_BUF[0]	FPac_EXP
;	BCD_BUF[1]	FPac_SIGN
;	BCD_BUF[2]	FPtmp+0
;	BCD_BUF[3]	FPtmp+1
;	BCD_BUF[4]	FPtmp+2
;	BCD_CNT FPtmp+3
;	BCD_CNT2 FPtmp_EXP
;	BCD_DIG FPtmp_SIGN

BCD_BUF	equ	AC32+4	;	All 6 bytes = 12 BCD digits
BCD_CNT	equ BCD_BUF+5	; Loop counter
BCD_CNT2	equ	BCD_CNT+1	; Loop Counter2
BCD_DIG	equ BCD_CNT2+1

;	BCD_CONTROL flags

BCD_SIGN	equ	7	; 1 = Negative
BCD_LZEROS	equ	6	; 1 = supress leading ZEROS
BCD_PLUS	equ	5	; 1 = Display + when positive

;	Notes:
;	Convert binary to packed bcd 1 bit at a time
;
;	The method calculates the decimal adjusted result 1 bit at
;	a time by shifting the binary value into the LSB of the bcd
;	result and the decimal adjusting the remaining bcd digits.
;
;
;	We move the bcd adjustment to the top of the loop to
;	just avoid an extra loop test. 
;	You may move the entire bcd adjust routine to the end of the loop 
;	and test for loop completion prior to the bcd adjust function
;	with no other changes.



	
_BCD32:		; Convert Signed AC32 into packed ACSII stored in BCD_BUF - signed
	CLR8	BCD_STATUS
	JMP_BCLR	AC32_MSB,7,_BCD32_MAIN
	BSET	BCD_STATUS,BCD_SIGN
	AC32_NEG
	JMP	_BCD32_MAIN

; BCD32U()	Convert AC32 into packed ACSII stored in BCD_BUF - unsigned
_BCD32U:	; Convert Unsigned AC32 into packed ACSII stored in BCD_BUF - signed
	CLR8	BCD_STATUS

_BCD32_MAIN
	LOAD8I	BCD_CNT,32	; 32 bits
	IN_CLR	BCD_BUF,5	; BCD buffer is 5 bytes, 10 digits

;	Outer binary shift Loop is 32 times, 32 bits
_BCD32_LOOP
;	Inner BCD adjust loop 
;	10 digits
;
	LOAD8I	FSR,BCD_BUF
	LOAD8I	BCD_CNT2,5

_BCD32_ADJ
	ADD8I	INDF,0x03	; [FSR] += 0x03
	JMP_BSET	INDF,3,_BCD32_ADJ_LOW
	SUB8I	INDF,0x03	; Restore
_BCD32_ADJ_LOW
	ADD8I	INDF,0x30	; [FSR] += 0x30
	JMP_BSET	INDF,7,_BCD32_ADJ_HI
	SUB8I	INDF,0x30	; Restore
_BCD32_ADJ_HI

	INC8	FSR
	DJNZ8	BCD_CNT2,_BCD32_ADJ

;	SHIFT AC32 into BCD_BUF 4 + 5 bytes
;
	LOAD8I	FSR,AC32
	LOAD8I	BCD_CNT2,9
	CLRC		; Clear Carry
_BCD32_SHIFT
	RL8_MSB	INDF	
	INC8	FSR
	DJNZ8	BCD_CNT2,_BCD32_SHIFT	

	DJNZ8	BCD_CNT, _BCD32_LOOP
_BCD32_EXIT
	CLR8	BCD_CNT2	; Used as offset for decimal PT
	ENDSUB



;	===================================================================
_BCD32_PUTS:	; Display BCD result from packed ACSII BCD_BUF
	LOAD8I	BCD_CNT,9	; 9 .. 0, 10 digits
;	Entry for less then 10 digits
_BCD32_PUTS_CNT	; Output less then 10 digits
	JMP_BCLR	BCD_STATUS,BCD_SIGN,_BCD32_PUTS_P
	; Minus
	PUTCI	'-'
	JMP	_BCD32_PUTS_LOOP

_BCD32_PUTS_P
	; Plus ?
	JMP_BCLR	BCD_STATUS,BCD_PLUS,_BCD32_PUTS_LOOP
	PUTCI	'+'

;	Main
_BCD32_PUTS_LOOP

;	Display BCD Digit
;	BCD32_DIG: Get a nibble from the BCD BUFFER
_BCD32_DIGIT:	; Display one BCD digit from packed ACSII BCD_BUF
	MOV8	BCD_DIG,BCD_CNT		; Compute index into BCD_BUF
	SH8_LSB	BCD_DIG			; BCD_BUF + (BCD_CNT >> 1)
	ADD8I	BCD_DIG,BCD_BUF
	MOV8	FSR,BCD_DIG
	movf	INDF,w 		; [FSR] Lower nibble into lower W
	JMP_BCLR	BCD_CNT,0,_BCD32_DIGIT_LOW	; Test for HIGH/LOW BYTE
_BCD32_DIGIT_HI	; Upper Nibble
	swapf INDF,w      		; Upper nibble into lower W
_BCD32_DIGIT_LOW
	andlw 0x0f
	movwf	BCD_DIG
	; Dsiplay leading ZEROs ?
	JMP_BCLR BCD_STATUS,BCD_LZEROS,_BCD_PUTC_C 
	; Skip Leading Zeros!
	JMP_Z	_BCD_PUTC_DEC
	BCLR	BCD_STATUS,BCD_LZEROS	; NON XERO, Turn off skipping
;	Display digit
_BCD_PUTC_C
	ADD8I	BCD_DIG,0x30
	PUTC	BCD_DIG
_BCD_PUTC_DEC
	DEC8	BCD_CNT
	JMP_BCLR	BCD_CNT,7,_BCD32_PUTS_LOOP	; < 0 ?

;	If the whole number was zero then display one zero!
;
;
;	If we saw NO ZEROS while skiping leading zeros, then display one '0'

	JMP_BCLR BCD_STATUS, BCD_LZEROS, _BCD32_PUTS_X

	PUTCI '0'

_BCD32_PUTS_X
	ENDSUB

;	=========================================================================
;	ENDIF BCD32_ASM
#endif
