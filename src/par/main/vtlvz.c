/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

/* VTLVZ: $Revision: 1.7 $ ; $Date: 2003/08/19 21:24:44 $	*/

#include "par.h"

/*********************** self documentation **********************/
char *sdoc[] = {
"									",
" VTLVZ -- Velocity as function of Time for Linear V(Z);		",
"          writes out a vector of velocity = v0 exp(a t/2)		",
"									",
" vtlvz > velfile nt= dt= v0= a= 					",
"									",
" Required parameters							",
" nt=	number of time samples						",
" dt=	time sampling interval						",
" v0=	velocity at the surface						",
" a=	velocity gradient						",
"									",
NULL};
/**************** end self doc ********************************/

int
main(int argc, char **argv)
{
	int	it,nt;
	float	v0,a,dt,t,*v;

	/* hook up getpar to handle the parameters */
	initargs(argc,argv);
	requestdoc(1);

	/* get parameters */
	MUSTGETPARINT("nt",&nt);
	MUSTGETPARFLOAT("dt",&dt);
	MUSTGETPARFLOAT("v0",&v0);
	MUSTGETPARFLOAT("a",&a);
	
	/* allocate space */
	v = alloc1float(nt);

	for (it=0,t=0.0; it<nt; ++it,t+=dt)
		v[it] = v0*exp(a*t/2.0);
	fwrite (v,sizeof(float),nt,stdout);
	
	/* free space */
	free1float(v);

	return(CWP_Exit());
} 
