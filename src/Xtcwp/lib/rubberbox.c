/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

/* RUBBERBOX: $Revision: 1.5 $ ; $Date: 1997/07/30 21:14:57 $	*/

/*********************** self documentation **********************/
/*****************************************************************************
RUBBERBOX -  Function to draw a rubberband box in X-windows plots

XtcwpRubberbox	Track pointer with rubberband box

******************************************************************************
Function Prototype:
void XtcwpRubberbox (Display *dpy, Window win, XEvent event,
	int *x, int *y, int *width, int *height);

******************************************************************************
Input:
dpy		display pointer
win		window ID
event		event of type ButtonPress

Output:
x		x of upper left hand corner of box in pixels
y		y of upper left hand corner of box in pixels
width		width of box in pixels
height		height of box in pixels

******************************************************************************
Notes:
XtcwpRubberbox assumes that event is a ButtonPress event for the 1st button;
i.e., it tracks motion of the pointer while the 1st button is down, and
it sets x, y, w, and h and returns after a ButtonRelease event for the
1st button.

Before calling XtcwpRubberbox, both ButtonRelease and Button1Motion events 
must be enabled.

******************************************************************************
Author:		Dave Hale, Colorado School of Mines, 01/27/90
*****************************************************************************/
/**************** end self doc ********************************/

#include "Xtcwp/Xtcwp.h"

void 
XtcwpRubberBox (Display *dpy, Window win, XEvent event,
	int *x, int *y, int *width, int *height)
/*****************************************************************************
Track pointer with rubber box
******************************************************************************
Input:
dpy		display pointer
win		window ID
event		event of type ButtonPress

Output:
x		x of upper left hand corner of box in pixels
y		y of upper left hand corner of box in pixels
width		width of box in pixels
height		height of box in pixels
******************************************************************************
Notes:
XtcwpRubberBox assumes that event is a ButtonPress event for the 1st button;
i.e., it tracks motion of the pointer while the 1st button is down, and
it sets x, y, w, and h and returns after a ButtonRelease event for the
1st button.

Before calling XtcwpRubberBox, both ButtonRelease and Button1Motion events 
must be enabled.
******************************************************************************
Author:		Dave Hale, Colorado School of Mines, 01/27/90
*****************************************************************************/
{
	GC gc;
	XGCValues *values=NULL;
	XEvent eventb;
	XStandardColormap scmap;
	int scr=DefaultScreen(dpy);
	int xb,yb,w,h,x1,x2,y1,y2,xorig,yorig,xold,yold;
	long background;

	/* determine typical background color */
	if (XtcwpCreateRGBDefaultMap(dpy,&scmap))
		background = (long) ((XtcwpGetFirstPixel(dpy)+XtcwpGetLastPixel(dpy))/2);
	else
		background = (long) WhitePixel(dpy,scr);

	/* make graphics context */
	gc = XCreateGC(dpy,win,0,values);
  	XSetFunction(dpy,gc,GXxor);
  	XSetForeground(dpy,gc,BlackPixel(dpy,scr)^background);

	/* track pointer */
	xorig = event.xbutton.x;
	yorig = event.xbutton.y;
	xold = xorig;
	yold = yorig;
	x1 = xorig;
	y1 = yorig;
	w = 0;
	h = 0;
	while((h)|(~h)/* True */) {
		XNextEvent(dpy,&eventb);
		if (eventb.type==ButtonRelease) {
			xb = eventb.xbutton.x;
			yb = eventb.xbutton.y;
			break;
		} else if (eventb.type==MotionNotify) {
			xb = eventb.xmotion.x;
			yb = eventb.xmotion.y;

			/* if box is the same, continue */
			if (xb==xold && yb==yold) 
				continue;

			/* erase old box */
			x1 = (xold<xorig)?xold:xorig;
			y1 = (yold<yorig)?yold:yorig;
			x2 = (xold>xorig)?xold:xorig;
			y2 = (yold>yorig)?yold:yorig;
			w = x2-x1;
			h = y2-y1;
			XDrawRectangle(dpy,win,gc,x1,y1,w,h);

			/* draw current box */
			x1 = (xb<xorig)?xb:xorig;
			y1 = (yb<yorig)?yb:yorig;
			x2 = (xb>xorig)?xb:xorig;
			y2 = (yb>yorig)?yb:yorig;
			w = x2-x1;
			h = y2-y1;
			XDrawRectangle(dpy,win,gc,x1,y1,w,h);

			/* remember current pointer position */
			xold = xb;
			yold = yb;
		}
	}

	/* erase rubber box */
	XDrawRectangle(dpy,win,gc,x1,y1,w,h);

	/* free graphics context */
	XFreeGC(dpy,gc);

	/* set output parameters */
	*x = x1;
	*y = y1;
	*width = w;
	*height = h;
}
