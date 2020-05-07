;File:	io.h
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
#ifndef	IO_H
#define	IO_H

; Set a user defined output FUNCTION
SET_PUTC	MACRO	ADDRESS
	LOAD8I	_PUTC_FN+1,ADDRESS>>8
	LOAD8I	_PUTC_FN,ADDRESS & 0xff
	ENDM

; Set a user defined input FUNCTION
SET_GETC	MACRO	ADDRESS
	LOAD8I	_GETC_FN+1,ADDRESS>>8
	LOAD8I	_GETC_FN,ADDRESS & 0xff
	ENDM

; Set I/O pointers to defaults states priot to user setup
INIT_IO	MACRO
	call	_INIT_IO
	ENDM

;	=========================================================================
;	Debugger macros
GETC MACRO
	CALL	_GETC
; Set Z flag if W is zero because RETLW 0 does not se the flag!
	ENDM

UNGETC MACRO	ARG
	movfw	ARG
	CALL	_UNGETC
	ENDM

UNGETC_RESET MACRO
	CALL	_UNGETC_RESET
	ENDM

PUTCI	MACRO	ARG
	movlw	ARG
	CALL	_PUTC
	ENDM

PUTC	MACRO	ARG
	movfw	ARG
	CALL	_PUTC
	ENDM

PUTS	MACRO	STR
	local	next,_str
	JMP		next
_str
	dt		STR
	dt		0
next
	LOAD16I	_PUTS_PTR,_str
	CALL	_PUTS
	ENDM
PUT_NL	MACRO
	movlw	'\n'
	CALL	_PUTC
	ENDM

PUTCW	MACRO
	CALL	_PUTC
	ENDM

DB_PUTCI	MACRO ARG
	movlw	ARG
	CALL	_DEBUG_PUTCW
	ENDM

DB_PUTCW	MACRO
	CALL	_DEBUG_PUTCW
	ENDM
;	=====================================================================

#endif
