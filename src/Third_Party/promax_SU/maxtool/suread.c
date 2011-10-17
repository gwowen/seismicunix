/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

/* Copyright (c) Colorado School of Mines, 2003.*/
/* All rights reserved.                       */

/***************************************************************************
SUREAD:  read SU input stream
***************************************************************************
code by J.E. Anderson, Mobil/CWP (June, 1993)
modified by J.E. Anderson, April, 1995, updated trace header words 
	and internal documentation
modified by S.A. Levin Landmark (December, 2008) handle endianness issues
        with automagic and explicit XDR options

***************************************************************************/
#include     <stdlib.h>
#include     <unistd.h>
#include     <stdio.h>
#include     <math.h>
#include     "cwpsegy.h"
#include     <cpromax.h>
#include     <pwd.h>

void init_suread_(int *lensav, int *itooltype);
void exec_suread_(float *trace, int *ihead, float *rhead);
static FILE *suread_open(int ichoose,char *cmnd,char *path,char *prog); 
static void suread_inith(int *ind);
static void suread_puth(cwpsegy *tr, int *ihead,float *rhead,int *ind);
static char *suread_stringPrep(char *s);
#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#define swap2h(N) *((unsigned int *) (&N))=(((unsigned int) (((unsigned short *) (&N))[0])) + (((unsigned int) 65536)*((unsigned int) (((unsigned short *)(&N))[1]))))

BEGINPARMS
FILE *fp;
char *cmnd;
char *datafile;
char *cmndfile;
int ichoose;
int ipsort;
int idtyp;
int nread;
int nbytes;
int maxdtr;
int ipkey;
int iskey;
int do_swap;
float samprat;
long numsmp;
long iens;
long ens_value;
long tr_in_ens;
int *ind;
cwpsegy *tr;
ENDPARMS(parms)

