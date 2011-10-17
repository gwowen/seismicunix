/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfhdr.h"

#undef sfputl

#if __STD_C
int sfputl(reg Sfio_t* f, Sflong_t l)
#else
int sfputl(f,l)
reg Sfio_t*	f;
reg Sflong_t	l;
#endif
{
	return __sf_putl(f,l);
}
