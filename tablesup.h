;File:	tablesup.h
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
#ifndef TABLESUP_H
#define TABLESUP_H

#include "macro.h"

; Next conditional is to avoid a bug in condition macro processing
#ifndef TABLE_H
#include "table.h"
#endif

; Table load TRac FP with temporary real 40bit
TRac_TABLE_LD	MACRO const
	TABLE_SET_SOURCE	const
	CALL _TRac_TABLE_LD
	ENDM

; Table load TRarg FP with temporary real 40bit
TRarg_TABLE_LD	MACRO const
	TABLE_SET_SOURCE	const
	CALL _TRarg_TABLE_LD
	ENDM

; Table load FPac with 32bit IEEE float
FPac_TABLE_LD	MACRO const
	TABLE_SET_SOURCE	const
	CALL _FPac_TABLE_LD
	ENDM

; Table load FParg with 32bit IEEE float
FParg_TABLE_LD	MACRO const
	TABLE_SET_SOURCE	const
	CALL _FParg_TABLE_LD
	ENDM

; Table load AC with 32bit number
AC32_TABLE_LD	MACRO	const
	TABLE_SET_SOURCE	const
	CALL	_AC32_TABLE_LD
	ENDM

; Table load ARG with 32bit number
ARG32_TABLE_LD	MACRO	const
	TABLE_SET_SOURCE	const
	CALL	_ARG32_TABLE_LD
	ENDM

; Set table target TRac and size for 40bit temprary real
TABLE_SET_TRac MACRO
	CALL	_TABLE_SET_TRac
	ENDM

; Set table target TRarg and size for 40bit temprary real
TABLE_SET_TRarg MACRO
	CALL	_TABLE_SET_TRarg
	ENDM

; Set table target AC32 and size for 32bit 
TABLE_SET_AC32 MACRO 
	CALL	_TABLE_SET_AC32
	ENDM

; Set table target ARG32 and size for 32bit temprary 
TABLE_SET_ARG32 MACRO 
	CALL	_TABLE_SET_ARG32
	ENDM

; Set table target FP AC and size for 40bit temprary real
_TABLE_SET_TRac:
	TABLE_SET_TARGET	FPac
	TABLE_SET_SIZE		6
	ENDSUB

; Set table target FP ARG and size for 40bit temprary real
_TABLE_SET_TRarg:
	TABLE_SET_TARGET	FParg
	TABLE_SET_SIZE		6
	ENDSUB

; Set table target AC and size for 32bit number
_TABLE_SET_AC32:
	TABLE_SET_TARGET	AC32
	TABLE_SET_SIZE		4
	ENDSUB

; Set table target ARG and size for 32bit number
_TABLE_SET_ARG32:
	TABLE_SET_TARGET	ARG32
	TABLE_SET_SIZE		4
	ENDSUB

; Load FP AC with temporary real 40bit - assumes table pointer is set
_TRac_TABLE_LD:
	CALL	_TABLE_SET_TRac
	JMP		_TABLE_LD

; Load FP ARG with temporary real 40bit - assumes table pointer is set
_TRarg_TABLE_LD:
	CALL	_TABLE_SET_TRarg
	JMP		_TABLE_LD

; Load FP AC with 32bit IEEE - assumes table pointer is set
_FPac_TABLE_LD:
	CALL	_TABLE_SET_AC32
	CALL	_TABLE_LD
	JMP		_IEEE_TO_TRac

; Load FP ARG with 32bit IEEE - assumes table pointer is set
_FParg_TABLE_LD:
	CALL	_TABLE_SET_ARG32
	CALL	_TABLE_LD
	JMP		_IEEE_TO_TRarg

; Load AC as 32bit number - assumes table pointer is set
_AC32_TABLE_LD:
	CALL	_TABLE_SET_AC32
	JMP		_TABLE_LD

; Load ARG as 32bit number - assumes table pointer is set
_ARG32_TABLE_LD:
	CALL	_TABLE_SET_ARG32
	JMP		_TABLE_LD

#endif