void init_suread_(int *lensav, int *itooltype)
/***************************************************************************
initialization phase for SUREAD
***************************************************************************/
{
	char *temp;
        int itemp;
	exParGetInt("CHOOSE1",&parms->ichoose);
	exParGetString("CMND",&parms->cmnd);
	exParGetString("CMNDFILE",&temp);
	parms->cmndfile = suread_stringPrep(temp);
	exParGetString("DATAFILE",&temp);
	parms->datafile = suread_stringPrep(temp);
	exParGetInt("SORT",&parms->ipsort);
	exParGetInt("TYPE",&parms->idtyp);
	exParGetInt("NPENS",&parms->maxdtr);
        exParGetInt("XDR",&itemp);
	suread_stringPrep(parms->cmndfile);
	suread_stringPrep(parms->datafile);

/***************************************************************************
Allocate save buffers and open the input stream
***************************************************************************/
	parms->tr=(cwpsegy *)malloc(sizeof(cwpsegy));
	parms->ind=(int *)malloc(120*sizeof(int));
	parms->fp=suread_open(parms->ichoose,parms->cmnd,
				parms->datafile,parms->cmndfile);

/***************************************************************************
Read the first trace header.  Then read the trace data for the first trace.
***************************************************************************/
	parms->nread=fread(parms->tr,1,240,parms->fp);
	if(!parms->nread)
		exErrFatal("suread: Could not get first trace header.");

        if(parms->tr->trid != 0)
           parms->do_swap = (parms->tr->trid < 0 || parms->tr->trid > 255);
        else if(parms->tr->d1 != 0.0f)
	   parms->do_swap = (fabs(parms->tr->d1) < 1.0e-36);
	else
           parms->do_swap = 0;
        if(itemp == 1) parms->do_swap = 0;
        if(itemp == 2) parms->do_swap = 1;

        if(parms->do_swap) {
           swab((char *) parms->tr, (char *) parms->tr, 240);
           swap2h(parms->tr->tracl);
           swap2h(parms->tr->tracr);
           swap2h(parms->tr->fldr);
           swap2h(parms->tr->tracf);
           swap2h(parms->tr->ep);
           swap2h(parms->tr->cdp);
           swap2h(parms->tr->cdpt);
           swap2h(parms->tr->offset);
           swap2h(parms->tr->gelev);
           swap2h(parms->tr->selev);
           swap2h(parms->tr->sdepth);
           swap2h(parms->tr->gdel);
           swap2h(parms->tr->sdel);
           swap2h(parms->tr->swdep);
           swap2h(parms->tr->gwdep);
           swap2h(parms->tr->sx);
           swap2h(parms->tr->sy);
           swap2h(parms->tr->gx);
           swap2h(parms->tr->gy);
           swap2h(parms->tr->d1);
           swap2h(parms->tr->f1);
           swap2h(parms->tr->d2);
           swap2h(parms->tr->f2);
           swap2h(parms->tr->ungpow);
           swap2h(parms->tr->unscale);
        }
	parms->nbytes=parms->tr->ns*sizeof(float);
	parms->nread=fread(&parms->tr->data[0],1,parms->nbytes,parms->fp);
	if(!parms->nread)
		exErrFatal("suread: Could not get data for first trace.");
	if(parms->nread!=parms->nbytes)
		exErrFatal("suread: Read wrong number of bytes on first trace");
        if(parms->do_swap) {
            swab((char *) parms->tr->data, (char *) parms->tr->data, parms->nread);
            for(int i=0; i < parms->tr->ns; ++i) {
               swap2h(parms->tr->data[i]);
	    }
           
        }
	parms->nbytes+=240;
	parms->nread=parms->nbytes;
	fprintf(stderr,
"Successfully read first trace. Sample rate = %d usec.  Number of samples = %d.\n",
parms->tr->dt,parms->tr->ns);
	parms->samprat=0.001*parms->tr->dt;
	parms->numsmp=parms->tr->ns;

/***************************************************************************
Initialize ensemble, common block and trace header information
***************************************************************************/
	parms->iens=1;
	parms->tr_in_ens=1;
	suread_inith(parms->ind);
	parms->iskey=1+hdrIndex("SEQNO");
	switch(parms->ipsort){
	case ICDP:
		parms->ipkey=1+hdrIndex("CDP");
		parms->ens_value=parms->tr->cdp;
		fprintf(stderr,"suread: Input ensembles based on cdp\n");
		break;
	case ISIN:
		parms->ipkey=1+hdrIndex("SIN");
		parms->ens_value=parms->tr->fldr;
		fprintf(stderr,"suread: Input ensembles based on fldr\n");
		break;
	case IRECSLOC:
		parms->ipkey=1+hdrIndex("REC_SLOC");
		parms->ens_value=parms->tr->wevel;
		fprintf(stderr,"suread: Input ensembles based on wevel\n");
		break;
	case IOFFSET:
		parms->ipkey=1+hdrIndex("OFFSET");
		parms->ens_value=parms->tr->offset;
		fprintf(stderr,"suread: Input ensembles based on offset\n");
		break;
	case ICHAN:
		parms->ipkey=1+hdrIndex("CHAN");
		parms->ens_value=parms->tr->tracf;
		fprintf(stderr,"suread: Input ensembles based on tracf\n");
		break;
	case IUNKNOWN:
		parms->ipkey=1+hdrIndex("TRACENO");
		parms->ens_value=parms->tr->tracl;
		fprintf(stderr,"suread: Input ensembles based on tracl\n");
		break;
	}
	globalRuntime->numsmp     	= parms->numsmp;
	globalRuntime->samprat     		= parms->samprat;
	globalRuntime->igeom_match 	= FALSE;
	globalRuntime->itrno_valid 		= FALSE;
	globalRuntime->idomain     		= ITX;
	globalRuntime->maxdtr      		= parms->maxdtr;
	globalRuntime->ipsort  		= parms->ipsort;
	globalRuntime->idtyp   		= parms->idtyp;
	globalRuntime->iskey		= parms->iskey;
	globalRuntime->ipkey		= parms->ipkey;

/***************************************************************************
Return the length of the saved common block and type of module.
***************************************************************************/
	*lensav = NPARMS(parms);
	*itooltype = INPUT;
	return;
}

