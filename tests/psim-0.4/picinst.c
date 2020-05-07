/*
 * Copyright 1996, 1997, 2000 Eric Smith <brouhaha@users.sourceforge.net>
 *
 * PSIM is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.  Note that I am not granting permission to
 * redistribute or modify PSIM under the terms of any later version of
 * the General Public License.
 *
 * This program is distributed in the hope that it will be useful (or at least
 * amusing), but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program (in the file "COPYING"); if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: picinst.c,v 1.1.1.1 2003/12/14 03:40:06 magore Exp $
 */

#include <stdio.h>
#include "pic.h"


#define SET_STATUS(flag,val) \
if (val) \
  status |= STATUS_##flag; \
else \
  status &= ~ STATUS_##flag;


void exec_inst (void)
{
  word inst;
  int reg;
  byte arg;
  byte mask;
  word result;
  romaddr prevpc;

      inst = read_rom (pc);
      prevpc = pc;
      pc = nextpc;
      nextpc = pc + 1;
      if (nextpc >= MAXROM)
		nextpc = 0;
      cycle++;
      run_tmr0 ();
      if (skip_flag)
	skip_flag = 0;
      else
	{
	  skip_flag = 0;
	  set_rom_flag (prevpc, ROM_FLAG_EXECUTED);
	  switch (inst >> 12)
	    {
	    case 0:  /* operate instructions */
	      if (inst < 0x80)
		{
		  switch (inst)
		    {
		    case 0x00:  /* nop */
		      break;  /* no status changes */
		    case 0x08:  /* return */
		      nextpc = pop ();  /* no status changes */
		      skip_flag = 1;
		      break;
		    case 0x09:  /* retfie */
		      nextpc = pop ();  /* no status changes */
		      skip_flag = 1;
		      intcon |= INTCON_GIE;
		      break;
#ifdef SIM_INST_SET
		    case 0x0b:  /* output */
		      printf ("%c", w);
		      break;
		    case 0x0c:  /* mark */
		      printf ("\nTiming mark at cycle %lu\n", cycle);
		      break;
#endif /* SIM_INST_SET */
		    case 0x62:  /* option */
		      option = w;
		      break;
		    case 0x64:  /* clrwdt */
		      /* no status changes */
		      break;
		    case 0x65:  /* tris porta */
		      trisa = w;  /* no status changes */
		      break;
		    case 0x66:  /* tris portb */
		      trisb = w;  /* no status changes */
		      break;
		    case 0x63:  /* sleep */
		    case 0x67:  /* tris portc */
		    default:
		      {
			printf ("unimplemented instruction\n");
			dobreak ();
		      }
		    }
		  break;
		}
	      reg = inst & 0x7f;
	      arg = read_reg_direct (reg);
	      switch ((inst >> 8) & 0xf)
		{
		case 0x0:  /* movwf (specials were handled above) */
		  result = w;  /* no status changes */
		  break;
		case 0x1:  /* clrf, clrw */
		  result = 0;
		  status |= STATUS_Z;
		  break;
		case 0x2:  /* subwf */
		  result = (arg & 0xf) + ((w & 0xf) ^ 0xf) + 1;
		  SET_STATUS(DF, result & 0x10)
		  result = arg + (w ^ 0xff) + 1;
		  SET_STATUS(C, result & 0x100)
		  result &= 0xff;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0x3:  /* decf */
		  result = (arg - 1) & 0xff;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0x4:  /* iorwf */
		  result = arg | w;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0x5:  /* andwf */
		  result = arg & w;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0x6:  /* xorwf */
		  result = arg ^ w;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0x7:  /* addwf */
		  result = (arg & 0xf) + (w & 0xf);
		  SET_STATUS(DF, result & 0x10)
		  result = arg + w;
		  SET_STATUS(C, result & 0x100)
		  result &= 0xff;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0x8:  /* movf */
		  result = arg;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0x9:  /* comf */
		  result = arg ^ 0xff;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0xa:  /* incf */
		  result = (arg + 1) & 0xff;
		  SET_STATUS(Z, result == 0)
		  break;
		case 0xb:  /* decfsz */
		  result = (arg - 1) & 0xff;  /* no status changes */
		  skip_flag = (result == 0x00);
		  break;
		case 0xc:  /* rrf */
		  result = (arg >> 1);
		  if (status & STATUS_C)
		    result |= 0x80;
		  SET_STATUS(C, arg & 0x01)
		  break;
		case 0xd:  /* rlf */
		  result = (arg << 1);
		  if (status & STATUS_C)
		    result |= 0x01;
		  SET_STATUS(C, result & 0x100)
		  result &= 0xff;
		  break;
		case 0xe:  /* swapf */
		  result = ((arg >> 4) | (arg << 4)) & 0xff;  /* no status changes */
		  break;
		case 0xf:  /* incfsz */
		  result = arg + 1;  /* no status changes */
		  skip_flag = (result == 0x100);
		  result &= 0xff;
		  break;
		}
	      if (inst & 0x80)
		write_reg_direct (reg, result);
	      else
		w = result;
	      break;
	    case 1:  /* bit instructions */
	      mask = 1 << ((inst >> 7)  & 7);
	      reg = inst & 0x7f;
	      arg = read_reg_direct (reg);
	      switch ((inst >> 10) & 3)
		{
		case 0:  /* bcf */
		  write_reg_direct (reg, arg & ~ mask);
		  break;
		case 1:  /* bsf */
		  write_reg_direct (reg, arg | mask);
		  break;
		case 2:  /* btfsc */
		  if (! (arg & mask))
		    skip_flag = 1;
		  break;
		case 3:  /* btfss */
		  if (arg & mask)
		    skip_flag = 1;
		  break;
		}
	      break;
	    case 2:  /* branch/call instructions */
	      if (! (inst & 0x0800))
		push (pc);
	      nextpc = ((pclath << 8) & 0x1800) | (inst & 0x07ff);
	      if (nextpc == (pc - 1))
		{
		  printf ("halted\n");
		  run = 0;
		  step = 0;
		}
	      else
		skip_flag = 1;
	      break;
	    case 3:  /* literal instructions */
	      switch ((inst >> 8) & 0xf)
		{
		case 0x0:  /* movlw */
		  w = inst & 0xff;  /* no status changes */
		  break;
		case 0x4:  /* retlw */
		  w = inst & 0xff;  /* no status changes */
		  nextpc = pop ();
		  skip_flag = 1;
		  break;
		case 0x8:  /* iorlw */
		  w |= (inst & 0xff);
		  SET_STATUS(Z, w == 0)
		  break;
		case 0x9:  /* andlw */
		  w &= (inst & 0xff);
		  SET_STATUS(Z, w == 0)
		  break;
		case 0xa:  /* xorlw */
		  w ^= (inst & 0xff);
		  SET_STATUS(Z, w == 0)
		  break;
		case 0xc:  /* sublw */
		  result = (inst & 0xf) + ((w & 0xf) ^ 0xf) + 1;
		  SET_STATUS(DF, result & 0x10)
		  result = (inst & 0xff) + (w ^ 0xff) + 1;
		  SET_STATUS(C, result & 0x100)
		  w = result & 0xff;
		  SET_STATUS(Z, w == 0)
		  break;
		case 0xe:  /* addlw */
		  result = (inst & 0x0f) + (w & 0x0f);
		  SET_STATUS(DF, result & 0x10)
		  result = (inst & 0xff) + w;
		  SET_STATUS(C, result & 0x100)
		  w = result & 0xff;
		  SET_STATUS(Z, w == 0)
		  break;
		default:
		  {
		    printf ("unimplemented instruction\n");
		    dobreak ();
		  }
		}
	      break;
	    }
	}
}
