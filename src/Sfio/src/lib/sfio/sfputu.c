/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfhdr.h"

#undef sfputu

#if __STD_C
int sfputu(reg Sfio_t* f, Sfulong_t u)
#else
int sfputu(f,u)
reg Sfio_t*	f;
reg Sfulong_t	u;
#endif
{
	return __sf_putu(f,u);
}
