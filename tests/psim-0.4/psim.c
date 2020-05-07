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
 * $Id: psim.c,v 1.2 2004/01/22 02:14:51 magore Exp $
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "pic.h"
#ifdef VIDEO
#include "display.h"
#endif


char *progname;

void print_usage (FILE *f)
{
  fprintf (f, "Usage: %s <hexfile>\n", progname);
}


void fatal (int retval, char *fmt, ...)
{
  va_list ap;

  if (retval == 1)
    print_usage (stderr);
  else
    {
      fprintf (stderr, "%s: ", progname);
      va_start (ap, fmt);

      vfprintf (stderr, fmt, ap);
      va_end (ap);
    }
  exit (retval);
}


char disbuf [20];


void show_status (void)
{
  printf ("%6lu  ", cycle);
  if (skip_flag)
    strcpy (disbuf, "pipeline stall");
  else
    {
      sprintf (disbuf, "%04x: ", pc);
      disassemble_inst (read_rom (pc), & disbuf [strlen (disbuf)]);
    }
  printf ("%-20s  W=%02x  S=%c%c%c%c%c%c%c%c  TMR0=%02x",
	  disbuf,
	  (int) w,
	  (status & STATUS_IRP) ? 'I' : '.',
	  (status & STATUS_RP1) ? '1' : '.',
	  (status & STATUS_RP0) ? '0' : '.',
	  (status & STATUS_TO ) ? 'T' : '.',
	  (status & STATUS_PD ) ? 'P' : '.',
	  (status & STATUS_Z  ) ? 'Z' : '.',
	  (status & STATUS_DF ) ? 'D' : '.',
	  (status & STATUS_C  ) ? 'C' : '.',
	  (int) tmr0);
#ifdef VIDEO
  printf ("  portb=%02x  pos=%3d,%3d", portb, line, hpos);
#endif
  printf ("\n");
}

// 2002,2003 Mike Gore
// Move code from main()
void pic_init(char *name)
{
	init_rom ();

	if (! load_rom_file (name))
		fatal (2, "can't read hex file\n");

#ifdef VIDEO
   init_display (argc, argv, "PSIM");
 #endif


	stack_ind = 0;
	pc = 0;
	nextpc = 1;
// Added next 6 by lines - to allow external programs to run the simulator
	stack_ind = 0;	// Have we return to the same stack level ?
	max_stack_ind = 0;	// Maximum Stack Depth
	skip_flag = 0;
	ex_flag = 0;
	trace = 0;
	run = 0;
	step = 0;

	init_reg ();
	init_debug ();
}


sim_pic()
{
  while (! quit)
    {
      while (run || step)
	{
	  if (step)
	    step--;
	  if ((! skip_flag) && (! ignore_break) && test_rom_flag (pc, ROM_FLAG_BREAKPOINT))
	    {
	      printf ("breakpoint reached at %04x\n", pc);
	      run = 0;
	      step = 0;
	    }
	  else
	    {
	      ignore_break = 0;
	      exec_inst ();
#ifdef VIDEO
	      display_pixel (! (portb & 0x40), ! (portb & 0x20), portb & 0x01);
#endif
	      if (trace && (run || step))
		show_status ();
	    }
	}
      show_status ();
      debug ();
    }

}

