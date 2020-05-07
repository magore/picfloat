;File:	macro.h
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
#ifndef	MACRO_H
#define	MACRO_H
	NOLIST
	NOEXPAND
;	========================================================================
;	Microchip macros for single and multibyte operations:
;	Arithmetic
;	Boolian
;	Shifts
;	Compare (signed/unsigned)
;	Load
;	Store
;	Move
;	Tables (simple - see table macro file for robust versions)
;
;	========================================================================

;	========================================================================
;	Notes: 
;	- Multibyte Macros: to save space can be used to build subroutines
;	for multi-byte functions
;	
;	- TAB STOPS in this document are set to 4!
;
;	Assumptions: 
;	- LITTLE ENDIAN byte order (Low byte first)
;	- CONSTANTS limited to 4 bytes by the assembler
;
;	========================================================================

;	========================================================================
;	Temp Variable used for Signed Comparisons
	cblock
	MACRO_tmp1
	endc
;	========================================================================
;	Logical
TRUE	equ 1
FALSE	equ 0

;	Bit numbers
MSB	equ	7
LSB	equ 0

;	========================================================================
; Control structure variables
	variable	_FOR_STACK
	variable	_ELSE_STACK
	variable	_IF_STACK


;	========================================================================
;	BIT RELATED macros
;	========================================================================


;	SET A BIT:	MEM,BIT
BSET	MACRO MEM,BIT
	bsf	MEM,BIT
	ENDM
	
;	CLEAR A BIT:  MEM,BIT
BCLR	MACRO MEM,BIT
	bcf	MEM,BIT
	ENDM

;	========================================================================
;	Condition Codes
;	========================================================================

;	Clear Borrow STatus
CLRB	MACRO 
	BSET	STATUS,C
	ENDM

;	Set Borrow Status
SETB	MACRO 
	BCLR	STATUS,C
	ENDM

;	========================================================================
;	These next 4 condition code macros are defined in MPLAB

;	Clear Z Status
;CLRZ	MACRO 
;	BSET	STATUS,Z
;	ENDM
	
;	Set Z Status
;SETZ	MACRO 
;	BCLR	STATUS,Z
;	ENDM
	

;	Clear Carry Status
;CLRC	MACRO
;	clrc
;	ENDM

;	Set Carry Status
;SETC	MACRO
;	setc
;	ENDM

;	========================================================================
;	SKIP macros - need to be carefull when using with other macros!
;	========================================================================
;

;	Skip Bit Set: MEM,BIT
SKIP_BSET	MACRO MEM,BIT
	btfss MEM,BIT
	ENDM

;	Skip Bit Clear:	MEM,BIT
SKIP_BCLR	MACRO MEM, BIT
	btfsc	MEM,BIT
	ENDM

;	Skip Not Zero
SKIP_NZ	MACRO
	SKIP_BCLR	STATUS,Z
	ENDM

;	Skip Zero
SKIP_Z	MACRO
	SKIP_BSET	STATUS,Z
	ENDM

;	Skip Not Carry
SKIP_NC	MACRO
	SKIP_BCLR	STATUS,C
	ENDM

;	Skip Carry
SKIP_C	MACRO
	SKIP_BSET	STATUS,C
	ENDM

;	SKip Not Borrow
SKIP_NB	MACRO
	SKIP_BSET	STATUS,C
	ENDM

;	Skip Borrow
SKIP_B	MACRO
	SKIP_BCLR	STATUS,C
	ENDM

;	========================================================================
;	UNCONDITIONAL JUMP/RETURN/CALL
;	========================================================================

;	Jump: TARGET
JMP	MACRO TARGET
	goto	TARGET
	ENDM

;	Call Subroutine:	TARGET
;	CALL MACRO   TARGET
;	call	TARGET
;	ENDM

;	Return 8bit CONSTANT
RET8I	MACRO const
	retlw const
	ENDM
	
;	End Subroutine
ENDSUB	MACRO
	return
	ENDM

