/*
 * Copyright 1997, 2000 Eric Smith <brouhaha@users.sourceforge.net>
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
 * $Id: picdis.c,v 1.1.1.1 2003/12/14 03:40:06 magore Exp $
 */

#include <stdio.h>
#include "pic.h"


char *disassemble_inst (word inst, char *buffer)
{
  int reg;
  char *mnem = "???";
  char opbuf [10];
  char *operand = & opbuf [0];
  int suffix = 0;

  opbuf [0] = '\0';
  switch (inst >> 12)
    {
    case 0:  /* operate instructions */
      reg = inst & 0x7f;
      suffix = 1;
      switch ((inst >> 8) & 0xf)
	{
	case 0x0:  /* special */
	  suffix = 0;
	  if (inst & 0x80)
	    mnem = "movwf";
	  else
	    {
	      switch (inst & 0x7f)
		{
		case 0x00:  mnem = "nop";    reg = -1; break;
		case 0x08:  mnem = "return"; reg = -1; break;
		case 0x09:  mnem = "retfie"; reg = -1; break;
#ifdef SIM_INST_SET
		case 0x0b:  mnem = "output"; reg = -1; break;
		case 0x0c:  mnem = "mark";   reg = -1; break;
#endif /* SIM_INST_SET */
		case 0x62:  mnem = "option"; reg = -1; break;
		case 0x63:  mnem = "sleep";  reg = -1; break;
		case 0x64:  mnem = "clrwdt"; reg = -1; break;
		case 0x65:  mnem = "tris";   reg = 5; break;
		case 0x66:  mnem = "tris";   reg = 6; break;
		case 0x67:  mnem = "tris";   reg = 7; break;
		}
	    }
	  break;
	case 0x1:
	  suffix = 0;
	  if (inst & 0x80)
	    mnem = "clrf";
	  else
	    {
	      mnem = "clrw";
	      reg = -1;
	    }
	  break;
	case 0x2:  mnem = "subwf";  break;
	case 0x3:  mnem = "decf";   break;
	case 0x4:  mnem = "iorwf";  break;
	case 0x5:  mnem = "andwf";  break;
	case 0x6:  mnem = "worwf";  break;
	case 0x7:  mnem = "addwf";  break;
	case 0x8:  mnem = "movf";   break;
	case 0x9:  mnem = "comf";   break;
	case 0xa:  mnem = "incf";   break;
	case 0xb:  mnem = "decfsz"; break;
	case 0xc:  mnem = "rrf";    break;
	case 0xd:  mnem = "rlf";    break;
	case 0xe:  mnem = "swapf";  break;
	case 0xf:  mnem = "incfsz"; break;
	}
      if (reg >= 0)
	{
	  if (suffix)
	    sprintf (operand, "%02x,%c", reg & 0x7f,
		     inst & 0x80 ? 'f' : 'w');
	  else
	    sprintf (operand, "%02x", reg & 0x7f);
	}
      break;
    case 1:  /* bit instructions */
      switch ((inst >> 10) & 3)
	{
	case 0:  mnem = "bcf"; break;
	case 1:  mnem = "bsf"; break;
	case 2:  mnem = "btfsc"; break;
	case 3:  mnem = "btfss"; break;
	}
      sprintf (operand, "%02x,%d", inst & 0x7f,
	       (inst >> 7) & 7);
      break;
    case 2:  /* branch/call instructions */
      mnem = (inst & 0x0800) ? "goto" : "call";
      sprintf (operand, "%04x", inst & 0x07ff);
      break;
    case 3:  /* literal instructions */
      switch ((inst >> 8) & 0xf)
	{
	case 0x0:  mnem = "movlw"; break;
	case 0x4:  mnem = "retlw"; break;
	case 0x8:  mnem = "iorlw"; break;
	case 0x9:  mnem = "andlw"; break;
	case 0xa:  mnem = "xorlw"; break;
	case 0xc:  mnem = "sublw"; break;
	case 0xe:  mnem = "addlw"; break;
	}
      sprintf (operand, "%02x", inst & 0xff);
    }

  sprintf (buffer, "%-6s  %-4s", mnem, operand);
  return (buffer);
}
