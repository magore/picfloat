//;File: prtnum.c
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
#include "prtnum.h"
#include "cvtnum.h"

int parse(int argc, char *argv[], char *msg, char *num)
{
	int i;
	char *ptr;
	int flag = 0;
	int mflag = 0;
	int nflag = 0;

	for(i=1;i<argc;++i) {
		ptr = argv[i];
		if(strcmp(ptr,"-c") == 0)
			flag |= N_C_FLAG;
		else if(strcasecmp(ptr,"-e") == 0)
			flag |= N_EXT;
		else if(strcasecmp(ptr,"-t") == 0)
			flag |= N_TR;
		else if(strcasecmp(ptr,"-p") == 0)
			flag |= N_PIC;
		else if(strcasecmp(ptr,"-q") == 0)
			flag |= N_EQ;
		else if(*ptr == '-') {
			printf("usage: %s -Extern -Temp_real -Pic -eQuate -Cmode\n",
				argv[0]);
			return(-1);
		}
		else if(mflag == 0) {
			mflag = 1;
			strcpy(msg,ptr);
		}
		else if(nflag == 0) {
			nflag = 1;
			strcpy(num,ptr);
		}
	}
	return(flag);
}
	
	
void prtnum(double din, char *msg, int flag)
{
		int i,ind;
		unsigned char out[TR_SIZE];
		unsigned char *fp;
		double rem,delta,d;
		float f;

		// Single IEEE FMT; fp[3] = MSB,EXP, SIGN, fp[0] = LSB
		f = din;
		fp = (unsigned char *) &f;

		// Double IEEE convert double to TR
		d2tr(din,out,flag);
		// Convert TR to double IEEE 
		d = tr2d(out);
		// Compare value to check conversion loss	
		delta = d - din;


		if(flag & N_EXT) {
				if(flag & N_PIC) {
						printf(";%s\t; %.10e,residual: %.10e\n",
							msg,din,delta);
						return;
				}
				if(flag & N_TR)  {
						printf("extern rom unsigned char _TR_%s[]; ",msg);
						printf(" // %.10e, residual: %.10e\n", din,delta);
						return;
				}
				printf("extern rom unsigned char _TR_%s[]; ",msg);
				printf(" // %.10e, residual: %.10e\n", din,delta);
				return;
		}

		if(flag & N_TR) {
			if(flag & N_PIC) {
				// PIC TEMP REAL, LSB .. MSB, EXP, SIGN
					ind = 0;
					printf("%s\t; %.10e (temp real) residual: %.10e:\n",
						msg,din,delta);
					for(i=0;i<TR_SIZE;++i) 
						printf("\tretlw\t0x%02x ; (%d) %s\n",
							out[i],i, tr_msg[i]);
					printf("\n");
					return;
			}
			if(flag & N_C_FLAG) {
				// C TEMP REAL, LSB .. MSB, EXP, SIGN
					printf("rom unsigned char _TR_%s[]=\t{ ",msg);
					for(i=0;i<TR_SIZE;++i) {
						if(i == 0)
							printf("0x%02x", out[i]);
						else
							printf(",0x%02x", out[i]);
					}
					printf(" }; // %.10e, residual: %.10e\n", 
						din,delta);
					return;
			}
			else {
				// 6805 TEMP REAL, EXP, SIGN, MSB .. LSB
				// format is Backwards
					printf("rom unsigned char _TR_%s[]=\t{ ",msg);
					for(i=TR_EXP;i>= 0;--i) {
						if(i == TR_EXP)
							printf("0x%02x", out[i]);		// EXP
						else
							printf(",0x%02x", out[i]);
					printf(" }; // %.8e\n", din);
					return;
			}
		}
	}
	else {
// IEEE format numbers
			if(flag & N_EQ) {
				// 6805 REAL, EXP, SIGN, MSB .. LSB
				// format is Backwards
					ind = 0;
					printf("%s\tequ	0x",msg);
					printf("%02x",fp[3]);
						printf("%02x",fp[2]);
						printf("%02x",fp[1]);
						printf("%02x",fp[0]);
						printf(";\t%.10e (32bit IEEE)\n",din);
						return;
				}

				if(flag & N_PIC) {
						// PIC IEEE, LSB .. MSB, SIGN:EXP
						ind = 0;
						printf("%s\t; %.10e (32bit IEEE)\n",msg,din);
						printf("\tretlw\t0x%02x ; (%d) LSB\n",fp[0],0);
						printf("\tretlw\t0x%02x ; (%d)\n",fp[1],1);
						printf("\tretlw\t0x%02x ; (%d)\n",fp[2],2);
						printf("\tretlw\t0x%02x ; (%d) MSB, EXP,SIGN\n",fp[3],3);
						printf("\n");
						return;
				}
				if(flag & N_C_FLAG) {
					printf("rom unsigned char _%s[]=\t{ ",msg);
					printf("0x%02x,0x%02x,0x%02x,0x%02x", 
							fp[0], fp[1], fp[2], fp[3]);
					printf(" }; // %.10e\n", din);
						return;
				}
				// SIGN:EXP MSB .. LSB
				printf("rom unsigned char _%s[]=\t{ ",msg);
				printf("0x%02x,0x%02x,0x%02x,0x%02x", 
						fp[3], fp[2], fp[1], fp[0]);
				printf(" }; // %.10e\n", din);
				return;
		}

}