;	========================================================================
;	JUMP ZERO STATUS macros
;	TO - FROM
;	========================================================================

;	Jump Zero:	TARGET
JMP_Z	MACRO TARGET
	SKIP_NZ
	JMP	TARGET
	ENDM

;	Jump Not Zero:	TARGET
JMP_NZ	MACRO TARGET
	SKIP_Z
	JMP	TARGET
	ENDM

;	========================================================================
;	JUMP CARRY STATUS macros
;	TO - FROM
;	========================================================================
;	Jump Carry:   TARGET
JMP_C	MACRO	TARGET
	SKIP_NC
	JMP	TARGET
	ENDM

;	Jump Not Carry:	TARGET
JMP_NC	MACRO TARGET
	SKIP_C
	JMP	TARGET
	ENDM

;	========================================================================
;	Jump Borrow
;	TO - FROM
;	========================================================================
;	JUMP IF STATUS BORROW: TARGET
JMP_B	MACRO TARGET
	SKIP_NB
	JMP	TARGET
	ENDM

;	Jump Not Borrow: TARGET
JMP_NB	MACRO TARGET
	SKIP_B
	JMP	TARGET
	ENDM

;	========================================================================
;	JUMP ON BIT SET/CLEAR
;	========================================================================

;	Jump Bit Set: MEM,BIT,TARGET
JMP_BSET	MACRO MEM,BIT, TARGET
	SKIP_BCLR MEM,BIT
	JMP	TARGET
	ENDM
	
;	Jump Bit Clear:	MEM,BIT,TARGET
JMP_BCLR	MACRO MEM,BIT, TARGET
	SKIP_BSET MEM,BIT
	JMP	TARGET
	ENDM

;	========================================================================
;	8 Bit MOV/SWAP
;	MOVE TO, FROM
;	========================================================================

;	Move 8bit Memory, 8bit Memory: TO,FROM
MOV8	MACRO TO,FROM
	movfw FROM
	movwf TO
	ENDM

;	LOAD 8bit Memory, 8bit CONSTANT:   MEM,CONSTANT
LOAD8I	MACRO MEM,const
	movlw LOW(const)
	movwf MEM
	ENDM

;	SWAP 8bit Memory Locations: TO,FROM
SWAP8	MACRO TO,FROM
	movfw TO	; W=TO
	xorwf FROM,W  ; W=(TO xor FROM)
	xorwf FROM,f  ; FROM=(original TO)=((TO xor FROM) xor FROM)
	xorwf TO,f; TO =(original FROM)=((TO xor FROM) xor TO)
	ENDM

;	========================================================================
;	Unary SINGLE BYTE (8BIT) MEMORY OPS
;	========================================================================


;	Clear 8bit Memory:	MEM
CLR8	MACRO MEM
	clrf	MEM
	ENDM

;	Compliment 8bit Memory:	MEM
COM8	MACRO MEM
	comf	MEM,f
	ENDM

;	DEC 8bit Memory:	MEM
DEC8	MACRO MEM
	decf	MEM,f
	ENDM

;	INC 8bit Memory:	MEM
INC8	MACRO MEM
	incf	MEM,f
	ENDM

;	NEG 8bit Memory:	MEM
NEG8	MACRO MEM
	COM8	MEM
	INC8	MEM
	ENDM

;	ROTATE RIGHT 8bit Memory (towards LSB): MEM
RR8_LSB	MACRO MEM
	rrf	MEM,f
	ENDM


;	ROTATE LEFT 8bit Memory (towards MSB):	MEM
RL8_MSB	MACRO MEM
	rlf	MEM,f
	ENDM

;	SHIFT RIGHT 8bit Memory (towards LSB):	MEM
SH8_LSB	MACRO MEM
	CLRC
	rrf	MEM,f
	ENDM

;	SHIFT RIGHT 8bit Memory ARITHMETIC (towards LSB):	MEM
ASH8_LSB	MACRO MEM
	CLRC
	SKIP_BCLR MEM,MSB
	SETC
	rrf	MEM,f
	ENDM

