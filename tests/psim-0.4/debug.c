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
 * $Id: debug.c,v 1.3 2004/01/28 02:35:42 magore Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>

#ifdef USE_READLINE
#include <readline/readline.h>
#endif
#ifdef USE_HISTORY
#include <readline/history.h>
#endif

#define MAX_ARGS 16


#include "pic.h"


int trace = 0;
int run = 0;
int step = 0;
int quit = 0;
int ignore_break = 0;
int ex_flag = 0;	// Added by Mike Gore


static char hex_digits [] = "0123456789abcdef";


/*
 * strtol() doesn't complain about leftovers, so I use my own
 *
 * This is really only intended for input of unsigned longs; it returns -1
 * if the string isn't entirely composed of hex digits.
 */

static long hex_strtol (char *s)
{
  long val = 0;
  char *p;

  if (! *s)
    return (-1);  /* must have at least one digit! */

  while (*s)
    {
      p = strchr (hex_digits, tolower (*s));
      if (! p)
	return (-1);
      val = (val << 4) + (p - & hex_digits [0]);
      s++;
    }
  return (val);
}


static int parse_address (char *s)
{
  long val;

  val = hex_strtol (s);
  if ((val >= 0) && (val <= 0xffff))
    return (val);

  return (-1);
}


static int parse_byte (char *s)
{
  long val;

  val = hex_strtol (s);
  if ((val >= 0) && (val <= 0xff))
    return (val);

  return (-1);
}


static void dump_range (word addr1, word addr2)
{
  word addr;
  byte data;

  for (; addr1 <= addr2; addr1 += 16)
    {
      printf ("%04x: ", addr1);
      for (addr = addr1; addr < (addr1 + 16); addr++)
	{
	  if (addr <= addr2)
	    {
	      data = read_reg_abs (addr);
	      printf ("%02x ", data);
	    }
	  else
	    printf ("   ");
	}
      printf ("\n");
    }
}


static void show_mem (word addr)
{
  byte val;
  val = read_reg_abs (addr);
  printf("reg[%03x]: %02x\n", addr, val);
}


static void list_breakpoints (void)
{
  int i;
	
  for (i=0; i < MAXROM; i++) 
    {
      if (test_rom_flag (i, ROM_FLAG_BREAKPOINT))
	printf ("%04x\n", i);
    }
}


static int break_cmd (int argc, char *argv[])
{
  int addr;

  if ((argc < 1) || (argc > 2))
    return (-1);

  if (argc == 1)
    {
      list_breakpoints ();
      return (0);
    }

  addr = parse_address (argv [1]);
  if ((addr < 0) || (addr >= MAXROM))
    return (-1);

  set_rom_flag (addr, ROM_FLAG_BREAKPOINT);

  return (0);
}


static int clear_cmd (int argc, char *argv[])
{
  int addr;

  if ((argc < 1) || (argc > 2))
    return (-1);

  if (argc == 1)
    {
      for (addr = 0; addr < MAXROM; addr++)
	clr_rom_flag (addr, ROM_FLAG_BREAKPOINT);
      return (0);
    }

  addr = parse_address (argv [1]);

  if ((addr < 0) || (addr > MAXROM))
    return (-1);

  clr_rom_flag (addr, ROM_FLAG_BREAKPOINT);

  return (0);
}


// =======================================================================
// Added to allow external programs to run the simulator
// 2002,2003 Mike Gore
// my_ex_cmd( addr ) 
// 	Similate code starting at addr until a return 
// 	(at same stack level) is reached
int broke = 0;
void my_ex_cmd (int addr)
{
	int wd;

	broke = 1;
	nextpc = addr+1;
	pc = addr;
	stack_init();
	skip_flag = 0;
	ex_flag = 1;
	push(addr);	// Call function
	cycle = 0;
	step = 0;
	trace = 0;
	run = 1;
	ignore_break = 0;
	wd = 0;
	// printf("start: pc:%04x, nextpc:%04x, cycle:%ld\n",pc,nextpc,cycle);
	while(run) {
		my_trap(pc);	// Trap certain addressed
		if(++wd > 10000000) {
			run = 0;
			trace = 0;
			step = 0;
			printf("Stuck\n");;
			show_status();
			exit(1);
		}
		if(step)
			--step;
		
		exec_inst();
		if(!ex_flag)
			run = 0;
	}
	// printf("stop: pc:%04x, nextpc:%04x, cycle:%ld\n",pc,nextpc,cycle);
	broke = 0;
}

