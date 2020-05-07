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
 * $Id: pictmr0.c,v 1.1.1.1 2003/12/14 03:40:06 magore Exp $
 */

#include <stdio.h>
#include "pic.h"


unsigned int prescaler;
byte tmr0;
unsigned int tmr0_noinc = 0;

READFN(tmr0)


void tmr0_wr (byte addr, byte data)
{
  tmr0 = data;
  if ((option & OPTION_PSA) == 0)
    prescaler = 0;
  tmr0_noinc = 2;
}


void run_tmr0 (void)
{
  /* $$$ currently assumes internal clock! */

  if ((option & OPTION_PSA) == 0)
    {
      prescaler++;
      if (prescaler == (1 << ((option & 7) + 1)))
	prescaler = 0;
      else
	{
	  if (tmr0_noinc)
	    tmr0_noinc--;
	  return;
	}
    }
  if (tmr0_noinc)
    {
      tmr0_noinc--;
      return;
    }
  tmr0++;
  if (tmr0 == 0)
    intcon |= INTCON_T0IF;
}