;	SHIFT LEFT 8bit Memory (towards MSB): MEM
SH8_MSB	MACRO MEM
	CLRC
	rlf	MEM,f
	ENDM

;	========================================================================
;	8 Bit UNARY LOGICAL
;	========================================================================
;	AND 8bit Memory with 8bit Memory:	TO,FROM
AND8	MACRO TO,FROM
	movfw FROM
	andwf TO,f
	ENDM

;	AND 8bit Memory with 8bit CONSTANT:   TO,CONSTANT
AND8I	MACRO TO,const
	movlw const
	andwf TO,f
	ENDM

;	OR 8bit Memory with 8bit Memory:	TO,FROM
OR8	MACRO TO,FROM
	movfw FROM
	iorwf TO,f
	ENDM

;	OR 8bit Memory with 8bit CONSTANT:	TO,CONSTANT
OR8I	MACRO TO,const
	movlw const
	iorwf TO,f
	ENDM

;	XOR 8bit Memory with 8bit Memory:	TO,FROM
XOR8	MACRO TO,FROM
	movfw FROM
	xorwf TO,f
	ENDM

;	XOR 8bit Memory with 8bit CONSTANT:   TO,CONSTANT
XOR8I	MACRO TO,const
	movlw const
	xorwf TO,f
	ENDM

;	========================================================================
;	8 Bit ARRITHMETIC
;	========================================================================

;	ADD 8bit Memory with 8bit Memory:	TO,FROM
ADD8	MACRO TO,FROM
	movfw FROM
	addwf TO,f
	ENDM

;	ADD 8bit Memory with 8bit CONSTANT:   TO,CONSTANT
ADD8I	MACRO TO,const
	movlw const
	addwf TO,f
	ENDM


;	ADD 8bit Memory with 8bit Memory with Carry:  TO,FROM
ADDC8	MACRO TO,FROM
	movfw FROM
	SKIP_NC
	incfsz FROM,w
	addwf TO,f	; Leave Carry set IF FROM = 256
	ENDM

;	ADD 8bit Memory with 8bit CONSTANT with Carry:	TO,CONSTANT
ADDC8I	MACRO	TO,const
	IF (((const) + 1) & 0xff)  != 0 ; if 0x100 KEEP CARRY
	movlw const
	SKIP_NC
	movlw ((const) + 1) & 0xff
	addwf TO,f
	ENDIF 
	ENDM


;	SUB 8bit Memory with 8bit Memory:	TO,FROM
SUB8	MACRO TO,FROM
	movfw FROM
	subwf TO,f
	ENDM


;	SUB 8bit Memory with 8bit Memory with Borrow: TO,FROM
SUBB8	MACRO TO,FROM
	movfw FROM
	SKIP_NB
	incfsz FROM,w
	subwf TO,F
	ENDM

;	SUB 8bit Memory with 8bit CONSTANT:   TO,CONSTANT
SUB8I	MACRO TO,const
	movlw const
	subwf TO,f
	ENDM

;	SUB 8bit Memory with 8bit CONSTANT with Borrow:	TO,CONSTANT
SUBB8I	MACRO TO,const
	movlw const
	SKIP_NB
	movlw ((const) + 1) & 0xff
	IF (((const) + 1) & 0xff)  != 0 ; IF 0x100 KEEP CARRY!
	subwf TO,f
	ENDIF 
	ENDM


;	========================================================================
;	TWO ARG UNSIGNED COMPARE CONDITION CODES TRUTH TABLE
;
;	Note **** Microchip uses Borrow Logic ****
;	X = Don't care (need additional status)
;
;	*IF WE WERE* using Carry Logic then:
;	TO - FROM
;	C, Z	TO == FROM
;	XC,NZ	TO != FROM
;	NC,NZ	TO < FROM
;	NC,XZ	TO <= FROM
;	C, NZ	TO > FROM
;	C, XZ	TO >= FROM
;
;	Using Borrow (NOT Carry) we have:
;	TO - FROM
;	NB,Z	TO == FROM
;	XB,NZ	TO != FROM
;	B, NZ	TO < FROM
;	B, XZ	TO <= FROM
;	NB,NZ	TO > FROM
;	NB,XZ	TO >= FROM
;
;	========================================================================

