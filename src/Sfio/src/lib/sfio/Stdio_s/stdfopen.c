/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"stdio_s.h"

/*	Open a stream given a file descriptor.
**
**	Written by Kiem-Phong Vo.
*/

#if __STD_C
Sfio_t* _stdfopen(const char* file, reg const char* mode)
#else
Sfio_t* _stdfopen(file,mode)
reg char*	file;
reg char*	mode;
#endif
{
	Sfio_t*	f;

	if((f = sfopen(NIL(Sfio_t*), file, mode)) )
	{	int	uflag;
		_sftype(mode, NIL(int*), &uflag);
		if(!uflag)
			f->flags |= SF_MTSAFE;
	}

	return f;
}
