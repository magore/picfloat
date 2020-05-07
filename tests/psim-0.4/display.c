/*
 * x_interface.c: X window system display for PIC-Pong simulator
 *
 * Copyright 1991, 1992, 1993, 1996, 1997, 2000 Eric Smith <brouhaha@users.sourceforge.net>
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
 * $Id: display.c,v 1.1.1.1 2003/12/14 03:40:05 magore Exp $
 */

#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "pic.h"
#include "display.h"


Display *mydisplay;
Window mywindow;


/* pixel values */
#define SYNC 0
#define BLANK 1
#define BLACK 2
#define WHITE 3

#define MAX_PIX 4

unsigned long pixel_value [MAX_PIX];
GC gc [MAX_PIX];


#define window_width 300
#define window_height 256


int hpos;
int line;
int frame;

unsigned char display_buffer [window_height][window_width];


void left_up (void)
{
  if (adc [0] != 0)
    adc [0] --;
}

void left_down (void)
{
  if (adc [0] != 255)
    adc [0] ++;
}

void right_up (void)
{
  if (adc [1] != 0)
    adc [1] --;
}

void right_down (void)
{
  if (adc [1] != 255)
    adc [1] ++;
}

void serve (void)
{
}


typedef void (*keyfunc)(void);


#define MAX_KEY 256
keyfunc keymap [MAX_KEY];


void setup_keyboard (void)
{
  int i;

  for (i = 0; i < MAX_KEY; i++)
    keymap [i] = NULL;

  keymap [XKeysymToKeycode (mydisplay, XK_a)] = left_up;
  keymap [XKeysymToKeycode (mydisplay, XK_z)] = left_down;

  keymap [XKeysymToKeycode (mydisplay, XK_k)] = right_up;
  keymap [XKeysymToKeycode (mydisplay, XK_m)] = left_down;

  keymap [XKeysymToKeycode (mydisplay, XK_space)] = serve;
}


void setup_colors (void)
{
  Colormap cmap;
  XColor c1, c2;

  cmap = DefaultColormap (mydisplay, DefaultScreen (mydisplay));

  XAllocNamedColor (mydisplay, cmap,
		"red",
		& c1,
		& c2);
  pixel_value [SYNC] = c2.pixel;

  XAllocNamedColor (mydisplay, cmap,
		"black",
		& c1,
		& c2);
  pixel_value [BLANK] = c2.pixel;

  XAllocNamedColor (mydisplay, cmap,
		"grey10",
		& c1,
		& c2);
  pixel_value [BLACK] = c2.pixel;

  XAllocNamedColor (mydisplay, cmap,
		"white",
		& c1,
		& c2);
  pixel_value [WHITE] = c2.pixel;
}


void init_display (int argc, char *argv[],
		    char *window_name)
{
  XSizeHints myhint;
  int myscreen;
  int i;

  mydisplay = XOpenDisplay ("");
  if (!mydisplay)
    {
      fprintf (stderr, "Can't init X\n");
      exit (1);
    }
  myscreen = DefaultScreen (mydisplay);

  pixel_value [WHITE] = WhitePixel (mydisplay, myscreen);
  pixel_value [BLACK] = BlackPixel (mydisplay, myscreen);

  myhint.x = 50; myhint.y = 50;
  myhint.width = window_width;
  myhint.height = window_height;
  myhint.flags = PPosition | PSize;

  mywindow = XCreateSimpleWindow
    (mydisplay,
     DefaultRootWindow (mydisplay),
     myhint.x, myhint.y, myhint.width, myhint.height,
     5, 
     pixel_value [WHITE],
     pixel_value [BLACK]);

  XSetStandardProperties (mydisplay, mywindow, window_name, window_name,
			  None, argv, argc, &myhint);

  setup_colors ();

  for (i = 0; i < MAX_PIX; i++)
    {
      gc [i] = XCreateGC (mydisplay, mywindow, 0, 0);
      XSetBackground (mydisplay, gc [i], pixel_value [BLACK]);
      XSetForeground (mydisplay, gc [i], pixel_value [i]);
    }

  XSelectInput (mydisplay, mywindow,
		KeyPressMask | KeyReleaseMask);

  XMapRaised (mydisplay, mywindow);

  setup_keyboard ();

  hpos = 0;
  line = 0;
  frame = 0;
}


void handle_input ()
{
  XEvent event;

  while (XEventsQueued (mydisplay, QueuedAfterReading) != 0)
    {
      XNextEvent (mydisplay, & event);
      switch (event.type)
	{
	case KeyPress:
	  if (keymap [event.xkey.keycode])
	    {
	      keymap[event.xkey.keycode] ();
	    }
	  break;
	case KeyRelease:
	  break;
	case NoExpose:
	  break;
	default:
	  printf ("unknown event %d\n", event.type);
	  break;
	}
    }
}




void bell (void)
{
  XBell (mydisplay, 50);
}


void advance_line (void)
{
  hpos = 0;
  line += 1;
}


int sync_width = 0;
extern int trace;


int frame_too_long_warn = 0;
int line_too_long_warn = 0;


void display_pixel (int sync, int blank, int vid)
{
  unsigned char pixcode = BLACK;
#if 1
  if (sync & (!blank))
    {
      printf ("sync without blanking!\n");
      dobreak ();
    }
  if (sync & vid)
    {
      printf ("sync with white!\n");
      dobreak ();
    }
  if (blank & vid)
    {
      printf ("blanking and white!\n");
      dobreak ();
    }
#endif
  if (sync)
    {
      pixcode = SYNC;
      if (! sync_width)
	{
	  advance_line ();
	  line_too_long_warn = 0;
	}
      sync_width++;
    }
  else
    {
      if ((sync_width > 50) && (line > 200))
	{
	  line = 0;
	  frame++;
	  frame_too_long_warn = 0;
	  printf ("frame %d\n", frame);
	  XFlush (mydisplay);
	}
      sync_width = 0;
      if (blank)
	pixcode = BLANK;
      else if (vid)
	pixcode = WHITE;
    }
  if ((line < window_height) && (hpos < window_width))
    {
      display_buffer [line][hpos] = pixcode;
      XDrawPoint (mydisplay, mywindow, gc [pixcode], hpos, line);
    }
  else
    {
      if ((! frame_too_long_warn) && (line >= (window_height + 1)))
	{
	  printf ("frame too long\n");
	  frame_too_long_warn = 1;
	  dobreak ();
	}
      if ((! line_too_long_warn) && (hpos >= window_width))
	{
	  printf ("line too long\n");
	  line_too_long_warn = 1;
	  dobreak ();
	}
    }
  hpos += 1;
}
