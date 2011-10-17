/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfhdr.h"

#undef sfgetc

#if __STD_C
int sfgetc(reg Sfio_t* f)
#else
int sfgetc(f)
reg Sfio_t*	f;
#endif
{
	return __sf_getc(f);
}
