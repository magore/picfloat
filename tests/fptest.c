//;File: ftest.c
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

#ifdef FPBIG
#define NUMBITS 32
#else
#define NUMBITS 24
#endif

FSIZE check(char *title, double dresult, double baseline)
{
	double sign;
	double bits;
	double delta;
	double ratio;


// correct - computed
	delta = baseline - dresult;
	ratio = delta/baseline;
	

	if( fabs(delta) == 0 || fabs(baseline) == 0) {
		bits = 32.0;
	}
	else {
		bits = -log(fabs(ratio)) / log(2.0);
	}

	if( bits < NUMBITS) {
		printf("%s=%+3.8e, AbsErr: %+3.8e, RelBits=%+3.2f\n",
			title,dresult, delta, bits);
	}
	else {
		printf("%s=%+3.8e, Abserr: %+3.8e, RelBits=PERFECT\n",
			title, dresult, delta);
	}
}

double tab[] = {
	1.0,
	0.1,
	0.01,
	0.001,
	0.0001,
	0.0,
	-1.0 
};

main(int argc, char *argv[])
{
	int i,j,k;
	double angle = 0.0;
	double c1,c2;
	float c3;	
	double c4;
	double rads;
	double error;
	double dresult,baseline;
	FSIZE frads;
	FSIZE fresult;
	char title[1024];
	long count;


	if(argc > 1)
		pic_init(argv[1]);

	for(i=-360;i<=360;i+=45) {
		for(j=0;j<=1;++j) {
			for(k=0;tab[k] >= 0;++k) {
				if(j == 0)
					angle = (double) i - tab[k];
				else
					angle = (double) i + tab[k];
				rads = M_PI * angle / 180.0;
				baseline = sin(rads);
				frads = rads;
				// esin(&frads,&fresult); dresult = fresult;
				dresult = dsin(rads); 
				sprintf(title, "SIN(%.10f)=", angle);
				check(title, dresult, baseline);
				cycles();
			}
		}
	}
	for(i=-360;i<=360;i+=45) {
		for(j=0;j<=1;++j) {
			for(k=0;tab[k] >= 0;++k) {
				if(j == 0)
					angle = (double) i - tab[k];
				else
					angle = (double) i + tab[k];
				rads = M_PI * angle / 180.0;
				baseline = cos(rads);
				// frads = rads; ecos(&frads,&fresult); dresult = fresult;
				dresult = dcos(rads);
				sprintf(title, "COS(%.10f)=", angle);
				check(title, dresult, baseline);
				cycles();
			}
		}
	}
	for(i=-360;i<=360;i+=45) {
		for(j=0;j<=1;++j) {
			for(k=0;tab[k] >= 0;++k) {
				if(j == 0)
					angle = (double) i - tab[k];
				else
					angle = (double) i + tab[k];
				rads = M_PI * angle / 180.0;
				baseline = tan(rads);
				// frads = rads; etan(&frads,&fresult); dresult = fresult;
				dresult = dtan(rads);
				sprintf(title, "TAN(%.10f)=", angle);
				check(title, dresult, baseline);
				cycles();
			}
		}
	}
	// for(i=-360;i<361;++i) {
	for(i=-360;i<361;++i) {
		angle = (double) i;
		rads = M_PI * angle / 180.0;
		baseline = sin(rads);
// SIN
		// frads = rads; esin(&frads,&fresult); dresult = fresult;
		dresult = dsin(rads);

		sprintf(title, "SIN(%.10f)=", angle);
		check(title, dresult, baseline);
		cycles();
	}

	// for(i=-360;i<361;++i) {
	for(i=-360;i<361;++i) {
		angle = (double) i;
		rads = M_PI * angle / 180.0;
		baseline = cos(rads);
// COS
		// frads = rads; ecos(&frads,&fresult); dresult = fresult;
		dresult = dcos(rads);

		sprintf(title, "COS(%.10f)=", angle);
		check(title, dresult, baseline);
		cycles();
	}

	// for(i=-360;i<361;++i) {
	for(i=-360;i<361;++i) {
		angle = (double) i;
		rads = M_PI * angle / 180.0;
		baseline = tan(rads);
// TAN
		// frads = rads; etan(&frads,&fresult); dresult = fresult;
		dresult = dtan(rads);

		sprintf(title, "TAN(%.10f)=", angle);
		check(title, dresult, baseline);
		cycles();
	}
	return(0);
}
