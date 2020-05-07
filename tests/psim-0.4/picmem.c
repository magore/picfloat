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
 * $Id: picmem.c,v 1.3 2004/01/28 02:35:42 magore Exp $
 */

#include <stdio.h>
#include "pic.h"
#include "ihexin.h"


unsigned long cycle;

romaddr pc, nextpc;
bool skip_flag;


byte w;
byte option;
byte status;
byte fsr;
byte pclath;
byte intcon;

romaddr stack [MAXSTACK];


word rom [MAXROM];
byte rom_flags [MAXROM];


byte ram [MAXRAM];
byte *ramptr [MAXRAM];

byte (* ram_rd_fn [MAXRAM])(byte addr);
void (* ram_wr_fn [MAXRAM])(byte addr, byte data);


int stack_ind = 0;
int max_stack_ind = 0;
unsigned int stack_save[MAXSTACK];

void stack_init()
{
	int i;
	stack_ind = 0;
	max_stack_ind = 0;
	for(i=0;i<MAXSTACK;++i)
		stack_save[i] = 0;
}

void stack_dump(int levels)
{
	int i;
	for(i=0;i<levels;++i)
		if(i == 0) 
			printf("stack[top]:%04x\n", stack[i]);
		else
			printf("stack[%d]:%04x\n", i, stack[i]);
}

void push (romaddr addr)
{
  int i;
// Added to allow external programs to run the simulator
// 2002,2003 Mike Gore
	if(stack_ind < MAXSTACK)
		++stack_ind;
	else {
		printf("STACK OVERFLOW\n");
		stack_dump(stack_ind);
		run = 0;
		step = 0;
		ex_flag = 0;
	}

  for (i = MAXSTACK - 1; i > 0; i--)
    stack [i] = stack [i - 1];
  stack [0] = addr;

// Track Max Stack Depth
	if(stack_ind > max_stack_ind) {
		max_stack_ind = stack_ind;
		for(i=0;i<stack_ind;++i)
			stack_save[i] = stack[i];
		for(;i<MAXSTACK;++i)
			stack_save[i] = 0;
	}
}

romaddr pop (void)
{
	romaddr result;
	int i;

// 2002,2003 Mike Gore
// Added to allow external programs to run the simulator
// We track the stack level to determine if a call has returned yet
// No stack to pop!
	if(!stack_ind) {
		printf("STACK UNDERFLOW\n");
		ex_flag = 0;
		run = 0;
		step = 0;
		return(pc);
	}

	if(stack_ind) 
		--stack_ind;

	if(!stack_ind) {
		if(!stack_ind && ex_flag) {
			ex_flag = 0;
			run = 0;
			step = 0;
		}
	}

  result = stack [0];
  for (i = 0; i < MAXSTACK - 1; i++)
    stack [i] = stack [i+1];
  stack [MAXSTACK - 1] = result;
	
  return (result);
}


byte ram_rd (byte addr)
{
  return (* (ramptr [addr]));
}

void ram_wr (byte addr, byte data)
{
  * (ramptr [addr]) = data;
}


byte ind_rd (byte addr)
{
  if (fsr & 0x7f)
    return (read_reg_abs (fsr));  /* $$$ doesn't handle IRP */
  else
    return (0);
}

void ind_wr (byte addr, byte data)
{
  if (fsr & 0x7f)
    write_reg_abs (fsr, data);
}


READFN(option)
WRITEFN(option)


byte pcl_rd (byte addr)
{
  return (pc & 0xff);
}

void pcl_wr (byte addr, byte data)
{
  nextpc = (pclath << 8) | data;
  skip_flag = 1;
}


READFN(status)
WRITEFN(status)

READFN(fsr)
WRITEFN(fsr)

READFN(pclath)
WRITEFN(pclath)

READFN(intcon)
WRITEFN(intcon)


