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
 * $Id: picio.c,v 1.1.1.1 2003/12/14 03:40:06 magore Exp $
 */


#include <stdio.h>
#include "pic.h"


int buttonl, buttonr;


byte porta = 0xff;
byte trisa;
byte portb = 0xe0;
byte trisb;

READFN(trisa)
WRITEFN(trisa)

READFN(trisb)
WRITEFN(trisb)


byte porta_rd (byte addr)
{
  return ((buttonl ? 0x00 : 0x04) | (buttonr ? 0x00 : 0x08));
}

void porta_wr (byte addr, byte data)
{
  ;
}

READFN (portb);
void portb_wr (byte addr, byte data)
{
  portb = data | 0x80;
}


