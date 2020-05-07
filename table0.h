;File:	table0.h
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
#ifndef	TABLE0_H
#define	TABLE0_H

;	INCLUDE "macro.h"
;	Depends on TABLE.H table_start and table_end
; Next conditional is to avoid a bug in condition macro processing
#ifndef TABLE_H
	INCLUDE "table.h"
#endif

;	===================================================================
; MAKE TABLE0
    TABLE_MAKE_SUP  _TABLE0_MAIN,table0_start,table0_end


; ================================================================
; TABLE0_LD - Load Memory from table0
TABLE0_LD	MACRO	target
	TABLE_LD_SUP	_TABLE0_MAIN,target
	ENDM

;	===================================================================
;	AC32/ARG32 load


;	Use Jump Table build in table0.h

AC32_TABLE0_LD	MACRO const
	TABLE_SET_AC32
	TABLE0_LD	const
	ENDM
	
ARG32_TABLE0_LD	MACRO	const
	TABLE_SET_ARG32
	TABLE0_LD	const
	ENDM

;	===================================================================
;	TR AC/ARG load


;	FPac_TR = ROM
TRac_TABLE0_LD	MACRO const
	TABLE_SET_TRac
	TABLE0_LD	const
	ENDM
	
;	FParg_TR = ROM
TRarg_TABLE0_LD	MACRO	const
	TABLE_SET_TRarg
	TABLE0_LD	const
	ENDM

;	===================================================================
;	FP IEEE AC/ARG
;	FPac_TR = IEEE32_TO_TR(AC32)
FPac_TABLE0_LD	MACRO const
	TABLE_SET_AC32
	TABLE0_LD	const
	IEEE_TO_TRac
	ENDM
	
;	FParg = IEEE32_TO_TR(ARG32)
FParg_TABLE0_LD	MACRO	const
	TABLE_SET_ARG32
	TABLE0_LD	const	
	IEEE_TO_TRarg
	ENDM

;	===================================================================
;	===================================================================
;	IFDEF TABLE0_H
#endif
