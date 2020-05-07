;File:	math.h
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
#ifndef	MATH_H
#define	MATH_H
NPOW6	equ 0x0a4fb11f;     1.0000000000e-32 (32bit IEEE)
NPOW5	equ 0x24e69595;     1.0000000000e-16 (32bit IEEE)
NPOW4	equ 0x322bcc77;     1.0000000000e-08 (32bit IEEE)
NPOW3	equ 0x38d1b717;     1.0000000000e-04 (32bit IEEE)
NPOW2	equ 0x3c23d70a;     1.0000000000e-02 (32bit IEEE)
NPOW1	equ 0x3dcccccd;     1.0000000000e-01 (32bit IEEE)
POW0	equ 0x3f800000;     1.0000000000e+00 (32bit IEEE)
POW1	equ 0x41200000;     1.0000000000e+01 (32bit IEEE)
POW2	equ 0x42c80000;     1.0000000000e+02 (32bit IEEE)
POW3	equ 0x461c4000;     1.0000000000e+04 (32bit IEEE)
POW4	equ 0x4cbebc20;     1.0000000000e+08 (32bit IEEE)
POW5	equ 0x5a0e1bca;     1.0000000000e+16 (32bit IEEE)
POW6	equ 0x749dc5ae;     1.0000000000e+32 (32bit IEEE)
TEN8	equ 0x4cbebc20;     1.0000000000e+08 (32bit IEEE)
TEN9	equ 0x4e6e6b28;     1.0000000000e+09 (32bit IEEE)
TEN10	equ 0x501502f9;     1.0000000000e+10 (32bit IEEE)
M_E	equ 0x402df854;     2.7182818285e+00 (32bit IEEE)
M_LOG2E	equ 0x3fb8aa3b;     1.4426950409e+00 (32bit IEEE)
M_LOG10E	equ 0x3ede5bd9;     4.3429448190e-01 (32bit IEEE)
M_LN2	equ 0x3f317218;     6.9314718056e-01 (32bit IEEE)
M_LN10	equ 0x40135d8e;     2.3025850930e+00 (32bit IEEE)
M_PI	equ 0x40490fdb;     3.1415926536e+00 (32bit IEEE)
M_PI_2	equ 0x3fc90fdb;     1.5707963268e+00 (32bit IEEE)
M_1_PI	equ 0x3ea2f983;     3.1830988618e-01 (32bit IEEE)
M_PI_4	equ 0x3f490fdb;     7.8539816340e-01 (32bit IEEE)
M_2_PI	equ 0x3f22f983;     6.3661977237e-01 (32bit IEEE)
M_2_SQRTPI	equ 0x3f906ebb;     1.1283791671e+00 (32bit IEEE)
M_SQRT2	equ 0x3fb504f3;     1.4142135624e+00 (32bit IEEE)
M_SQRT1_2	equ 0x3f3504f3;     7.0710678119e-01 (32bit IEEE)

_TEN	equ POW1
_PI	equ M_PI
_TENTH	equ NPOW1

;	==================================================================
;	ENDIF MATH_H
#endif
