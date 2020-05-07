;File:	atoi.asm
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

_SKIP_SPACES:	; Skip white space in input
    GETC
; Eat white space
    CMP8I   _GETC_CHAR,' '
    JMP_Z   _SKIP_SPACES
    CMP8I   _GETC_CHAR,'\t'
    JMP_Z   _SKIP_SPACES
	UNGETC	_GETC_CHAR
	ENDSUB

_GET_SIGN:	; Get character and test +/- sign, set _atoi_sign if negative
	CLR8	_atoi_sign
    GETC
; +/- ?
    CMP8I   _GETC_CHAR,'+'
    JMP_Z   _GET_SIGN_PLUS
    CMP8I   _GETC_CHAR,'-'
    JMP_Z	_GET_SIGN_NEG
; Done
	UNGETC	_GETC_CHAR			; Put Character back
_GET_SIGN_PLUS
	ENDSUB
; -ve
_GET_SIGN_NEG
	LOAD8I	_atoi_sign,0x80
; +ve
	ENDSUB

; ======================================================================
_TEST_DIG:	; Is _GETC_CHAR >='0' && <='9' ? Set Carry Clear, else Set Carry
	CMP8I	_GETC_CHAR,'0'
	JMP_L	_TEST_DIG_exit; < '0'
	CMP8I	_GETC_CHAR,'9'
	JMP_G	_TEST_DIG_exit; > '9'
	SUB8I	_GETC_CHAR,'0'
	CLRC
	ENDSUB
_TEST_DIG_exit:
	SETC
	ENDSUB
; ======================================================================

; Uses AC32,ARG32 and TMP32
_ATOI:		; Convert ACSII to integer, Store result in AC32
	CLR8	_atoi_count
	SKIP_SPACES
	GET_SIGN
	AC32_CLR
_atoi_loop
	GETC
	TEST_DIG
	JMP_C		_atoi_done
	AC32_MUL10
	ARG32_CLR
	MOV8	ARG32_LSB,_GETC_CHAR
	AC32_ADD
	INC8		_atoi_count
	JMP			_atoi_loop
	;
_atoi_done
	UNGETC	_GETC_CHAR
	TST8	_atoi_sign
	JMP_Z	_atoi_exit
	AC32_NEG
_atoi_exit
	ENDSUB