// Added to allow external programs to run the simulator
// 2002,2003 Mike Gore
// ex_cmd
static int ex_cmd (int argc, char *argv[])
{
  if (argc != 2)
		return (-1);
	nextpc = parse_address (argv [1]);
	my_ex_cmd(nextpc);
  return (0);
}
// =======================================================================



static int go_cmd (int argc, char *argv[])
{
  if (argc < 1)
    return (-1);

// Added to allow external programs to run the simulator
// 2002,2003 Mike Gore
// ex_cmd
  if(argc == 2)
		nextpc = parse_address (argv [1]);
  step = 0;
  trace = 0;
  run = 1;
  ignore_break = 1;

  return (0);
}


static int step_cmd (int argc, char *argv[])
{
  if ((argc < 1) || (argc > 2))
    return (-1);

  step = (argc == 2) ? atoi (argv [1]) : 1;
  trace = 1;
  run = 0;
  ignore_break = 1;

  return (0);
}


static int trace_cmd (int argc, char *argv[])
{
  if (argc != 1)
    return (-1);

  step = 0;
  trace = 1;
  run = 1;
  ignore_break = 1;

  return (0);
}


int next_addr = 0;

static int dump_cmd (int argc, char *argv[])
{
  int first, last;

  if ((argc < 1) || (argc > 3))
    return (-1);

  first = next_addr;
  if (argc >= 2)
    first = parse_address (argv [1]);
  last = first + 255;
  if (argc == 3)
    last = parse_address (argv [2]);

  if ((first < 0) || (last < first))
    return (-1);

  dump_range (first, last);

  next_addr = last + 1;

  return (0);
}


static int read_cmd (int argc, char *argv[])
{
  int addr;

  if (argc != 2)
    return (-1);

  addr = parse_address (argv [1]);

  if (addr < 0)
    return (-1);

  show_mem (addr);

  return (0);
}


static int write_cmd (int argc, char *argv[])
{
  int addr, val;

  if (argc != 3)
    return (-1);

  addr = parse_address (argv [1]);

  if (addr < 0)
    return (-1);

  val = parse_byte (argv [2]);

  if (val < 0)
    return (-1);

  write_reg_abs (addr, val);
  show_mem (addr);

  return (0);
}


static int quit_cmd (int argc, char *argv[])
{
  quit = 1;
  return (0);
}


static int xyzzy_cmd (int argc, char *argv [])
{
  printf ("Nothing happens here.\n");
  return (0);
}


static int adc_cmd (int argc, char *argv[])
{
  int chan;

  if ((argc < 2) || (argc > 3))
    return (-1);

  chan = parse_byte (argv [1]);
  if ((chan < 0) || (chan > 3))
    return (-1);

  if (argc == 2)
    printf ("%02x\n", adc [chan]);
  else
    adc [chan] = parse_byte (argv [2]);

  return (0);
}


static int press_cmd (int argc, char *argv[])
{
  if (argc != 2)
    return (-1);
  if (strcasecmp (argv [1], "l") == 0)
    buttonl = 1;
  else if (strcasecmp (argv [1], "r") == 0)
    buttonr = 1;
  else
    return (-1);
  return (0);
}


static int rel_cmd (int argc, char *argv[])
{
  if (argc != 2)
    return (-1);
  if (strcasecmp (argv [1], "l") == 0)
    buttonl = 0;
  else if (strcasecmp (argv [1], "r") == 0)
    buttonr = 0;
  else
    return (-1);
  return (0);
}


#define WORDS_PER_LINE 64
#define WORDS_PER_GROUP 16

static int coverage_cmd (int argc, char *argv[])
{
  int i, j;
  int any_loaded;
  int lastaddr = -1;
  if (argc != 1)
    return (-1);
  for (i = 0; i < MAXROM; i+= WORDS_PER_LINE)
    {
      any_loaded = 0;
      for (j = i; j < (i + WORDS_PER_LINE); j++)
	any_loaded = any_loaded | test_rom_flag (j, ROM_FLAG_LOADED);
      if (any_loaded)
	{
	  if (i != (lastaddr + WORDS_PER_LINE))
	    printf ("...\n");
	  lastaddr = i;
	  printf ("%04x:", i);
	  for (j = i; j < (i + WORDS_PER_LINE); j++)
	    {
	      if (! (j % WORDS_PER_GROUP))
		printf (" ");
	      if (j < MAXROM)
		{
		  if (! test_rom_flag (j, ROM_FLAG_LOADED))
		    printf (".");
		  else if (! test_rom_flag (j, ROM_FLAG_EXECUTED))
		    printf ("-");
		  else
		    printf ("*");
		}
	    }
	  printf ("\n");
	}
    }
  return (0);
}


