/*
 * Copyright 1991, 1996, 1997, 2000 Eric Smith <brouhaha@users.sourceforge.net>
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
 * $Id: ihexin.c,v 1.1.1.1 2003/12/14 03:40:05 magore Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ihexin.h"

ihex_in *open_ihex_in (char *fn)
{
  ihex_in *s;

  s = (ihex_in *) calloc (sizeof (ihex_in), 1);
  if (! s)
    return (NULL);

  if ((! fn) || (strcmp (fn, "-") == 0))
    s->f = stdin;
  else
    s->f = fopen (fn, "r");

  if (! s->f)
    {
      free (s);
      return (NULL);
    }

  return (s);
}

uint32 close_ihex_in (ihex_in *s)
{
  uint32 start_addr;

  start_addr = s->start_addr;
  fclose (s->f);
  free (s);
  return (start_addr);
}

static uint8 hex_digit (char p)
{
  switch (p)
    {
    case '0': return (0x0);
    case '1': return (0x1);
    case '2': return (0x2);
    case '3': return (0x3);
    case '4': return (0x4);
    case '5': return (0x5);
    case '6': return (0x6);
    case '7': return (0x7);
    case '8': return (0x8);
    case '9': return (0x9);
    case 'A': return (0xA);
    case 'B': return (0xB);
    case 'C': return (0xC);
    case 'D': return (0xD);
    case 'E': return (0xE);
    case 'F': return (0xF);
    case 'a': return (0xa);
    case 'b': return (0xb);
    case 'c': return (0xc);
    case 'd': return (0xd);
    case 'e': return (0xe);
    case 'f': return (0xf);
    default: return (0);
    }
}

static uint8 hex_byte (char **p, uint8 *cksm)
{
  uint8 byte;
  byte = hex_digit (**p) << 4;
  (*p)++;
  byte |= hex_digit (**p);
  (*p)++;
  if (cksm)
    (*cksm) += byte;
  return (byte);
}

#define MAX_IHEX_CHARS 256
/* must be <= (MAX_IHEX_IN_BYTES * 2) */

/* returns boolean */
static int fetch_ihex (ihex_in *s)
{
  char buffer [MAX_IHEX_CHARS];
  int buflen;
  char *sp;
  int ihex_type;
  int ihex_len;
  int addr_len = 2;
  int i;
  uint8 cksm;

  s->len = 0;
  s->idx = 0;

  /* read input until EOF or a line starting with an ':' */
  do
    {
      if (! fgets (buffer, MAX_IHEX_CHARS, s->f))
	return (0);
      buflen = strlen (buffer);
// 2002,2003 Mike Gore
// remove control characters
		while(buflen) {
			if(buffer[buflen-1] < ' ')
				buffer[--buflen] = 0;
			else break;
		}

    }
  while (buffer [0] != ':');

  /* make sure the record is long enough to have a length, address, type,
     and checksum */
  if (buflen < 11)
    return (0);

  /* make sure length, address, and type are composed of valid hex digits */
  for (i = 1; i <= 8; i++)
    if (! isxdigit (buffer [i]))
      return (0);

  sp = & buffer [1];

  /* extract length */
  ihex_len = hex_byte (& sp, & cksm);

  /* extract base address */
  s->base_addr = 0;
  for (i = 0; i < addr_len; i++)
    {
      s->base_addr <<= 8;
      s->base_addr += hex_byte (& sp, & cksm);
    }

  /* extract type */
  ihex_type = hex_byte (& sp, & cksm);

// printf("addr: %04x,len:%02x, type:%02x\n", s->base_addr,ihex_len,ihex_type);
// 2002,2003 Mike Gore
// Added next two lines
	if(ihex_type == 4) 
		ihex_type = 0;

  /* make sure we recognize the record type */
  if ((ihex_type != 0) & (ihex_type != 1))
    return (0);

#if 0
  cksm = 0;

  /* make sure length includes at least the address and checksum */
  if (ihex_len < (addr_len + 1))
    return (0);

  /* make sure we have enough characters in the buffer */
  if ((ihex_len * 2) > (buflen - 4))
    return (0);

  /* make sure rest of S-record is composed of valid hex digits */
  for (i = 0; i < (ihex_len * 2); i++)
    if (! isxdigit (buffer [i+4]))
      return (0);

  /* subtract address length and checksum length from s-record length */
  ihex_len -= (addr_len + 1);
#endif

  /* extract data bytes */
  for (i = 0; i < ihex_len; i++)
    s->bytes [i] = hex_byte (& sp, & cksm);

#if 0  
  /* extract and verify checksum */
  (void) hex_byte (&sp, & cksm);
  if (cksm != 0xff)
    return (0);
#endif

  /* if end record, save start address */
  if (ihex_type == 1)
    {
      s->start_addr = s->base_addr;
      return (1);
    }

  s->len = ihex_len;
  return (1);
}

/* returns number of bytes read, 0 for error or end of file */
int read_ihex (ihex_in *s, uint32 len, uint32 *addr, uint8 *data)
{
  int ret_len = len;

  if (s->idx == s->len)
    {
		if (! fetch_ihex (s)) {
// 2002,2003 Mike Gore
// Add error message
			printf(" oops: %s\n",s);
			return (0);
		}
    }

  if (ret_len > (s->len - s->idx))
    ret_len = s->len;

  (*addr) = s->base_addr + s->idx;
  memcpy (data, & s->bytes [s->idx], ret_len);
  s->idx += ret_len;

  return (ret_len);
}

