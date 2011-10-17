/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include	"sfhdr.h"

#undef sfulen

#if __STD_C
int sfulen(reg Sfulong_t v)
#else
int sfulen(v)
reg Sfulong_t	v;
#endif
{
	return __sf_ulen(v);
}