void init_reg (void)
{
  int i;

  for (i = 0; i < MAXRAM; i++)
    ramptr [i] = (void *) 0;
  for (i = 0x0c; i <= 0x7f; i++)  /* $$$ hack alert */
    {
      ramptr [i] = ramptr [i+0x80] = & ram [i];
      ram_rd_fn [i] = ram_rd_fn [i+0x80] = & ram_rd;
      ram_wr_fn [i] = ram_wr_fn [i+0x80] = & ram_wr;
    }

  ram_rd_fn [0x00] = ram_rd_fn [0x80] = & ind_rd;
  ram_wr_fn [0x00] = ram_wr_fn [0x80] = & ind_wr;

  ram_rd_fn [0x01] = & tmr0_rd;
  ram_wr_fn [0x01] = & tmr0_wr;

  ram_rd_fn [0x81] = & option_rd;
  ram_wr_fn [0x81] = & option_wr;

  ram_rd_fn [0x02] = ram_rd_fn [0x82] = & pcl_rd;
  ram_wr_fn [0x02] = ram_wr_fn [0x82] = & pcl_wr;

  ram_rd_fn [0x03] = ram_rd_fn [0x83] = & status_rd;
  ram_wr_fn [0x03] = ram_wr_fn [0x83] = & status_wr;

  ram_rd_fn [0x04] = ram_rd_fn [0x84] = & fsr_rd;
  ram_wr_fn [0x04] = ram_wr_fn [0x84] = & fsr_wr;

  ram_rd_fn [0x05] = & porta_rd;
  ram_wr_fn [0x05] = & porta_wr;

  ram_rd_fn [0x85] = & trisa_rd;
  ram_wr_fn [0x85] = & trisa_wr;

  ram_rd_fn [0x06] = & portb_rd;
  ram_wr_fn [0x06] = & portb_wr;

  ram_rd_fn [0x86] = & trisb_rd;
  ram_wr_fn [0x86] = & trisb_wr;

  ram_rd_fn [0x08] = & adcon0_rd;
  ram_wr_fn [0x08] = & adcon0_wr;

  ram_rd_fn [0x88] = & adcon1_rd;
  ram_wr_fn [0x88] = & adcon1_wr;

  ram_rd_fn [0x09] = ram_rd_fn [0x89] = & adres_rd;
  ram_wr_fn [0x09] = ram_wr_fn [0x89] = & adres_wr;

  ram_rd_fn [0x0a] = ram_rd_fn [0x8a] = & pclath_rd;
  ram_wr_fn [0x0a] = ram_wr_fn [0x8a] = & pclath_wr;

  ram_rd_fn [0x0b] = ram_rd_fn [0x8b] = & intcon_rd;
  ram_wr_fn [0x0b] = ram_wr_fn [0x8b] = & intcon_wr;

}

byte read_reg_abs (byte addr)
{
  if (ram_rd_fn [addr])
    return ((* ram_rd_fn [addr]) (addr));
  else
    {
      fprintf (stderr, "warning: stray read of %03x\n", addr);
      dobreak ();
      return (0);
    }
}

void write_reg_abs (byte addr, byte data)
{
  if (ram_wr_fn [addr])
    (* ram_wr_fn [addr]) (addr, data);
  else
    {
      fprintf (stderr, "warning: stray write of %03x\n", addr);
      dobreak ();
    }
}


byte read_reg_direct (byte addr)
{
  addr |= ((status & (STATUS_RP)) << 2);
  if (ram_rd_fn [addr])
    return ((* ram_rd_fn [addr]) (addr));
  else
    {
      fprintf (stderr, "warning: stray read of %03x\n", addr);
      dobreak ();
      return (0);
    }
}

void write_reg_direct (byte addr, byte data)
{
  addr |= ((status & (STATUS_RP)) << 2);
  if (ram_wr_fn [addr])
    (* ram_wr_fn [addr]) (addr, data);
  else
    {
      fprintf (stderr, "warning: stray write of %03x\n", addr);
      dobreak ();
    }
}


bool test_rom_flag (romaddr addr, int flag)
{
  return (rom_flags [addr] & flag);
}

void set_rom_flag  (romaddr addr, int flag)
{
  rom_flags [addr] |= flag;
}

void clr_rom_flag  (romaddr addr, int flag)
{
  rom_flags [addr] &= ~flag;
}


void init_rom (void)
{
  int i;
  for (i = 0; i < MAXROM; i++)
    {
      rom [i] = 0;
      rom_flags [i] = 0;
    }
}

int load_rom_file (char *fn)
{
  ihex_in *f;
  uint32 addr;
  uint8 data [2];

  f = open_ihex_in (fn);
  if (!f)
    return (0);

  while (read_ihex (f, 2, & addr, & data [0]) == 2)
    {
      addr >>= 1;
      if (addr < MAXROM)
	{
	  rom [addr] = (data [1] << 8) | data [0];
	  set_rom_flag (addr, ROM_FLAG_LOADED);
	}
    }

  close_ihex_in (f);
  return (1);
}

word read_rom (romaddr addr)
{
  return (rom [addr]);
}


