//;File: ieee2tr.c
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
#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
	int j;
	unsigned char *np;
	float fpout;
	

	unsigned int a0, a1,a2,a3,a4;
	unsigned int c;
	char line[256];

	np = (unsigned char *) &fpout;


	fgets(line,sizeof(line)-2,stdin);

	sscanf(line,"%x %x %x %x", &a3,&a2,&a1,&a0);
	np[3] = a3;
	np[2] = a2;
	np[1] = a1;
	np[0] = a0;

	c = (np[3] & 1) << 7;
	np[3] >>= 1;
	np[2] &= 0x7f;
	np[2] |= c;

	printf("%e\n",(double) fpout);

        printf("rom unsigned char %s[]=\t{ ","xx");
        printf("0x%02X,0x%02X,0x%02X,0x%02X", np[3], np[2], np[1], np[0]);
        printf(" }; // %.8e\n", fpout);
        printf("rom unsigned char %s[]=\t{ ","zz");
        printf("0x%02X,0x%02X,0x%02X,0x%02X,0x%02X",
                np[3] & 0x80,
                ((np[3] & 0x7f) << 1) | (np[2] & 0x80) >> 7,
                0x80 | np[2], np[1], np[0]);
        printf(" }; // %.8e\n", fpout);

}
