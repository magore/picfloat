;File:	mem.h
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
#ifndef	MEM_H
#define	MEM_H

;	INCLUDE "macro.h"
;	===========================================================
;	Memory move vars
	cblock
		mem_from, mem_to, mem_count, mem_tmp:2
	endc


;	===========================================================
;	MULTI BYTE Moves using an index register for both source and target
;
;	Warning INDEX register (FSR) is over written!
;
;	Notes:
;	

;	===========================================================
IN_MOV_MAC	MACRO	TO,FROM,count
	LOAD8I	mem_to,TO
	LOAD8I	mem_from,FROM
	movlw	count
	CALL	MEM_MOV
	ENDM

	
	
;	General Memory Move Subroutine
;	W = number of bytes to move
;
MEM_MOV4
	movlw	4
	JMP	MEM_MOV

MEM_MOV6
	movlw	6
	JMP	MEM_MOV

MEM_MOV
	movwf	mem_count		; Number of Bytes to Move
MEM_MOV_LOOP				;	[mem_to++] = [mem_from++]
	MOV8	FSR,mem_from	; FSR = mem_from++
	INC8	mem_from
	MOV8	mem_tmp,INDF	; mem_tmp = [FSR]
	MOV8	FSR,mem_to		; FSR = mem_to++
	INC8	mem_to
	MOV8	INDF,mem_tmp	; [FSR] = mem_tmp
	DJNZ8	mem_count,MEM_MOV_LOOP
	ENDSUB

;	======================================================
#endif