;	========================================================================
;	Compare Unsigned
;	CMP Unsigned 8bit Memory with 8bit Memory - using Borrow Logic: TO,FROM
CMP8	MACRO TO,FROM
	movfw FROM
	subwf TO,w	; W = FROM, TO - W
	ENDM

;	Compare Unsigned Immediate
;	CMP Unsigned 8bit Memory with 8bit CONSTANT - Borrow Logic: TO,CONSTANT
CMP8I	MACRO TO,const
	movlw const
	subwf TO,w
	ENDM

;	Compare Signed
;	CMP Signed 8bit Memory with 8bit Memory - Borrow Logic: TO,FROM
SCMP8	MACRO	TO,FROM
	movfw TO
	xorlw 0x80
	movwf MACRO_tmp1
	movfw FROM
	xorlw 0x80
	subwf MACRO_tmp1,W	; MACRO_tmp1=AC^0x80, MACRO_tmp1-(W^0x80)
	ENDM


;	Compare Signed Immediate
;	CMP Signed 8bit Memory with 8bit CONSTANT - using Borrow Logic: TO,FROM
SCMP8I	MACRO	TO,const
	movfw TO
	xorlw 0x80
	movwf MACRO_tmp1
	movlw	(const) ^ 0x80
	subwf MACRO_tmp1,w
	ENDM

;	========================================================================
;	JUMP TESTS AFTER PRIOR COMPARE/SUBTRACT macros
;
;	Test if the AC is LE,L,GE,G then the tested TO 
;	Using Borrow (NOT Carry) we have:
;	TO - FROM
;	NB,Z	TO == FROM
;	XB,NZ	TO != FROM
;	B, NZ	TO < FROM
;	B, XZ	TO <= FROM
;	NB,NZ	TO > FROM
;	NB,XZ	TO >= FROM
;
;	Set Grater or Equal Status
;	|TO| >= |FROM|
SET_GE	MACRO
	CLRB
	ENDM

;	Set Plus Status
;	|TO| >= |FROM|
SET_P	MACRO
	CLRB
	ENDM

;	Set Grater Status
;	|TO| > |FROM|
SET_G	MACRO
	CLRZ
	CLRB
	ENDM

;	Set Less or Equal Status
;	|TO| <= |FROM|
SET_LE	MACRO
	SETZ
	SETB
	ENDM

;	Set Less Status
;	|TO| < |FROM|
SET_L	MACRO
	SETB
	CLRZ
	ENDM

;	Set Negitive Status
;	|TO| < |FROM|
SET_N	MACRO
	SETB
	CLRZ
	ENDM


;	Jump Grater or Equal Status:  TARGET
;	|TO| >= |FROM|
JMP_GE	MACRO TARGET
	JMP_NB TARGET  ; NB branch, Z don't care
	ENDM

;	Jump Plus Status:	TARGET
;	|TO| >= |FROM|	TO - FROM >= 0
JMP_P	MACRO TARGET
	JMP_GE TARGET
	ENDM

;	Jump Grater Status:   TARGET
;	|TO| > |FROM|
JMP_G	MACRO TARGET
	local JMP_G_X
	JMP_Z JMP_G_X ; Z don't branch
	JMP_NB TARGET ; NB branch
JMP_G_X
	ENDM

;	Jump Less or Equal Status:	TARGET
;	|TO| <= |FROM|
JMP_LE	MACRO TARGET
	JMP_Z TARGET  ; Z branch
	JMP_B TARGET  ; B branch
	ENDM

