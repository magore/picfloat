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
 * $Id: main.c,v 1.2 2004/01/28 02:35:42 magore Exp $
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "pic.h"
#ifdef VIDEO
#include "display.h"
#endif

extern char *progname;

// 2002,2003 Mike Gore
// Modified to allow external programs to run the simulator
// Do not compile main() if an external program will controll the emulator

// Dummy instruction trap handler
void my_trap(unsigned int pc) 
{
}

int main (int argc, char *argv [])
{
  progname = argv [0];

  if (argc != 2)

    fatal (1, NULL);
// Load file to simulate
  pic_init(argv[1]);

// moved simulator loop into command called sim_pic()
  sim_pic();

  exit (0);
}


