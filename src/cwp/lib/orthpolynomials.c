/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */


/*********************** self documentation **********************/
/***********************************************************************
ORTHPOLYNOMIALS - compute ORTHogonal POLYNOMIALS

hermite_n_polynomial:  Compute n-th order generalized Hermite polynomial.

**************************************************************************
hermite_n_polynomial: 

Input:
h0		array of size nt holding H_{n-1}
h1		array of size nt holding H_{n}
t		array of size nt holding time vector
nt		size of arrays, no. of samples
n		order of polynomial
sigma		variance
Output:
h		array of size nt holding H_{n+1}
******************************************************************************
Notes:	Note that n in the function call is the order of the derivative and
        j in the code below is the n in the recurrence relation
******************************************************************************
Copyright (c) 2007 by the Society of Exploration Geophysicists.
For more information, go to http://software.seg.org/2007/0004 .
You must read and accept usage terms at:
http://software.seg.org/disclaimer.txt before use.
******************************************************************************
Author: Werner M. Heigl, Apache Corporation, E&P Technology, December 2006
******************************************************************************/



#include "cwp.h"



void
hermite_n_polynomial(double *h, double *h0, double *h1, double *t, int nt, int n, double sigma)
/*****************************************************************************
  hermite_n_polynomial:  Compute n-th order generalized Hermite polynomial.
******************************************************************************
Input:
h0		array of size nt holding H_{n-1}
h1		array of size nt holding H_{n}
t		array of size nt holding time vector
nt		size of arrays, no. of samples
n		order of polynomial
sigma		variance
******************************************************************************
Output:
h		array of size nt holding H_{n+1}
******************************************************************************
Note:	Note that n in the function call is the order of the derivative and
        j in the code below is the n in the recurrence relation

******************************************************************************
Copyright (c) 2007 by the Society of Exploration Geophysicists.
For more information, go to http://software.seg.org/2007/0004 .
You must read and accept usage terms at:
http://software.seg.org/disclaimer.txt before use.
******************************************************************************
Author: Werner M. Heigl, Apache Corporation, E&P Technology, December 2006
******************************************************************************/
{
	int i;		/* loop variable */
	int j=1;	/* recurrence counter */

	/* as long as necessary use recurrence relation */
	do {
		/* current instance of recurrence relation */
		for (i = 0; i < nt; ++i) 		
			h[i] = ( t[i] * h1[i] - j * h0[i] ) / sigma;
		
		/* update inputs to recurrence relation */
		memcpy((void *) h0, (const void *) h1, DSIZE * nt);
		memcpy((void *) h1, (const void *) h , DSIZE * nt);
		
		/* update counter */
		++j;
		
	} while (j < n);
	
}
