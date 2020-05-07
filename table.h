;File:	table.h
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
;
#ifndef	TABLE_H
#define	TABLE_H

;	INCLUDE "macro.h"
;	INCLUDE "mem.h"



;	======================================================
;
;	TABLE MACROS and functions: Load memory from CONSTANT ROM
;
;	Reserved RAM usage:
;
;	table_ptr					= pointer into CONSTANT ROM
;	table_size					= size reload
;	table_to					= target address in ram, is not modified
;	FSR 						= current target address in ram
;	
; ==================================================================
;	Memory used for table memeory access functions
	cblock
		table_ptr:2, table_size, table_to
	endc
; ==================================================================

; Set table target address (ram)  - not modified after transfer
TABLE_SET_TARGET	MACRO arg
	LOAD8I	table_to,arg
	ENDM

; Set table source address (rom)  - points past table after transfer
TABLE_SET_SOURCE	MACRO	const
	LOAD16I	table_ptr,const
	ENDM

; Set table size (bytes to transfer) - not modified after transfer
TABLE_SET_SIZE	MACRO arg
	LOAD8I	table_size,arg
	ENDM

; Table transfer using previously set target,source and size - target modified
TABLE_LD	MACRO
	CALL	_TABLE_LD
	ENDM

; Table transfer to W, W = rom[table_ptr]
TABLE_LD_MEM	MACRO TO
	CALL	_TABLE_LDW
	movwf 	TO
	ENDM

; ======================================================
;	TABLE_LD
;	Transfer Constant Data to RAM
;
;	table_ptr	= pointer into CONSTANT ROM
;	table_size	= size reload
;	table_to	= target address in ram, is not modified
;	FSR = current target address in ram
; ======================================================
; Table load - assumes target,source and size have been set - uses INDF
_TABLE_LD:
	MOV8 FSR,table_to
	MOV8 mem_count,table_size
_TABLE_LD_loop
	CALL	_TABLE_LD_byte	; W = [table_ptr]
	movwf	INDF		; [FSR] = W
	INC8	FSR		; FSR++
	INC16	table_ptr	; table_ptr++
	DJNZ8	mem_count,_TABLE_LD_loop
	ENDSUB

; Load W with table pointer
_TABLE_LDW:
	CALL	_TABLE_LD_byte	; W = [table_ptr]
	INC16	table_ptr	; table_ptr++
	ENDSUB


;	======================================================
;	TABLE_LD_byte
;	return the byte pointed to by table_ptr
;
;	W = [table_ptr]
;
; Table load byte into W - assumes source has been set - uses INDF
_TABLE_LD_byte:
	MOV8	PCLATH,table_ptr+1
	MOV8	PCL,table_ptr
	; target is RETW
	; ENDSUB implied

; ========================================================================
; We want to use _TABLE_LD, as defined above, to load arrays of constants
; bytes of ROM - but we want to avoid the high setup cost when the table 
; is bigger then 256bytes
;
; Trick; We can use call address itself to encode part of the address
; 	- but we need an extra jump table to do the extra work.
; 	(very good idea if the tables wll be used often)
; How ?
; Notes:
;	BASE is the absolute address for the start of the array table data
;	ADDR is the absolute address of some data in the array
;	_TABLE_MAIN - extra jump table to encode the high byte of the address
;
; Here is how we might encode our call
;	movfw	LOW(ADDR)
;	call	_TABLE_MAIN#v(HIGH(ADDR-BASE))
; 
; Lets implement an actuall table for the call above
; Notes:
;	W has LOW byte of absolute data address
; 	#v() inderectly encodes the high byte of the absolute data address
;	table_ptr is 16bit address pointer for table load function
; Assumes:
;	table_to has address of ram to load - must be set elsewhere
;	table_size has size of ram to load - must be set elsewhere
;
; _TABLE_MAIN0
;	movwf	table_ptr				; W has low byte of absolute data address
;	LOAD8I	table_ptr+1,HIGH(BASE) 		; high byte of absolute data address
; 	JMP	_TABLE_LD	; Load the data
; _TABLE_MAIN1
;	movwf	table_ptr				; W has low byte of absolute data address
;	LOAD8I	table_ptr+1,HIGH(BASE+256) 		; high byte of absolute data address
; 	JMP	_TABLE_LD	; Load the data
; ... etc
;
; ============================================================================

; ============================================================================

; TABLE_MAKE - build suplimentary jump table for tables larger then 256bytes
TABLE_MAKE_SUP	MACRO lable, tab_start, tab_end
	local pointer=tab_start
	local addr
	WHILE pointer <= tab_end
addr=pointer >> 8
lable#v(addr)
		movwf	table_ptr ; W = low byte of absolute data address
		LOAD8I	table_ptr+1,addr ; high byte of absolute data address
		JMP	_TABLE_LD
pointer=pointer+256
	ENDW
	ENDM

; ================================================================
; TABLE_LD_SUP - Load Memory from table accessed with TABLE_MAKE_SUP
; Assumes table_size and table_to is set - prior to call
; Notes: table_size and table_to are not changed after call
;
; TABLE_LD_SUP - Load Memory from table accessed with TABLE_MAKE_SUP
TABLE_LD_SUP   MACRO   lable,target
; LOW Byte of Address
	movlw	LOW(target)
 	CALL    lable#v(HIGH(target))
 	ENDM


;	===============================================================
;	ENDIF TABLE_H
#endif