;	Jump Less Status:	TARGET
;	|TO| < |FROM|
JMP_L	MACRO TARGET
	local JMP_L_X
	JMP_Z JMP_L_X ; Z don't branch
	JMP_B TARGET ; B branch, Z don't care
JMP_L_X
	ENDM

;	Jump Negative Status: TARGET
;	|TO| < |FROM|	TO - FROM < 0
JMP_N	MACRO TARGET
	JMP_L TARGET
	ENDM

;	========================================================================
;	TEST 8bit Memory - set Zero / Non Zero status: MEM
TST8	MACRO	MEM
	movf	MEM,f
	ENDM

;	========================================================================
;	LOOP with MEM8 and test condition codes
;	========================================================================

;	Increment 8bit Memory Jump Not Zero: MEM
IJNZ8	MACRO MEM,TARGET
	incfsz MEM,f
	JMP	TARGET
	ENDM

;	Increment 8bit Memory Jump Zero:	MEM
IJZ8	MACRO MEM,TARGET
	local NZ
	incfsz MEM,f
	JMP	NZ
	JMP	TARGET
NZ
	ENDM

;	Decrement 8bit Memory Jump Not Zero:  MEM
DJNZ8	MACRO MEM,TARGET
	decfsz MEM,f
	JMP	TARGET
	ENDM

;	Decrement 8bit Memory Jump Zero:	MEM
DJZ8	MACRO MEM,TARGET
	local NZ
	decfsz MEM,f
	JMP	NZ
	JMP	TARGET
NZ
	ENDM

;	========================================================================
;	MULTI BYTE macros
;	IN_ prefix implies inline 
;
;	Typically these macros would be used to build subroutines
;	========================================================================


;	========================================================================
;	CLR Memory:   MEM,COUNT
IN_CLR	MACRO MEM, count
	local index = 0
	WHILE index < count
	CLR8 MEM+index
index	= index + 1 
	ENDW
	ENDM


;	MOVE Memory TO,FROM,COUNT:	TO,FROM,COUNT
IN_MOV	MACRO TO,FROM, count
	local index = 0
	WHILE index < count
	MOV8	TO+index, FROM+index
index	= index + 1 
	ENDW
	ENDM

;	SWAP Memory TO,FROM,COUNT:	TO,FROM,COUNT
IN_SWAP	MACRO TO, FROM, count
	local index = 0
	WHILE index < count
	SWAP8 TO+index, FROM+index
index	= index + 1 
	ENDW
	ENDM

;	MOVE Memory IMMEDIATE TO,CONSTANT,COUNT:	TO,CONSTANT,COUNT
IN_LOADI	MACRO TO, const, count
	local index = 0
	WHILE index < count
	LOAD8I TO+index, ((const) >> (index*8)) & 0xff
index	= index + 1 
	ENDW
	ENDM


;	========================================================================
;	SINGLE ARG TEST Zero or Not Zero

;	TEST Memory   MEM,COUNT for Zero / Non Zero:  MEM,COUNT
IN_TST	MACRO MEM, count
	local index = 0
	WHILE index < count
	IF index < 1
	movf	MEM+index,W
	ELSE
	iorwf MEM+index,W
	ENDIF 
index	= index + 1
	ENDW
	ENDM

;	========================================================================
;	COMPARE UNSIGNED , SET CONDITION CODES
;	Using Borrow (NOT Carry)
;	TO - FROM
;	NB,Z	TO == FROM
;	XB,NZ	TO != FROM
;	B, NZ	TO < FROM
;	B, XZ	TO <= FROM
;	NB,NZ	TO > FROM
;	NB,XZ	TO >= FROM
;

;	COMPARE Memory UNSIGNED Set Status:  TO,FROM,COUNT
IN_CMP	MACRO TO,FROM, count
	local NZ
	local index = count
	WHILE index > 0
	CMP8	TO+index-1, FROM+index-1
	JMP_NZ NZ
index	= index - 1 
	ENDW
NZ
	ENDM

