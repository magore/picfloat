;File:	atof.asm
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

;================================================================
_ATOF:		; Convert ASCII to floating point, Store result on FPac
	FPac_CLR
	CLR8	_atof_pow10
	CLR8	_atof_radix
	SKIP_SPACES
	GET_SIGN
_atof_loop_main
	GETC
	JMP_BSET	_atof_radix,0,_atof_digit ; Have already seen decimal point 
	CMP8I	_GETC_CHAR,'.'		; Decimal point ?
	JMP_NZ	_atof_digit			; No
	BSET	_atof_radix,0		; Yes
	JMP		_atof_loop_main		; Get another character
;
_atof_digit
	TEST_DIG
	JMP_C	_atof_numx
; FPac *= 10
	FPac_MUL10
; Fpac += _GETC_CHAR
	FPac_SWAP
	AC32_CLR		; Convert _GETC_CHAR to float
	MOV8	AC32_LSB,_GETC_CHAR
	ITOF32
	FPac_SWAP		; FParg = (float) _GETC_CHAR
	FP_ADD			; FPac += (float) _GETC_CHAR

    JMP_BCLR	_atof_radix,0,_atof_loop_main
	DEC8	_atof_pow10	; Scale the exponent for digits left of decimal point
	JMP		_atof_loop_main
;
_atof_numx
	MOV8	FPac_SIGN,_atoi_sign	; Restore sign
	TRac_STORE	FPtmp				; Save Floating point number and sign

; Parse Exponent
	CMP8I	_GETC_CHAR,'E'
	JMP_Z	_atof_have_exp
	CMP8I	_GETC_CHAR,'e'
	JMP_Z	_atof_have_exp
	UNGETC	_GETC_CHAR				; Put character back
	JMP		_atof_scale				; No exponent

_atof_have_exp
	ATOI
; TODO Check overflow
	ADD8	_atof_pow10,AC32_LSB
;
; Compute exponent base 10 to scale the fp number by
; Notes: 
;		If the exponent is >= 0 then we just keep multiplying the
;		fp number by 10 exponent count times (using fast multiply).
;		- otherwise -
;		keep dividing by 10 exponent count times ( using fast divide)
;
; 		_atof_pow10 has total powers of TEN
;
_atof_scale
	TRac_LOAD	FPtmp				; Restore Floating point number
	TST8	_atof_pow10
	JMP_Z	_atof_exit
	JMP_BCLR _atof_pow10,7,_atof_exp_mul10	; >= 0 ?

_atof_exp_div10
	FPac_DIV10						; FPac /= 10
	INC8	_atof_pow10
	JMP_NZ	_atof_exp_div10
	ENDSUB

_atof_exp_mul10
	FPac_MUL10						; FPac *= 10
	DEC8	_atof_pow10
	JMP_NZ	_atof_exp_mul10
_atof_exit
	ENDSUB
