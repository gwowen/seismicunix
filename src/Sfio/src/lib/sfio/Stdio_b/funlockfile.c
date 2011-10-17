/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfstdio.h"

/*	Try to unlock a file
**	Written by Kiem-Phong Vo
*/

#if __STD_C
void funlockfile(FILE* f)
#else
void funlockfile(f)
reg FILE*	f;
#endif
{
	Sfio_t*	sf;

	if(!(sf = _sfstream(f)))
		return;

	(void)sfmutex(sf, SFMTX_UNLOCK);
}