static int help_cmd (int argc, char *argv[]);


typedef int (*cmd_handler)(int argc, char *argv[]);

typedef struct
{
  char *name;
  cmd_handler handler;
  int min_chr;
  char *usage;
} cmd_entry;

cmd_entry cmd_table [] =
{
  { "adc",   adc_cmd,       1, "ADC <chan> [<data>]             list or set A/D input\n" },
  { "break", break_cmd,     1, "Break [<addr> [r|w]]            list or set breakpoints\n" },
  { "clear", clear_cmd,     1, "Clear [<addr>]                  clear breakpoints\n" },
  { "coverage", coverage_cmd, 2, "COverage                        code coverage\n" },
#if 0
  { "disassemble", dis_cmd, 2, "DIsassemble [<addr> [<addr>]]   disassemble from addr\n" },
  { "dump", dump_cmd,       1, "Dump [<addr> [<addr>]]          dump memory\n" },
#endif
  { "go", go_cmd,           1, "Go                              go\n" },
  { "help", help_cmd,       1, "Help                            list commands\n" },
// Added to allow external programs to run the simulator
// 2002,2003 Mike Gore
// Added "ex" - execute function call in simulator until matching return
  { "ex", ex_cmd,           1, "EXecute							Execute fuction\n" },
  { "press", press_cmd,     1, "Press <button>                  press button\n" },
  { "quit", quit_cmd,       4, "QUIT                            quit simulator\n" },
  { "read", read_cmd,       1, "Read <addr>                     read memory at addr\n" },
  { "release", rel_cmd,     3, "RELease <button>                release button\n" },
  { "step", step_cmd,       1, "Step [count]                    single-step one instruction\n" },
  { "trace", trace_cmd,     1, "Trace                           go in trace mode\n" },
  { "write", write_cmd,     1, "Write <addr> <val>              write val to memory at addr\n" },
  { "xyzzy", xyzzy_cmd,     1, "Xyzzy\n" },
  { NULL, NULL, 0, NULL }
};


static int find_cmd (char *s)
{
  int i;
  int len = strlen (s);

  for (i = 0; cmd_table [i].name; i++)
    {
      if ((len >= cmd_table [i].min_chr) &&
	  (strncasecmp (s, cmd_table [i].name, len) == 0))
	return (i);
    }

  return (-1);
}


static int help_cmd (int argc, char *argv[])
{
  int i;

  if (argc == 1)
    {
      for (i = 0; cmd_table [i].name; i++)
	fprintf (stderr, cmd_table [i].usage);
      fprintf (stderr, "\n"
	               "Commands may be abbreviated to the portion listed in caps.\n");
      return (0);
    }

  if (argc != 2)
    return (-1);

  i = find_cmd (argv [1]);
  if (i < 0)
    {
      fprintf (stderr, "unrecognized command\n");
      return (1);
    }

  fprintf (stderr, cmd_table [i].usage);
  return (0);
}


static void execute_command (int argc, char *argv [])
{
  int i;

  i = find_cmd (argv [0]);

  if (i < 0)
    {
      fprintf (stderr, "unrecognized command\n");
      return;
    }

  if ((* cmd_table [i].handler)(argc, argv) < 0)
    fprintf (stderr, "Usage: %s", cmd_table [i].usage);
}


#ifndef USE_READLINE

#define MAX_LINE 200

/*
 * print prompt, get a line of input, return a copy
 * caller must free
 */
char *readline (char *prompt)
{
  char inbuf [MAX_LINE];

  if (prompt)
    printf (prompt);
  fgets (inbuf, MAX_LINE, stdin);
  return (strdup (& inbuf [0]));
}

#endif /* USE_READLINE */


void debug (void)
{
  char *cmd;
  char *s;
  int argc;
  char *argv [MAX_ARGS];

  cmd = readline ("> ");
  if (cmd)
    {
#ifdef USE_HISTORY
      if (*cmd)
	add_history (cmd);
#endif
      argc = 0;
      for (s = cmd; (argc < MAX_ARGS) && ((s = strtok (s, " \t\n")) != NULL); s = NULL)
	argv [argc++] = s;

      if (argc)
	execute_command (argc, argv);

      free (cmd);
    }
}


void dobreak (void)
{
  run = 0;
  step = 0;
// Added to allow external programs to run the simulator
// 2002,2003 Mike Gore
  ex_flag = 0;
  if(broke)
	exit(1);
}


void sigint_handler (int sig)
{
    dobreak ();
    signal (SIGINT, sigint_handler);
}


void init_debug (void)
{
  signal (SIGINT, sigint_handler);
}




