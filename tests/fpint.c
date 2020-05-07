//;File: fpint.c
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
#include <fpu_control.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "picfp.c"

#define myabs(a)	((a < 0.0) ? -(a) : (a))

FSIZE check(char *title, FSIZE fresult, double baseline)
{
	double sign;
	double bits;
	double delta;


	delta = (double) fresult - baseline;

	if( delta == 0.0) {
		bits = 24;

	}
	else {
		bits = log((double) myabs(delta/baseline)) / log(2.0);
	}

	bits = myabs(bits);

	if(bits < 24.0) {
		printf("%s=%+3.8e,%+3.8e, BITS=%+3.2f\n",
			title,(double)fresult, (double) delta, (double) bits);
	}
	else {
		printf("%s=%+3.8e,%+3.8e, BITS=PERFECT\n",
			title, (double) fresult, (double)delta);
	}
}

main(int argc, char *argv[])
{
	int i;
	double dnum;
	FSIZE number;
	FSIZE integer;
	FSIZE fraction;
	char title[1024];

	pic_init(argv[1]);

	dnum = 0x200000 - 1;

	for(i=0;i<40;++i) {
		number = dnum;
		__FP_LOAD_IEEE_AC((unsigned char *)&number);
		__FP_SPLIT();
		__FP_STORE_IEEE_AC((unsigned char *)&integer);
		__FP_STORE_IEEE_ARG((unsigned char*)&fraction);
		printf("num:%.10e,int:%.10e,frac:%.10e\n",
			(double)number,
			(double)integer,
			(double)fraction);
		dnum /= 2.0;
		cycles();
	}

	
	return(0);
}

		

	

	

	


