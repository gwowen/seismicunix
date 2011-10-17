/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

/* SUKILL: $Revision: 1.15 $ ; $Date: 2006/11/07 22:58:42 $	*/

#include "su.h"
#include "segy.h"

/*********************** self documentation **********************/
char *sdoc[] = {
" 								",
" SUKILL - zero out traces					",
" 								",
" sukill <stdin >stdout min= count=1				",
" 								",
" Required parameters						",
" 	min= 		first trace to kill (one-based)		",
" 								",
" Optional parameters						",
" 	count= 1	number of traces to kill 		",
" 								",
NULL};

/* Credits:
 *	CWP: Chris Liner, Jack K. Cohen
 *
 * Trace header fields accessed: ns
 */
/**************** end self doc ***********************************/


segy tr;

int
main(int argc, char **argv)
{
	register int itr;	/* trace counter		*/
	int min;		/* first trace to zero out	*/
	int count;		/* number of traces to zero out	*/
	int nt = 0;		/* number of time samples	*/

	/* Initialize */
	initargs(argc, argv);
	requestdoc(1);


	/* Get parameters */
	MUSTGETPARINT("min", &min);
	if (min < 1) err("min = %d, must be >= 1", min);
	if (!getparint("count", &count))  count = 1;


	/* Echo traces till min if they are there */
	for (itr = 1; itr < min; ++itr) {
		if (gettr(&tr)) puttr(&tr);
		else err("failed to get requested trace #%ld", itr);
	}

	/* Kill "count" traces if they are there
	 * Do first outside loop to get nt    */
	if (gettr(&tr)) {
		nt = tr.ns;
		memset( (void *) tr.data, 0, nt * FSIZE);
		puttr(&tr);
		++itr;
	} else err("failed to get requested trace #%ld", itr);


	for ( ; itr < min + count; ++itr) {
		if (gettr(&tr)) {
			memset( (void *) tr.data, 0, nt * FSIZE);
			puttr(&tr);
		} else err("failed to get requested trace #%ld", itr);
	}


	/* Echo the trailing traces if any */
	while (gettr(&tr)) {
		puttr(&tr);
	}

	return(CWP_Exit());
}
