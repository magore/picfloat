//;File: picfp.c
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
// __NO_MATH_INLINES
#include <math.h>
#include <signal.h>

#include "pic.h"
#include "../utils/cvtnum.h"
// =====================================================

#ifdef FPBIG		// Control floating point size
#define FSIZE double	// Note TR format is only 40bits
#else
#define FSIZE float		// 32bit IEEE
#endif

#define NE 1
#define FLOAT(x) FSIZE x[NE] = {0}
static FSIZE eone[NE] = {1.0};  /* The constant 1.0 */
// =====================================================

#define U8 unsigned char

// ======================================================
// Address entry points
#include "picfp.h"

long cycle_save;
int max_stack_ind_save;

void _SAVE()
{
	cycle_save = cycle;
	max_stack_ind_save = max_stack_ind;
}

void _RESTORE()
{
	cycle = cycle_save;
	max_stack_ind = max_stack_ind_save;
}

// Display simulator cyles
void cycles()
{
	printf("cycles: %8lu, Stack depth:%d\n",cycle, max_stack_ind);
	// stack_dump(max_stack_ind);
}

void etoasc(FSIZE *a,char *str,int n)
{
        char tmp[256];
        unsigned char *src = (unsigned char *) a;
        sprintf( str, "%.10e", *a );
#if DEBUG
    sprintf(tmp, ":%02x%02x%02x%02x;",
                src[3], src[2], src[1], src[0]);
        strcat(str,tmp);
#endif
}
// ======================================================
// Instruction Trap handeler
// Used by PARANOI to detect FP errors
void my_trap(unsigned int pc)
{
#ifdef PIC_FPE
	if(pc == _FP_ERROR) {
		raise(SIGFPE);
	}
#endif
}

// ======================================================

#define wa(a,d) write_reg_abs(a,d)
#define ra(a) read_reg_abs(a)

// INT AC32 IO

void __AC32_LOAD(U8 *src)
{
        wa(AC32+3,src[3]);              // MSB
        wa(AC32+2,src[2]);
        wa(AC32+1,src[1]);
        wa(AC32+0,src[0]);
}

void __AC32_STORE(U8 *src)
{
        src[3] = ra(AC32+3);              // MSB
        src[2] = ra(AC32+2);
        src[1] = ra(AC32+1);
        src[0] = ra(AC32+0);
}

void __ARG32_LOAD(U8 *src)
{
        wa(ARG32+3,src[3]);              // MSB
        wa(ARG32+2,src[2]);
        wa(ARG32+1,src[1]);
        wa(ARG32+0,src[0]);
}

void __ARG32_STORE(U8 *src)
{
        src[3] = ra(ARG32+3);              // MSB
        src[2] = ra(ARG32+2);
        src[1] = ra(ARG32+1);
        src[0] = ra(ARG32+0);
}

// TR AC LOAD/STORE
void __FP_LOAD_TR_AC(U8 *src)
{
        wa(FPac_SIGN,src[5]);
        wa(FPac_EXP,src[4]);
        wa(AC32+3,src[3]);
        wa(AC32+2,src[2]);
        wa(AC32+1,src[1]);
        wa(AC32,src[0]);
}

void __FP_STORE_TR_AC(U8 *src)
{
        src[5] = ra(FPac_SIGN);		// SIGN
        src[4] = ra(FPac_EXP);      // EXP 
        src[3] = ra(AC32+3);        // MSB
        src[2] = ra(AC32+2);
        src[1] = ra(AC32+1);
        src[0] = ra(AC32+0);
}

// TR ARG LOAD/STORE
void __FP_LOAD_TR_ARG(U8 *src)
{
        wa(FParg_SIGN,src[5]);
        wa(FParg_EXP,src[4]);
        wa(ARG32+3,src[3]);
        wa(ARG32+2,src[2]);
        wa(ARG32+1,src[1]);
        wa(ARG32,src[0]);
}

void __FP_STORE_TR_ARG(U8 *src)
{
        src[5] = ra(FParg_SIGN);		// SIGN
        src[4] = ra(FParg_EXP);      // EXP 
        src[3] = ra(ARG32+3);        // MSB
        src[2] = ra(ARG32+2);
        src[1] = ra(ARG32+1);
        src[0] = ra(ARG32+0);
}

