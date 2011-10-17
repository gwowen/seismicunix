/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfhdr.h"

#undef sferror

#if __STD_C
int sferror(reg Sfio_t* f)
#else
int sferror(f)
reg Sfio_t*	f;
#endif
{
	return __sf_error(f);
}
