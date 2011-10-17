/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfhdr.h"

#undef sffileno

#if __STD_C
int sffileno(reg Sfio_t* f)
#else
int sffileno(f)
reg Sfio_t*	f;
#endif
{
	return __sf_fileno(f);
}
