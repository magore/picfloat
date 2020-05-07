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
 * $Id: pic.h,v 1.2 2004/01/22 02:14:51 magore Exp $
 */
// 2002,2003 Mike Gore
// Modified typedefs - next 4 lines - into defines

#define bool unsigned char
#define byte unsigned char 
#define romaddr unsigned short int
#define word unsigned short int

#define MAXROM 0x2000
#define MAXRAM 0x100
#define MAXSTACK 8

extern int stack_ind;
extern int max_stack_ind;

extern unsigned long cycle;

extern romaddr pc, nextpc;
extern bool skip_flag;

extern int ex_flag,ex_addr;

extern byte w;
extern byte option;
extern byte status;
extern byte fsr;
extern byte porta, trisa;
extern byte portb, trisb;
/* extern byte portc, trisc; */
extern byte adcon0, adcon1, adres;
extern byte pclath;
extern byte intcon;

#define STATUS_C   0x01
#define STATUS_DF  0x02
#define STATUS_Z   0x04
#define STATUS_PD  0x08
#define STATUS_TO  0x10
#define STATUS_RP0 0x20
#define STATUS_RP1 0x40
#define STATUS_RP  0x60
#define STATUS_IRP 0x80


#define OPTION_PS0    0x01
#define OPTION_PS1    0x02
#define OPTION_PS2    0x04
#define OPTION_PSA    0x08
#define OPTION_RTE    0x10
#define OPTION_RTS    0x20
#define OPTION_INTEDG 0x40
#define OPTION_RBPU   0x80


#define INTCON_RBIF   0x01
#define INTCON_INTF   0x02
#define INTCON_T0IF   0x04
#define INTCON_RBIE   0x08
#define INTCON_INTE   0x10
#define INTCON_T0IE   0x20
#define INTCON_ADIE   0x40
#define INTCON_GIE    0x80


#define ADCON0_ADON   0x01
#define ADCON0_ADIF   0x02
#define ADCON0_GO     0x04
#define ADCON0_CHS0   0x08
#define ADCON0_CHS1   0x10
#define ADCON0_CHS    0x18
#define ADCON0_ADCS0  0x40
#define ADCON0_ADCS1  0x80


extern romaddr stack [MAXSTACK];


void init_reg (void);

byte read_reg_abs (byte addr);
void write_reg_abs (byte addr, byte data);

byte read_reg_direct (byte addr);
void write_reg_direct (byte addr, byte data);


#define ROM_FLAG_LOADED        0x01
#define ROM_FLAG_BREAKPOINT    0x02
#define ROM_FLAG_EXECUTED      0x04
#define ROM_FLAG_EXECUTED_SKIP 0x08

bool test_rom_flag (romaddr addr, int flag);
void set_rom_flag  (romaddr addr, int flag);
void clr_rom_flag  (romaddr addr, int flag);

void init_rom (void);
int load_rom_file (char *fn);
word read_rom (romaddr addr);

void push (romaddr addr);
romaddr pop (void);

void my_ex_cmd (int addr);

typedef byte regrdfcn (byte addr);
typedef void regwrfcn (byte addr, byte data);


#define READFN(reg) \
byte reg##_rd (byte addr) \
{ \
  return (reg); \
}

#define WRITEFN(reg) \
void reg##_wr (byte addr, byte data) \
{ \
  reg = data; \
}



/* in picinst.c: */

void exec_inst (void);


/* in picdis.c: */

char *disassemble_inst (word inst, char *buffer);


/* in picio.c: */

int buttonl, buttonr;

regrdfcn porta_rd, trisa_rd, portb_rd, trisb_rd;
regwrfcn porta_wr, trisa_wr, portb_wr, trisb_wr;


/* in picadc.c: */

regrdfcn adcon0_rd, adcon1_rd, adres_rd;
regwrfcn adcon0_wr, adcon1_wr, adres_wr;

extern byte adc [4];


/* in pictmr0.c: */

extern byte tmr0;

regrdfcn tmr0_rd;
regwrfcn tmr0_wr;

void run_tmr0 (void);


/* in debug.c: */

extern int trace;
extern int run;
extern int step;
extern int quit;
extern int ignore_break;

void init_debug (void);
void debug (void);
void dobreak (void);