void exec_suread_(float *trace, int *ihead, float *rhead)
/***************************************************************************
execution phase for SUREAD
***************************************************************************/
{
	int j; 
	long ens;

/***************************************************************************
Check to see if this is the last pass or the cleanup pass and close files, etc.
***************************************************************************/
	if (globalRuntime->cleanup) {
		(void)free(parms->ind);
		return;
	}
	if(!parms->nread){
		if(parms->ichoose==0) {
			j=pclose(parms->fp);
		}else{
			j=fclose(parms->fp);
		}
		exPipeMode();
		return;
	}

/***************************************************************************
Fix up the output trace header and output trace from information in save buffer
***************************************************************************/
	suread_puth(parms->tr,ihead,rhead,parms->ind);
	ihead[STDHDR(isource)]  = parms->iens;
	ihead[STDHDR(iseqno)]   = parms->tr_in_ens;
	rhead[STDHDR(itlive_s)] = 0.;
	rhead[STDHDR(itfull_s)] = 0.;
	rhead[STDHDR(itlive_e)] = 
		(globalRuntime->numsmp-1)*globalRuntime->samprat;
	rhead[STDHDR(itfull_e)] = 
		(globalRuntime->numsmp-1)*globalRuntime->samprat;
	for(j=0;j<globalRuntime->numsmp;j++)
		trace[j]=parms->tr->data[j];

readnext:
/***************************************************************************
Read one trace ahead for next pass.  Always need to know ahead of time 
about changes to a new ensemble or if an end of file will be encountered to
set end of ensemble and end of data trace header flags for current trace
so have to always check values for one trace ahead of where current trace 
is being output.  Zero buffer prior to read so traces shorter than first
can be read ok.  Have to bomb if encounter a trace longer than first trace.
***************************************************************************/
	for(j=0;j<globalRuntime->numsmp;j++) parms->tr->data[j]=0.;
	parms->nread = fread(parms->tr,1,parms->nbytes,parms->fp);
	if(parms->nread==0) {
		fprintf(stderr,"suread: Hit EOF on input stream.\n");
		fprintf(stderr,"suread: Read %ld ensembles.\n",parms->iens);
		fprintf(stderr,"suread: Key for last ensemble equals %ld\n",parms->ens_value);
	}else if(parms->nread!=parms->nbytes) {
		fprintf(stderr,"suread: Read wrong number of bytes from input stream.\n");
		fprintf(stderr,"suread: Can not handle variable length records!\n");
		if(parms->nread>parms->nbytes) 
			exErrFatal("suread: encountered trace longer than first trace read");
	}
        if(parms->do_swap) {
           swab((char *) parms->tr, (char *) parms->tr, parms->nread);
           swap2h(parms->tr->tracl);
           swap2h(parms->tr->tracr);
           swap2h(parms->tr->fldr);
           swap2h(parms->tr->tracf);
           swap2h(parms->tr->ep);
           swap2h(parms->tr->cdp);
           swap2h(parms->tr->cdpt);
           swap2h(parms->tr->offset);
           swap2h(parms->tr->gelev);
           swap2h(parms->tr->selev);
           swap2h(parms->tr->sdepth);
           swap2h(parms->tr->gdel);
           swap2h(parms->tr->sdel);
           swap2h(parms->tr->swdep);
           swap2h(parms->tr->gwdep);
           swap2h(parms->tr->sx);
           swap2h(parms->tr->sy);
           swap2h(parms->tr->gx);
           swap2h(parms->tr->gy);
           swap2h(parms->tr->d1);
           swap2h(parms->tr->f1);
           swap2h(parms->tr->d2);
           swap2h(parms->tr->f2);
           swap2h(parms->tr->ungpow);
           swap2h(parms->tr->unscale);
           for(int i=0; i < parms->tr->ns; ++i) {
               swap2h(parms->tr->data[i]);
	   }
        }
	if(parms->nread) {
		switch(parms->ipsort){
		case ICDP: 
			ens = parms->tr->cdp;
			break;
		case ISIN:
			ens = parms->tr->fldr;
			break;
		case IRECSLOC:
			ens = parms->tr->wevel;
			break;
		case IOFFSET:
			ens = parms->tr->offset;
			break;
		case ICHAN:
			ens = parms->tr->tracf;
			break;
		case IUNKNOWN:
			ens = parms->tr->tracl;
			break;
		}
		if(parms->ens_value!=ens) {
/***************************************************************************
			Encountered new ensemble.  
			Current trace is at end of ensemble.
***************************************************************************/
			if(parms->tr_in_ens>globalRuntime->maxdtr) 
				fprintf(stderr,
				"Too many traces for ensemble %ld. Value =%ld. Current count = %d.\n",
				parms->iens,parms->ens_value,parms->tr_in_ens);
			parms->ens_value=ens;
			parms->tr_in_ens=1;
			parms->iens++;
			ihead[STDHDR(iend_ens)]=LASTTR;
			}
		else {
/***************************************************************************
			Next trace part of same ensemble.
***************************************************************************/
			parms->tr_in_ens++;
			ihead[STDHDR(iend_ens)]=NLAST;
			}
		ihead[STDHDR(ieoj)]=NLAST;
	} else {
/***************************************************************************
		Hit end of file.
***************************************************************************/
		if(parms->tr_in_ens>globalRuntime->maxdtr) 
			fprintf(stderr,
			"Too many traces for ensemble %ld.  Current count = %d\n",
			parms->ens_value,parms->tr_in_ens);
		parms->tr_in_ens=1;
		ihead[STDHDR(iend_ens)]=LASTTR;
		ihead[STDHDR(ieoj)]=LASTTR;
		if(parms->ichoose==0) {
			j=pclose(parms->fp);
		}else{
			j=fclose(parms->fp);
		}
		exPipeMode();
		return;
	}
	if(parms->tr_in_ens>globalRuntime->maxdtr) goto readnext;

	exFlushMode();		
	return;
}

