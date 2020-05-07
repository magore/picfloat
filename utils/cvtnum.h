//;File: cvtnum.h
// ========================================================================
// PIC Floating point library
// ========================================================================
//  Copyright (C) 1991,1997,1998,1999,2000,2001,2002,2003 Mike Gore 
//  All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the <ORGANIZATION> nor the names of its contributors 
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE
//
//  Contact Information
//  Mike Gore
//  Infowrite Consulting
//  405 Midwood Cres.
//  Waterloo Ont
//  N2L 5N4
//  Phone:       519-884-4943 home
//  Fax:         519-885-0548 fax
//  Email:       magore@sympatico.ca - or - magore@uwaterloo.ca
// ========================================================================

// ========================================================================
// ***** Please Use TABS = 4 when viewing this file ****
// ========================================================================
#define N_EXT 1
#define N_TR 2
#define N_PIC 4
#define N_EQ 8
#define N_C_FLAG 16

#define TR_MAN_LSB	0
#define TR_MAN_MSB	3
#define TR_MAN_SIZE	4
#define TR_EXP		4
#define TR_SIGN		5
#define TR_SIZE		6

extern char * tr_msg[];

/* cvtnum.c */
unsigned char sh_msb_c ( unsigned char *str , int cnt , unsigned char carryin );
unsigned char sh_lsb_c ( unsigned char *str , int cnt , unsigned char carryin );
unsigned char increment ( unsigned char *str , int count );
void d2tr ( double din , unsigned char *tr , unsigned int flag );
double tr2d ( unsigned char *tr );