;	COMPARE Memory IMMEDIATE - Set Status: SIGNED TO,FROM,CONSTANT
IN_SCMP	MACRO TO,FROM, count
	local NZ
	local index = count
	WHILE index > 0
	IF index == count
;	MSB
	SCMP8 TO+index-1,FROM+index-1
	ELSE
;	LSB
	CMP8	TO+index-1, FROM+index-1
	ENDIF 
	JMP_NZ NZ
index	= index - 1 
	ENDW
NZ
	ENDM

;	========================================================================
;	COMPARE Memory IMMEDIATE UNSIGNED Set Status:  TO,CONSTANT,COUNT
IN_CMPI	MACRO TO, const, count
	local NZ
	local index = count 

	WHILE index > 0
	CMP8I TO+index-1, ((const) >> ((index-1)*8)) & 0xff
	JMP_NZ NZ
index	= index - 1
	ENDW
NZ
	ENDM

;	COMPARE Memory IMMEDIATE SIGNED Set Status: MEM,CONSTANT,COUNT
IN_SCMPI	MACRO MEM,const, count
	local NZ
	local index = count
	WHILE index > 0
	IF index == count
;	MSB
	SCMP8I MEM+index-1,((const) >> ((index-1)*8)) & 0xff
	ELSE
;	LSB
	CMP8I MEM+index-1,((const) >> ((index-1)*8)) & 0xff
	ENDIF 
	JMP_NZ	NZ
index	= index - 1 
	ENDW
NZ
	ENDM

;	========================================================================
;	TWO ARG ARITHMETIC 

;	a += b
;	ADD Memory:   TO,FROM, COUNT
IN_ADD	MACRO TO,FROM, count
	local index = 0

	WHILE index < count
	IF index < 1
	ADD8	TO+index,FROM+index
	ELSE
	ADDC8	TO+index,FROM+index
	ENDIF 
index	= index + 1
	ENDW
	ENDM

;	a += b carry
;	ADD Memory with Carry: TO,FROM,COUNT
IN_ADDC	MACRO TO,FROM, count
	local index = 0

	WHILE index < count
	ADDC8	TO+index,FROM+index
index	= index + 1
	ENDW
	ENDM

;	a -= b
;	SUB Memory: TO,FROM,COUNT
IN_SUB	MACRO TO,FROM, count
	local index = 0

	WHILE index < count
	IF index < 1
	SUB8	TO+index,FROM+index
	ELSE
	SUBB8 TO+index,FROM+index
	ENDIF 
index	= index + 1
	ENDW
	ENDM

;	a -= b with carry
;	SUB Memory with Borrow: TO,FROM,COUNT
IN_SUBB	MACRO TO,FROM, count
	local index = 0
	WHILE index < count
	SUBB8 TO+index,FROM+index
index	= index + 1
	ENDW
	ENDM

;	========================================================================
;	TWO ARG CONSTANT ARITHMETIC 

;	a += CONSTANT
;	ADD Memory: MEM,CONSTANT,COUNT
IN_ADDI	MACRO MEM, const, count
	local index = 0

	WHILE index < count
	IF index < 1
	ADD8I MEM+index, (((const) >> (index*8)) & 0xff)
	ELSE
	ADDC8I MEM+index, (((const) >> (index*8)) & 0xff)
	ENDIF 
index	= index + 1
	ENDW
	ENDM

;	a -= CONSTANT
;	SUB Memory: MEM,CONSTANT,COUNT
IN_SUBI	MACRO	MEM,const, count
	local index = 0

	WHILE index < count
	IF index < 1
	SUB8I	MEM+index, (((const) >> (index*8)) & 0xff)
	ELSE
	SUBB8I	MEM+index, (((const) >> (index*8)) & 0xff)
	ENDIF 
index	= index + 1
	ENDW
	ENDM

;	========================================================================
;	ONE ARGS ARITHMETIC
;	a++
;	INC Memory: MEM,COUNT
IN_INC	MACRO MEM, count
	local index = 0

	WHILE index < count
	INC8	MEM+index
	IF index + 1 < count
	SKIP_NZ	
	ENDIF 