FILE *suread_open(int ichoose,char *cmnd,char *datafile,char *cmndfile)
/**************************************************************************************
return a FILE pointer to input stream whether from Unix command, file, or command file

int ichoose	0	read input pipe
		1	read input data file
		2	read from input command file (output of suput)
char *cmnd	command setting up input pipe
char *datafile	input data file
char *cmndfile	name of command file piping output on file descriptor
		returned for argument 1 (usually from suput)

************************************************************************/
{
	FILE *fp;
	char prog[50];
	int pfd[2];
	char fdstr[10];
	int j,k,l;

	if(ichoose==0) {
		fprintf(stderr,"\nsuread input pipe: \n%s\n",cmnd);
		fp=popen(cmnd,"r");
		if(fp==NULL) exErrFatal("Couldn't open input");
		return (fp);
		}

	if(ichoose==1) {
		fprintf(stderr,"suread input data file: %s\n",datafile);
		fp=fopen(datafile,"r");
		if(fp==NULL) exErrFatal("Couldn't open input");
		return (fp);
		}
	fprintf(stderr,"suread input command file: %s\n",cmndfile);
	j=0;
	k=-1;
	while(cmndfile[j]!='\0'){ 
		if(cmndfile[j]=='/') k=j; 
		j++;
		}
	k++;
	j=0;
	do{
		prog[j]=cmndfile[j+k];
		j++;
		}while(cmndfile[j+k]!='\0'); 
	if(pipe(pfd)==-1)
		{
		fprintf(stderr,"Could not open pipe in suread_open\n");
		return(NULL);
		}
	switch(fork()){
	case -1: 
		fprintf(stderr,"suread_open: Fork failed to create child.\n");
		return(NULL);
	case 0:
		if(close(pfd[0])==-1)
			fprintf(stderr,"suread_open: Non-fatal close error in child.\n");
/*		fprintf(stderr,"suread_open: File descriptor passed from parent = %d\n",pfd[1]); */
		sprintf(fdstr,"%d",pfd[1]);
		fprintf(stderr,"suread_open: prog = %s cmndfile = %s \n",prog,cmndfile);
		execl(cmndfile,prog,fdstr,NULL);
		fprintf(stderr,"suread_open: Failed to execute command file %s \n",prog);
		return(NULL);
	}
	if(close(pfd[1])==-1) 
		fprintf(stderr,"suread_open: Non-fatal close error in parent.\n");
/*	fprintf(stderr,"suread_open: Parent reads from file descriptor %d\n",pfd[0]); */
	fp=fdopen(pfd[0],"r");
	return(fp);
}
void suread_inith(int *ind)
/************************************************************************
initialize ProMAX trace header values and return indecies for later access

int *ind		returns as array of ProMAX trace header indecies
*************************************************************************/
{
   int j,k;
   defStdHdr();
   j=0;
   while( (sumap[j].suoffs>-1) && (sumap[j].suoffs<240) ) {
      if(sumap[j].pkey[0]=='S' && sumap[j].pkey[1]=='U'){
        ind[j]=hdrAdd(sumap[j].pkey, sumap[j].desc, 1, (int)sumap[j].ptype);
      }else{
        hdrAddStd(sumap[j].pkey);
        ind[j]=hdrIndex(sumap[j].pkey);
      }
      j++;	
   }
   hdrAddStd("TFULL_E");
   hdrAddStd("TLIVE_E");
}
void suread_puth(cwpsegy *tr,int *ihead,float *rhead,int *ind)
/************************************************************************
move SU header values into ProMAX trace header values 

cwpsegy *tr		input SU trace header structure
int *ihead	output ProMAX integer trace headers
float *rhead	output ProMAX real trace headers
int *ind		input ProMAX  trace header indecies
*************************************************************************/
{
   char *tp = (char *)tr;
   int j;
   long iv;
   float rv;
   initStdHdr(ihead,rhead);
   j=0;
   while( (sumap[j].suoffs>-1) && (sumap[j].suoffs<240) ) {
     if(sumap[j].sutype==1) {
	iv = *( (long *) (tp + sumap[j].suoffs) );
	if(sumap[j].ptype==1)
		ihead[ind[j]]=iv;
	else
		rhead[ind[j]]=iv;
	}
     else if(sumap[j].sutype==0) {
	iv = *( (short *) (tp + sumap[j].suoffs) );
	if(sumap[j].ptype==1)
		ihead[ind[j]]=iv;
	else
		rhead[ind[j]]=iv;
	}
     else if(sumap[j].sutype==3) {
	iv = *( (unsigned short *) (tp + sumap[j].suoffs) );
	if(sumap[j].ptype==1)
		ihead[ind[j]]=iv;
	else
		rhead[ind[j]]=iv;
	}
     else if(sumap[j].sutype==2) {
	rv = *( (float *) (tp + sumap[j].suoffs) );
	if(sumap[j].ptype==1)
		ihead[ind[j]]=rv;
	else
		rhead[ind[j]]=rv;
	}
     j++;
     }

    /*  take care of elevation and coordinate scalers */
     if(tr->scalel<0) {
	rhead[STDHDR(irec_elev)]/=ABS(tr->scalel);
	rhead[STDHDR(isou_elev)]/=ABS(tr->scalel);
     }
     if(tr->scalel>1) {
	rhead[STDHDR(irec_elev)]*=tr->scalel;
	rhead[STDHDR(isou_elev)]*=tr->scalel;
     }
     if(tr->scalco<0) {
	rhead[STDHDR(irec_x)]/=ABS(tr->scalco);
	rhead[STDHDR(irec_y)]/=ABS(tr->scalco);
	rhead[STDHDR(isou_x)]/=ABS(tr->scalco);
	rhead[STDHDR(isou_y)]/=ABS(tr->scalco);
     }
     if(tr->scalco>1) {
	rhead[STDHDR(irec_x)]*=tr->scalco;
	rhead[STDHDR(irec_y)]*=tr->scalco;
	rhead[STDHDR(isou_x)]*=tr->scalco;
	rhead[STDHDR(isou_y)]*=tr->scalco;
     }
     rhead[STDHDR(iaoffset)]=sqrt( 		( rhead[STDHDR(irec_x)]- rhead[STDHDR(isou_x)] )*
					( rhead[STDHDR(irec_x)]- rhead[STDHDR(isou_x)] ) +
					( rhead[STDHDR(irec_y)]- rhead[STDHDR(isou_y)] )*
					( rhead[STDHDR(irec_y)]- rhead[STDHDR(isou_y)] ) );
     return;
}
char *suread_stringPrep(char *s)
/*************************************************************************************
returns properly prepared version of user-entered string for decoding a file name
(allows user to get away with hitting a return in field for file name or an extra space)

char *s		input string

**************************************************************************************/
{
	
	int n,j,c;
	char *ss;

/***********************************
	left justify string
***********************************/
	n=strlen(s);
	for(j=0;j<n;j++) {
		c=s[j];
		if(!isspace(c)) break;	
	}
	n=n-j;
	ss=(char *)malloc( (n+1)*sizeof(char));
	for(c=0;c<n;c++) ss[c]=s[c+j];
	ss[n]='\0';			

/****************************************************
	convert any remaining white space to nulls
****************************************************/
	for(j=0;j<n;j++) {
		c=ss[j];
		if(isspace(c)) ss[j]='\0';
	}

	return (ss);
}
