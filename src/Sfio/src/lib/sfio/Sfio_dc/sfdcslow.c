/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include "sfdchdr.h"

/*	Make a stream op return immediately on interrupts.
**	This is useful on slow streams (hence the name).
**
**	Written by Glenn Fowler (03/18/1998).
*/

#if __STD_C
static int slowexcept(Sfio_t* f, int type, Void_t* v, Sfdisc_t* disc)
#else
static int slowexcept(f, type, v, disc)
Sfio_t*		f;
int		type;
Void_t*		v;
Sfdisc_t*	disc;
#endif
{
	NOTUSED(f);
	NOTUSED(v);
	NOTUSED(disc);

	switch (type)
	{
	case SF_FINAL:
	case SF_DPOP:
		free(disc);
		break;
	case SF_READ:
	case SF_WRITE:
		if (errno == EINTR)
			return(-1);
		break;
	}

	return(0);
}

#if __STD_C
int sfdcslow(Sfio_t* f)
#else
int sfdcslow(f)
Sfio_t*	f;
#endif
{
	Sfdisc_t*	disc;

	if(!(disc = (Sfdisc_t*)malloc(sizeof(Sfdisc_t))) )
		return(-1);

	disc->readf = NIL(Sfread_f);
	disc->writef = NIL(Sfwrite_f);
	disc->seekf = NIL(Sfseek_f);
	disc->exceptf = slowexcept;

	if(sfdisc(f,disc) != disc)
	{	free(disc);
		return(-1);
	}

	return(0);
}
