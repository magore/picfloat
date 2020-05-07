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
 * $Id: ihexin.h,v 1.1.1.1 2003/12/14 03:40:06 magore Exp $
 */

typedef unsigned char uint8;
typedef unsigned int uint32;

#define MAX_IHEX_IN_BYTES 128

/* ihex_in is intended to be an opaque type; don't look inside it! */
typedef struct
{
  FILE *f;
  char bytes [MAX_IHEX_IN_BYTES];
  uint32 base_addr;
  uint32 len;
  uint32 idx;
  uint32 start_addr;
} ihex_in;

ihex_in *open_ihex_in (char *fn);

uint32 close_ihex_in (ihex_in *s);

/* returns number of bytes read, 0 for error or end of file */
int read_ihex (ihex_in *s, uint32 len, uint32 *addr, uint8 *data);