// IEEE AC LOAD/STORE
void __FP_LOAD_IEEE_AC(U8 *src)
{
    unsigned char tr[TR_SIZE];
    double r;
	double *d;
#ifndef FPBIG
        wa(AC32+3,src[3]);              // MSB
        wa(AC32+2,src[2]);
        wa(AC32+1,src[1]);
        wa(AC32+0,src[0]);
		_SAVE();
		my_ex_cmd(_IEEE_TO_TRac);
		_RESTORE();
#else
	d = (double *) src;
    d2tr((double) *d,tr,0);
    __FP_LOAD_TR_AC(tr);
#endif
}

void __FP_STORE_IEEE_AC(U8 *src)
{
    unsigned char tr[TR_SIZE];
    double r;
	double *d;
#ifndef FPBIG
		_SAVE();
		my_ex_cmd(_TRac_TO_IEEE);
		_RESTORE();
        src[3] = ra(AC32+3);              // MSB
        src[2] = ra(AC32+2);
        src[1] = ra(AC32+1);
        src[0] = ra(AC32+0);
#else
    __FP_STORE_TR_AC(tr);
	d = (double *) src;
    *d = tr2d(tr);
#endif
}


// IEEE ARG LOAD/STORE
void __FP_LOAD_IEEE_ARG(U8 *src)
{
    unsigned char tr[TR_SIZE];
    double r;
	double *d;
#ifndef FPBIG
        wa(ARG32+3,src[3]);              // MSB
        wa(ARG32+2,src[2]);
        wa(ARG32+1,src[1]);
        wa(ARG32+0,src[0]);
		_SAVE();
		my_ex_cmd(_IEEE_TO_TRarg);
		_RESTORE();
#else
	d = (double *) src;
    d2tr((double) *d,tr,0);
    __FP_LOAD_TR_ARG(tr);
#endif
}

void __FP_STORE_IEEE_ARG(U8 *src)
{
    unsigned char tr[TR_SIZE];
    double r;
	double *d;
#ifndef FPBIG
		_SAVE();
		my_ex_cmd(_TRarg_TO_IEEE);
		_RESTORE();
        src[3] = ra(ARG32+3);              // MSB
        src[2] = ra(ARG32+2);
        src[1] = ra(ARG32+1);
        src[0] = ra(ARG32+0);
#else
    __FP_STORE_TR_ARG(tr);
	d = (double *) src;
    *d = tr2d(tr);
#endif
}

void __FPac_SWAP()
{
		my_ex_cmd(_FPac_SWAP);
}

void __FParg_ONE()
{
		my_ex_cmd(_FParg_ONE);
}

void __FP_FLOOR()
{
		my_ex_cmd(_FP_FLOOR);
}

void __FP_CEIL()
{
		my_ex_cmd(_FP_CEIL);
}

void __FP_LOG()
{
		my_ex_cmd(_FP_LOG);
}

void __FP_EXP()
{
		my_ex_cmd(_FP_EXP);
}

void __FP_POW()
{
		my_ex_cmd(_FP_POW);
}

void __FP_SQRT()
{
		my_ex_cmd(_FP_SQRT);
}

void __FP_ADD()
{
		my_ex_cmd(_FP_ADD);
}

void __FP_SUB()
{
		my_ex_cmd(_FP_SUB);
}

void __FP_MUL()
{
		my_ex_cmd(_FP_MUL);
}

void __FP_DIV()
{
		my_ex_cmd(_FP_DIV);
}

void __FP_CLR_AC()
{
		my_ex_cmd(_FPac_CLR);
}

void __FP_U32()
{
		my_ex_cmd(_FP_U32);
}

void __I32_FP()
{
		my_ex_cmd(_ITOF32);
}

void __ITOF32()
{
		my_ex_cmd(_ITOF32);
}

void __FP_SPLIT()
{
		my_ex_cmd(_FP_SPLIT);
}

void __FP_FTOA()
{
		my_ex_cmd(_FTOA);
}

void __FP_SIN()
{
		my_ex_cmd(_FP_SIN);
}

void __FP_COS()
{
		my_ex_cmd(_FP_COS);
}

void __FP_TAN()
{
		my_ex_cmd(_FP_TAN);
}

void __FP_ASIN()
{
		my_ex_cmd(_FP_ASIN);
}

void __FP_ACOS()
{
		my_ex_cmd(_FP_ACOS);
}

