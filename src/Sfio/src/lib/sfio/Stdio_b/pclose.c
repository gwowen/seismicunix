/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfstdio.h"

/*	Closing a popen stream
**	Written by Kiem-Phong Vo
*/


#if __STD_C
int pclose(reg FILE* f)
#else
int pclose(f)
reg FILE*	f;
#endif
{
	return fclose(f);
}
