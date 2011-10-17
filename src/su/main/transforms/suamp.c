/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

/* SUAMP: $Revision: 1.16 $ ; $Date: 2010/01/27 18:45:01 $		*/

#include "su.h"
#include "segy.h"

/*********************** self documentation **********************/
char *sdoc[] = {
" 								",
" SUAMP - output amp, phase, real or imag trace from		",
" 	(frequency, x) domain data				",
" 								",
" suamp <stdin >stdout mode=amp					",
" 								",
" Required parameters:						",
" none								",
" Optional parameter:						",
" mode=amp	output flag		 			",
" 		=amp	output amplitude traces			",
" 	       	=phase	output phase traces			",
" 	       	=real	output real parts			",
" 	     	=imag	output imag parts	 		",
" jack=0	=1  divide value at zero frequency by 2   	",
"                (operative only for mode=amp)			",
" Notes:							",
" 	The trace returned is half length from 0 to Nyquist. 	",
" 								",
" Example:							",
" 	sufft <data | suamp >amp_traces				",
" Example: 							",
"	sufft < data > complex_traces				",
" 	 suamp < complex_traces mode=real > real_traces		",
" 	 suamp < complex_traces mode=imag > imag_traces		",
"  								",
" Note: the inverse of the above operation is: 			",
"       suop2 real_traces imag_traces op=zipper > complex_traces",
"  								",
" Note: Explanation of jack=1 					",
" The amplitude spectrum is the modulus of the complex output of",
" the fft. f(0) is thus the average over the range of integration",
" of the transform. For causal functions, or equivalently, half",
" transforms, f(0) is 1/2 of the average over the full range.	",
" Most oscillatory functions encountered in wave applications are",
" zero mean, so this is usually not an issue.			",
NULL};

/* Credits:
 *	CWP: Shuki Ronen, Jack K. Cohen c.1986
 *
 * Notes:
 *	If efficiency becomes important consider inverting main loop
 *      and repeating extraction code within the branches of the switch.
 *
 * Trace header fields accessed: ns, trid
 * Trace header fields modified: ns, trid
 */
/**************** end self doc ***********************************/


#define	REAL	1
#define	IMAG	2
#define	AMP	3
#define	ARG	4

segy tr;

int
main(int argc, char **argv)
{
	cwp_String mode;	/* display: real, imag, amp, arg	*/
	int imode=AMP;		/* integer abbrev. for mode in switch	*/
	int nf;			/* number of samples on input trace	*/
	int nfby2;		/* nf/2					*/
	register float *xr;	/* real part of trace			*/
	register float *xi;	/* imaginary part of trace		*/
	int jack=0;		/* flag for special treatment of zero omega */


	/* Initialize */
	initargs(argc, argv);
	requestdoc(1);


	/* Get information from first trace */
	if (!gettr(&tr)) err("can't get first trace");
	if (tr.trid != FUNPACKNYQ)
		err("input not complex freq data, trid=%d", tr.trid);

	nf = tr.ns; /* always even--includes 1 or 2 "wasted" slots */
	nfby2 = nf/2;


	/* Get mode; note that imode is initialized to AMP */
	if (!getparstring("mode", &mode))	mode = "amp";
	if (!getparint("jack",&jack))	jack = 0;

	if      (STREQ(mode, "phase")) imode = ARG;
	else if (STREQ(mode, "real"))  imode = REAL;
	else if (STREQ(mode, "imag"))  imode = IMAG;
	else if (!STREQ(mode, "amp"))
		err("unknown mode=\"%s\", see self-doc", mode);


	/* Allocate arrays for real and imaginary parts */
	xr = ealloc1float(nfby2);
	xi = ealloc1float(nfby2);

	/* Main loop over traces */
	do {
		register int i;

		/* Separate complex trace into real and imag parts */
		for (i = 0; i < nfby2; ++i) {
			xr[i] = tr.data[2*i];
			xi[i] = tr.data[2*i+1];
		}

		/* Compute the desired half-length trace */
		switch(imode) {
		case REAL:
			for (i = 0; i < nfby2; ++i) {
				tr.data[i] = xr[i];
			}
			tr.trid = REALPART;
		break;
		case IMAG:
			for (i = 0; i < nfby2; ++i) {
				tr.data[i] = xi[i];
			}
			tr.trid = IMAGPART;
		break;
		case AMP:
		{
	 		register float re, im;
	
			re = xr[0];
			im = xi[0];
			if (jack) {
				tr.data[0] = (float) sqrt (re * re + im * im) / 2.0;
			} else {
				tr.data[0] = (float) sqrt (re * re + im * im);
			}
				for (i = 1; i < nfby2; ++i) {
					re = xr[i];
					im = xi[i];
					tr.data[i] = (float) sqrt (re * re + im * im);
			}
			tr.trid = AMPLITUDE;
		}
		break;
		case ARG:
			for (i = 0; i < nfby2; ++i) {
				float re = xr[i];
				float im = xi[i];
				if (re*re+im*im)  tr.data[i] = atan2(im, re);
				else	      tr.data[i] = 0.0;
			}
			tr.trid = PHASE;
		break;
		default:
			err("mysterious mode=\"%s\"", mode);
		}

		/* Output the half-length trace */
		tr.ns = nfby2;
		puttr(&tr);

	} while (gettr(&tr));


	return(CWP_Exit());
}
