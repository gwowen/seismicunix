/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfhdr.h"

#undef sfdlen

#if __STD_C
int sfdlen(reg Sfdouble_t v)
#else
int sfdlen(v)
reg Sfdouble_t	v;
#endif
{
	return __sf_dlen(v);
}