index	= index + 1
	ENDW
	ENDM


;	a--
;	DEC Memory: MEM,COUNT
IN_DEC	MACRO	MEM, count
	
	local index = 0
	local DONE
	WHILE index < count
	SUB8I MEM+index,1
	JMP_NB DONE
index	= index + 1
	ENDW
DONE
	ENDM

;	========================================================================
;	TWO ARG LOGICAL 


;	a &= b
;	AND Memory: TO,FROM,COUNT
IN_AND	MACRO TO,FROM, count
	local index = 0

	WHILE index < count
	AND8	TO+index,FROM+index
index	= index + 1
	ENDW
	ENDM

;	a |= b
;	OR Memory: TO,FROM,COUNT
IN_OR	MACRO TO,FROM, count
	local index = 0
	WHILE index < count
	OR8	TO+index,FROM+index
index	= index + 1
	ENDW
	ENDM

;	a ^= b
;	XOR Memory: TO,FROM,COUNT
IN_XOR	MACRO TO,FROM, count
	local index = 0
	WHILE index < count
	XOR8	TO+index,FROM+index
index	= index + 1
	ENDW
	ENDM

;	========================================================================
;	ONE ARGS ARITHMETIC

;	a = ~a;
;	COM Memory: MEM,COUNT
IN_COM	MACRO MEM, count
	local index = 0
	WHILE index < count
	COM8	MEM+index
	
index	= index + 1
	ENDW
	ENDM

;	a = -a
;	NEG Memory: MEM,COUNT
IN_NEG	MACRO MEM,count
	IN_COM MEM,count
	IN_INC MEM,count
	ENDM

;	a >>= 1
;	Shift Memory Towards LSB: MEM,COUNT
IN_SH_LSB	MACRO MEM, count
	local index = 0
	CLRC
	WHILE index < count
	RR8_LSB MEM+count-index-1
index	= index + 1
	ENDW
	ENDM

;	a >>= 1 carry
;	Shift Memory Towards LSB with Carry: MEM,COUNT
IN_SH_LSBC	MACRO	MEM, count
	local index = 0
	WHILE index < count
	RR8_LSB MEM+count-index-1
index	= index + 1
	ENDW
	ENDM

;	a <<= 1
;	Shift Memory Towards MSB: MEM,COUNT
IN_SH_MSB	MACRO	MEM, count
	local index = 0
	CLRC
	WHILE index < count
	RL8_MSB MEM+index
index	= index + 1
	ENDW
	ENDM

;	a <<= 1 carry
;	Shift Memory Towards MSB with Carry: MEM,COUNT
IN_SH_MSBC	MACRO MEM, count
	local index = 0
	WHILE index < count
	RL8_MSB MEM+index
index	= index + 1
	ENDW
	ENDM

;	========================================================================
;
;	A Few 16 bit examples
;
;	INC 16Bit Memory:	MEM
INC16	MACRO MEM
	IN_INC MEM,2
	ENDM

;	DEC 16Bit Memory: MEM
DEC16	MACRO MEM
	IN_DEC MEM,2
	ENDM

;	NEG 16Bit Memory: MEM
NEG16	MACRO	MEM
	IN_NEG MEM,2
	ENDM

;	TST 16Bit Memory MEM
TST16	MACRO MEM
	IN_TST MEM,2
	ENDM

;	CMP 16Bit Memory: TO,FROM
CMP16	MACRO TO,FROM
	IN_CMP TO,FROM,2
	ENDM

;	CMP 16Bit Memory:MEM,CONSTANT
CMP16I	MACRO MEM,const
	IN_CMPI MEM,const,2
	ENDM

;	ADD 16Bit Memory: TO,FROM
ADD16	MACRO TO,FROM
	IN_ADD TO,FROM,2
	ENDM