void __FP_ATAN()
{
		my_ex_cmd(_FP_ATAN);
}


int __FP_CMP()
{
		my_ex_cmd(_FP_CMP);
// FPac > FParg ?
		if(!(status & STATUS_Z) && (status & STATUS_C))
			return(1);
// FPac < FParg ?
		if(!(status & STATUS_Z) && !(status & STATUS_C))
			return(-1); 
// FPac == FParg ?
		if((status & STATUS_Z) && (status & STATUS_C))
			return(0);
}

#define FP_ABS(a) wa(FPac_SIGN,0);
#define FP_NEG(a) wa(FPac_SIGN,0x80 ^ ra(FPac_SIGN));


/* The following subroutines are implementations of the above
/* The following subroutines are implementations of the above
 * named functions, using the native or default arithmetic.
 */
void eadd(FSIZE *a, FSIZE *b, FSIZE *c)
{
#if NATIVE
	*c = *b + *a;
#else
	__FP_LOAD_IEEE_AC((unsigned char *)b);
	__FP_LOAD_IEEE_ARG((unsigned char *)a);
	__FP_ADD();
	__FP_STORE_IEEE_AC((unsigned char *)c);
#endif

}

void esub(FSIZE *a, FSIZE *b, FSIZE *c)
{
#if NATIVE
	*c = *b - *a;
#else
	__FP_LOAD_IEEE_AC((unsigned char *)b);
	__FP_LOAD_IEEE_ARG((unsigned char *)a);
	__FP_SUB();
	__FP_STORE_IEEE_AC((unsigned char *)c);
#endif
}

void emul(FSIZE *a, FSIZE *b, FSIZE *c)
{
#if NATIVE
	*c = (*b) * (*a);
#else
	__FP_LOAD_IEEE_AC((unsigned char *)b);
	__FP_LOAD_IEEE_ARG((unsigned char *)a);
	__FP_MUL();
	__FP_STORE_IEEE_AC((unsigned char *)c);
#endif
}

void ediv(FSIZE *a, FSIZE *b, FSIZE *c)
{
#if NATIVE
	*c = (*b) / (*a);
#else
	__FP_LOAD_IEEE_AC((unsigned char *)b);
	__FP_LOAD_IEEE_ARG((unsigned char *)a);
	__FP_DIV();
	__FP_STORE_IEEE_AC((unsigned char *)c);
#endif
}


/* Important note: comparison can be done by subracting
 * or by a compare instruction that may or may not be
 * equivalent to subtracting.
 */
int ecmp(FSIZE *a, FSIZE *b)
{

#if NATIVE
	if( (*a) > (*b) )
		return( 1 );
	if( (*a) < (*b) )
		return( -1 );
	if( (*a) == (*b) )
		return( 0 );
	return(0);
#else
	int tst;
	__FP_LOAD_IEEE_AC((unsigned char *)a);
	__FP_LOAD_IEEE_ARG((unsigned char *)b);
	tst = __FP_CMP();


	return(tst);
#endif
}


void emov( FSIZE *a, FSIZE *b )
{
#if NATIVE
	*b = *a;
#else
	__FP_LOAD_IEEE_AC((unsigned char *)a);
	__FP_STORE_IEEE_AC((unsigned char *)b);
#endif
}

void eneg( FSIZE *a )
{
#if NATIVE
	*a = -(*a);
#else
	__FP_LOAD_IEEE_AC((unsigned char *)a);
	FP_NEG(__FPac);
	__FP_STORE_IEEE_AC((unsigned char *)a);
#endif
}

void eclear(FSIZE *a)
{
#if NATIVE
	*a = 0.0;
#else
	__FP_CLR_AC();
	__FP_STORE_IEEE_AC((unsigned char *)a);
#endif
}

void eabs(FSIZE *x)
{
if( (*x) < 0.0 )
	*x = -(*x);
#if NATIVE
	if( (*x) < 0.0 )
		*x = -(*x);
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	FP_ABS(__FPac);
	__FP_STORE_IEEE_AC((unsigned char *)x);
#endif
}

void efloor(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) floor( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_FLOOR();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}

void elog(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) log( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_LOG();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}

void esin(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) sin( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_SIN();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}

void ecos(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) cos( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_COS();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}

void etan(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) tan( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_TAN();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}

