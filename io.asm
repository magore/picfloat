;File:	io.asm
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
#ifndef	IO_ASM
#define	IO_ASM
;	INCLUDE "io.h"


; Address pointer for getc
	cblock 
		_PUTC_FN:2,_GETC_FN:2
		_PUTS_PTR:2
		_PUTC_CHAR
		_GETC_CHAR
		_UNGETC_CHAR
		_IO_FLAGS
	endc
; PUTC
;	Jump to a user defined output FUNCTION
; 	Assumes we have been called
_PUTC:	; Put character _PUTC_CHAR to output 
	movwf		_PUTC_CHAR			; Save character to send
	MOV8    	PCLATH,_PUTC_FN+1	; Jump to user defined PUTC
	MOV8    	PCL,_PUTC_FN
	; Target is _PUTC Functions
	; ENDSUB implied


_GETC_BYTE:	; Get character from input - raw
	MOV8    	PCLATH,_GETC_FN+1
	MOV8    	PCL,_GETC_FN
	; Target is _GETC Functions
	; ENDSUB implied


_GETC:	; Get character from input and save in _GETC_BYTE
	JMP_BCLR	_IO_FLAGS,0,_GETC_CH	; Is there an unget character ?
	movfw		_UNGETC_CHAR			; Yes - then return it
	BCLR		_IO_FLAGS,0				; Reset the unget flag
	ENDSUB
_GETC_CH								; Jump to user defined GETC
	CALL		_GETC_BYTE 				; Get character
	movwf		_GETC_CHAR				; Save result
	iorlw		0						; Set status - retlw does NOT set it!
	ENDSUB


_UNGETC:	; Unget character, Save W regsiter in _UNGETC_CHAR
		movwf	_UNGETC_CHAR			; Load Unget Character with W
		BSET	_IO_FLAGS,0
		ENDSUB

_UNGETC_RESET:	; Purge UNGETC character, see _UNGETC
		BCLR	_IO_FLAGS,0
		ENDSUB


;   ======================================================
_PUTS:	; Put string pointed to by _PUTS_PTR
_PUTS_LOOP:
		CALL    _PUTS_GETC  ; W = [PUTS_PTR]
		iorlw	0	
		JMP_Z	_PUTS_EXIT
		CALL	_PUTC
		INC16   _PUTS_PTR		 ; _PUT_PTR++
		JMP		_PUTS_LOOP
_PUTS_EXIT
		ENDSUB
;   W = [PUTS_PTR]
;
_PUTS_GETC:	; Table load byte into W pointed to by _PUTS_PTR
    MOV8    PCLATH,_PUTS_PTR+1
    MOV8    PCL,_PUTS_PTR
    ; target is RETW
    ; ENDSUB implied

_IO_DUMMY:	; Dummy I/O function that does nothing
	ENDSUB

_DEBUG_PUTC:	; Output Charater trap used with PSIM simulator debugging
	movfw	_PUTC_CHAR					; Load W with Character
_DEBUG_PUTCW:	; Output W, Charater trap used with PSIM simulator debugging
	dw		0x000b
	ENDSUB

_INIT_IO:	; Set default I/O functions
	CLR8		_IO_FLAGS
	SET_PUTC	_DEBUG_PUTC
	SET_GETC	_IO_DUMMY
	ENDSUB


#endif