;	ADD 16Bit Memory: MEM,CONSTANT
ADD16I	MACRO MEM,const
	IN_ADDI MEM,const,2
	ENDM

;	SUB 16Bit Memory: TO,FROM
SUB16	MACRO TO,FROM
	IN_SUB TO,FROM,2
	ENDM

;	SUB 16Bit Memory: MEM,CONSTANT
SUB16I	MACRO MEM,const
	IN_SUBI MEM,const,2
	ENDM

;	MOV 16Bit Memory:  TO,FROM
MOV16	MACRO TO,FROM
	IN_MOV TO,FROM,2
	ENDM

;	MOV 16Bit Memory: MEM,CONSTANT
LOAD16I	MACRO MEM,const
	IN_LOADI MEM,const,2
	ENDM

;	========================================================================
;	8 bit source with 16 bit TARGET
;	Why ? Without these we wold have to move the source into 
;	a 16bit location - and use up more memory

;	Move Memory 8Bit to 16bit Unsigned: TO,FROM
MOV8_16U	MACRO TO,FROM
	MOV8	TO,FROM
	CLR8	TO+1
	ENDM

;	Extend 8Bit Memory to 16bit Memory: Signed MEM
I8TO16	MACRO MEM
	local done
	CLR8	MEM+1
	JMP_BCLR MEM,MSB,done
	DEC8	MEM+1
done
	ENDM

;	Extend 8Bit Memory to 16bit Memory Unsigned: MEM
U8TO16	MACRO MEM
	CLR8	MEM+1
	ENDM

;	SUB 8bit Memory from 16bit Memory Unsigned:  TO,FROM
SUB8_16U	MACRO TO,FROM
	local NB
	SUB8	TO,FROM
	JMP_NB NB
	DEC8	TO+1
NB
	ENDM

;	ADD 8bit Memory TO 16bit Memory Unsigned:	TO,FROM
ADD8_16U	MACRO TO,FROM
	local NC
	ADD8	TO,FROM
	JMP_NC NC
	INC8	TO+1
NC
	ENDM

;	CMP 8bit Memory with 16bit Memory Unsigned: TO,FROM
CMP8_16U	MACRO TO,FROM
	local NZ
	CMP8I TO+1,0  ; high byte
	JMP_NZ NZ
	CMP8	TO,FROM ; low byte
NZ
	ENDM

;	========================================================================


FOR		MACRO	LOOPVAR,STARTING,ENDING
	IN_LOADI	LOOPVAR,STARTING
_FOR#v(_FOR_STACK)
	IN_CMPI	LOOPVAR,ENDING
	JMP_Z	_NEXT#v(_FOR_STACK)
_FOR_STACK++				; PUSH STACK
	ENDM

BREAK MACRO
	JMP	_NEXT#v(_FOR_STACK-1)
	ENDM

NEXT	MACRO	LOOPVAR,STEP
_FOR_STACK--			; POP STACK
	IN_ADDI	LOOPVAR,STEP
	JMP	_FOR#v(_FOR_STACK)
_NEXT#v(_FOR_STACK)


IFBIT_SET	MACRO	VAR,BIT
	JMP_BCLR	VAR,BIT,_ENDIF#v(_IF_STACK)
_ELSE_STACK=IF_STACK
_IF_STACK++			; PUSH STACK
	ENDM

ELSEIF MACRO
_ELSE_STACK=IF_STACK
	JMP	_ENDIF#v(_IF_STACK-1)
_ENDIF#v(_IF_STACK)
	ENDM


IFBIT_CLR	MACRO	VAR,BIT
	JMP_BSET	VAR,BIT,_ENDIF#v(_IF_STACK)
_ELSE_STACK=IF_STACK
_IF_STACK++			; PUSH STACK
	ENDM

;ENDIF	MACRO
;	IF _IF_STACK != _ELSE_STACK
;_ENDIF#v(_IF_STACK)
;_IF_STACK--			; POP STACK
;		ENDM
;


;	========================================================================
;	MACRO_H
#endif