void easin(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) asin( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_ASIN();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}


void eacos(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) acos( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_ACOS();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}

void eatan(FSIZE *x, FSIZE *y)
{

#if NATIVE
	double xp = *x;
	*y = (FSIZE) atan( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_ATAN();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}

void epow(FSIZE *x, FSIZE *y, FSIZE *z)
{
	double xp = *x;
	double yp = *y;

#ifdef POW_BUG
	if(yp == (FSIZE) 0.0) {	/* Bug */
		*z = 1;
		return; 
	}
#endif

#if NATIVE 
	*z = (FSIZE) pow( xp, yp );
#else
	if(*y == (FSIZE) 0.0) {	/* Bug */
		*z = (FSIZE) 1.0;
#if DEBUG
printf("POW: X: %.10e, Y: %.10e, Z:%.10e\n", (double) *x, (double) *y, (double) *z);
#endif
		return; 
	}
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_LOAD_IEEE_ARG((unsigned char *)y);
	__FP_POW();
	__FP_STORE_IEEE_AC((unsigned char *)z);
#endif

#if DEBUG
printf("POW: X: %.10e, Y: %.10e, Z:%.10e\n", (double) *x, (double) *y, (double) *z);
#endif
}


void esqrt(FSIZE *x, FSIZE *y)
{
	double xp = *x;

#if NATIVE
	*y = (FSIZE) sqrt( xp );
#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	__FP_SQRT();
	__FP_STORE_IEEE_AC((unsigned char *)y);
#endif
}


void eifrac(FSIZE *x,long *i, FSIZE *f)
{

#if NATIVE	/* Seems to split ABS INT/FRAC parts into i/f */
	FSIZE y;
	double xp = *x;
	y = (FSIZE) floor( xp );
	if( y < 0.0 )
		{
		*f = y - *x;
		*i = -y;
		}
	else
		{
		*f = *x - y;
		*i = y;
		}

#else
	__FP_LOAD_IEEE_AC((unsigned char *)x);
	FP_ABS(__FPac);
	__FP_SPLIT();
	__FP_U32();
	__AC32_STORE((U8 *)i);
	__FP_STORE_IEEE_ARG((unsigned char *)f);
#endif
#if DEBUG
	printf("EIFRAC: X: %.10e, %ld, %.10e\n", 
		(double) *x, (long) *i, (double ) *f);
#endif
}


void ltoe(long *i, FSIZE *x)
{
#if NATIVE	
	*x = *i;
#else
	__AC32_LOAD((U8 *)i);
	__I32_FP();
	__FP_STORE_IEEE_AC((unsigned char *)x);
#endif
}



void einit(void)
{
	my_ex_cmd(_PIC_INIT);
}

// ========================================================================
// ========================================================================
// Double functions
double dsin(double d)
{
	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_SIN();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double dcos(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_COS();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double dtan(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_TAN();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double dasin(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_ASIN();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double dacos(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_ACOS();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double datan(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_ATAN();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double dlog(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_LOG();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double dexp(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_EXP();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double dpow(double base, double pow)
{

	unsigned char tr_base[TR_SIZE];
	unsigned char tr_pow[TR_SIZE];
	unsigned char tr_result[TR_SIZE];
	double r;
	d2tr(base,tr_base,0);
	d2tr(pow,tr_pow,0);
    __FP_LOAD_TR_AC(tr_base);
    __FP_LOAD_TR_ARG(tr_pow);
    __FP_POW();
    __FP_STORE_TR_AC(tr_result);
	r = tr2d(tr_result);
	return(r);
}

double dsqrt(double d)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
    __FP_SQRT();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}

double deifrac(double d, long *i)
{

	unsigned char tr[TR_SIZE];
	double r;
	d2tr(d,tr,0);
    __FP_LOAD_TR_AC(tr);
	FP_ABS(__FPac);
	__FP_SPLIT();
	__FP_U32();
	__AC32_STORE((U8 *)i);
    __FP_STORE_TR_ARG(tr);
	r = tr2d(tr);
	return(r);
}

double ltod(long i)
{

	unsigned char tr[TR_SIZE];
	double r;
	__AC32_LOAD((U8 *)i);
	__I32_FP();
    __FP_STORE_TR_AC(tr);
	r = tr2d(tr);
	return(r);
}


