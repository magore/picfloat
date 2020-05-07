//;File: fpsin.c
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


/* fp_quad
 *      Internal routine to split an angle into
 *      a given interval as an integer and fraction part
*/
int fp_quad(double *xp)
{
    int q;
    int flag = 0;

	if(*xp < 0.0) {
        flag = 8;       // Mark Sign
		*xp = -*xp;
	}
	q = *xp;
	*xp = *xp - (double) q;
    q &= 7;
    q |= flag;  // sign
    return(q);
}

//; p117, 3041, N=4

double sin_tab[] = {
	+.308974697000000e-06,
	-.365723485490000e-04,
	+.249039318253300e-02,
	-.807455120127930e-01,
	+.785398163393874e+00
};
//; p118, 3821, N=4
double cos_tab[] = {
	+.352876162000000e-05,
	-.325936500430000e-03,
	+.158543239125500e-01,
	-.308425134891180e+00,
	+.999999999943930e+00
};

//
// SIN(X) = z*P(z**2), z = x * SIN_SCALE
//
// 				3341 pg203
//
#define SIN_SCALE (4.0/1.0/M_PI)
double  fp_sin(double xp)
{
	double p;
	double *ptr;
	int quad;
//
// SIN
// 0	90   	90+		180- 	180		270-  	270  	360-
// 0	1		1		0		0		-1		-1	  	0
//

	xp *= SIN_SCALE;
	
	quad = fp_quad((double *)&xp);	
	switch(quad & 7) {
		case 1:	// 45 to 90
		case 5:	// 225 to 270
// Note when xp converges to 1.0 COS converges to 0
// The truncation error of xp is nearly matched by the 
// convergence of COS to 0 resulting in a low absolute error
			xp = 1.0-xp;	
		case 2:	// 90 to 135
		case 6:	// 270 to 315
			quad |= 16;
			ptr = cos_tab;
			break;
		case 3:	// 135 to 180
		case 7:	// 315 to 360
// when xp converges to 1.0 SIN converges to 0
// The truncation error of xp is nearly matched by the 
// convergence of SIN to 0 resulting in a low absolute error
			xp = 1.0-xp;
		case 0:
		case 4:
			ptr = sin_tab;
			break;
	}
			
	p = *ptr++;
	p *= xp;
	p *= xp;
	p += *ptr++;
	p *= xp;
	p *= xp;
	p += *ptr++;
	p *= xp;
	p *= xp;
	p += *ptr++;
	p *= xp;
	p *= xp;
	p += *ptr++;
	if (!(quad & 16))
		p *= xp;
	xp = fabs(p);
	
	if(quad & 4) 
		xp = -xp;
	if(quad & 8)
		xp = -xp;
	return(xp);
}	

#ifdef JUNK

/* fp_quad
 *      Internal routine to split an angle into
 *      a given interval as an integer and fraction part
*/
int FPquad,FPflag;
void fp_quad(double *xp)
{
    FPflag = 0;

	if(*xp < 0.0) {
        FPflag = 0x80;       // Mark Sign
		*xp = -*xp;
	}
	FPquad = *xp;
	*xp = *xp - (double) FPquad;
    FPquad &= 7;
}

double sin_tab[] = {
	5.4589878320e-08 ,
	-3.5955981533e-06 ,
	1.6043892991e-04 ,
	-4.6817533261e-03 ,
	7.9692626107e-02 ,
	-6.4596409750e-01 ,
	1.5707963268e+00 
};
//
// SIN(X) = z*P(z**2), z = x * SIN_SCALE
//
//
#define SIN_SCALE (2.0/M_PI)
double  fp_sincos(double xp, int cos)
{
	double p;
	double *ptr;
	int i;
//
// SIN
// 0	90   	90+		180- 	180		270-  	270  	360-
// 0	1		1		0		0		-1		-1	  	0
//

	xp *= SIN_SCALE;
	
	fp_quad((double *)&xp);	
	if(cos) {
		if(FPflag & 0x80)
			FPquad -= 1;
		else
			FPquad += 1;
	}
	if(FPquad & 1) 
		xp = 1.0-xp;
	if(FPquad & 2) 
		FPflag ^= 0x80;
	ptr = sin_tab;
			
	p = 0;
	for(i=0;i<6;++i) {
		p += *ptr++;
		p *= xp;
		p *= xp;
	}
	p += *ptr++;
	p *= xp;
	xp = fabs(p);

	if(FPflag & 0x80)
		xp = -xp;
	
	return(xp);
}	

#endif
