/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfstdio.h"

/*	Try to lock a file
**	Written by Kiem-Phong Vo
*/

#if __STD_C
int ftrylockfile(FILE* f)
#else
int ftrylockfile(f)
reg FILE*	f;
#endif
{
	Sfio_t*	sf;

	if(!(sf = _sfstream(f)))
		return 0;

	return sfmutex(sf, SFMTX_TRYLOCK);
}
