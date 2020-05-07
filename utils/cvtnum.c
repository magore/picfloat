//;File: cvtnum.c
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
#include "cvtnum.h"

char * tr_msg[] = {
	"LSB",
	"",
	"",
	"MSB",
	"EXP",
	"SIGN",
};

unsigned char sh_msb_c(unsigned char *str, int cnt, unsigned char carryin)
{

    unsigned char carryo;
    while(cnt-- > 0) {
        carryo = (*str & 0x80) ? 1 : 0 ;
        *str = (*str << 1) | carryin;
        carryin = carryo;
        ++str;
    }
    return(carryo);
}

unsigned char sh_lsb_c(unsigned char *str, int cnt, unsigned char carryin)
{

    unsigned char carryo;
    str += cnt;
    while(cnt-- > 0) {
        --str;
        carryo = (*str & 1) ? 1 : 0 ;
        *str = (*str >> 1) | (carryin ? 0x80 : 0);
        carryin = carryo;
    }
    return(carryo);
}

unsigned char increment(unsigned char *str, int count)
{
    unsigned char carry = 1;
    unsigned int num = 0;
    while(count-- > 0) {
		num = carry;
		num += *str;
		*str = num;
		carry = (num >= 0x100) ? 1 : 0;
        ++str;
    }
    return(carry);
}

// Convert Double to temporary real (TR) format number
void d2tr(double din, unsigned char *tr, unsigned int flag)
{
	int i;
	unsigned int carry;
	unsigned int dexp,dsign;
	unsigned char *dp_ptr;

//  Point to din
	dp_ptr = (unsigned char *) &din;

// Extract TR Sign from double
	dsign = dp_ptr[7] & 0x80;
// Extract Exponent, (note excess 0x3ff format)

	dexp = (dp_ptr[7] & 0x7f) << 4;
	dexp |= ((dp_ptr[6] & 0xF0U) >> 4) & 0x0f;
	dp_ptr[7] = 0;
	dp_ptr[6] &= 0x0f;

	if(dexp) {	// NON-ZERO ?
		dexp -= 0x3ff;
		dexp += 0x7f;
		if(dexp <= 0 ) {
			dexp = 0;
			printf("\t; %.10e (temp real) UNDERFLOW!!!\n",
				din);
		}
		if(dexp >= 0x100 ) {
			dexp = 0xff;
			printf("\t; %.10e (temp real) OVERFLOW!!!\n",
				din);
		}
		dexp &= 0xff;
		dp_ptr[6] |= 0x10;	// Implied bit
	}
// MSB , note impiled bit is at 0x10

// Adjust Implied bit into position
	sh_msb_c(dp_ptr,8,0);	// 0x20
	sh_msb_c(dp_ptr,8,0);	// 0x40
	sh_msb_c(dp_ptr,8,0);	// 0x80

// TODO round to nearest rules, tie break with round to even
// Round the truncated result, dp_ptr[2],dp_ptr[1],dp_ptr[0] are discarded
// so inspect upper bit of dp_ptr[2] for rounding into dp_ptr[3] ...
	if(dp_ptr[2] & 0x80) {
// Round up and check for overflow
		increment(dp_ptr+3,5);
		if(dp_ptr[7] & 1) {
			sh_lsb_c(dp_ptr,8,0);	// Restore Carry
			dexp++;					// Adjust Exponent
		}
	}
// C Mode ? keeps the impiled bit at 0x40 instead of 0x80
	if(flag & N_C_FLAG) {
		if(dexp && dp_ptr[6] & 0x80) {		// overflow bit
			sh_lsb_c(dp_ptr,8,0);
			dexp++;			// Adjust Exponent
		}
	}
// Sign
	tr[TR_SIGN] = dsign;
// Save TR EXP
	tr[TR_EXP] = dexp;
// Copy mantissa
	for(i=0;i<TR_MAN_SIZE;++i) 
		tr[i] = dp_ptr[i+3];
}

double tr2d(unsigned char *tr)
{

	int i;
	double d;
	unsigned carry;
	unsigned char *dptr;
	int dexp,dsign;

	dptr = (unsigned char *) &d;
// Translate temporary real back to double in order 

	dexp = tr[TR_EXP];
	dsign = tr[TR_SIGN];
// Init
	for(i=0;i<8;++i)
		dptr[i] = 0;
// NON-Zero ?
	if(dexp) {
		for(i=0;i<TR_MAN_SIZE;++i)
			dptr[i+3] = tr[i];
	}
	if(dexp && dptr[6] & 0x80)		// C Mode keeps MSB at 0x40 not 0x80
		sh_lsb_c(dptr,8,0);			// 0x40
	sh_lsb_c(dptr,8,0);				// 0x20
	sh_lsb_c(dptr,8,0);				// 0x10

// reset implied bit
	dptr[6] &= 0x0f;
	if(dexp) {
		dexp -= 0x7f;
		dexp += 0x3ff;
	}
	dptr[6] |= ((dexp & 0x0f) << 4);
	dptr[7] |= ((dexp >> 4) & 0xff);
// restore sign
	dptr[7] |= dsign;

	return(d);
}

	
