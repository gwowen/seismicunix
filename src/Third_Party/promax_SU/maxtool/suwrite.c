/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

/* Copyright (c) Colorado School of Mines, 2003.*/
/* All rights reserved.                       */

/***************************************************************************
SUWRITE:  write SU output stream
***************************************************************************
code by J.E. Anderson, Mobil/CWP (June, 1993)
modified by J.E. Anderson, April, 1995, updated trace header words 
	and internal documentation
modified by S.A. Levin Landmark (December, 2008) added control of endianness
        of output via XDR parameter.
***************************************************************************/
#include     <stdlib.h>
#include     <unistd.h>
#include     <stdio.h>
#include     "cwpsegy.h"
#include     <cpromax.h>
#include     <pwd.h>
#include     <sys/types.h>
#include     <inttypes.h>
#include     <netinet/in.h>
#include     <arpa/inet.h>

void init_suwrite_(int *lensav, int *itooltype);
void exec_suwrite_(float *trace, int *ihead, float *rhead);
static void suwrite_putsu(FILE *fp, cwpsegy *tp, long *itr, int *nsegy);
static FILE *suwrite_open(int ichoose, char *cmnd, char *datafile,char *cmndfile);
static void suwrite_hmap_2su(cwpsegy *tr, int *h);
static void suwrite_hdrinit(int *fmt, int *ind);
static void suwrite_geth(int *h,int *fmt, int *ind, int *ihead, float *rhead);
static char *suwrite_stringPrep(char *s);

BEGINPARMS
FILE *fp;
int ichoose;
char *cmnd;
char *cmndfile;
char *datafile;
int input_numsmp;
int input_samprat;
long first;
int  do_xdr;
cwpsegy *tr;
long itr;
int nsegy;
int *fmt;
int *ind;
int *h;
ENDPARMS(parms)

void init_suwrite_(int *lensav, int *itooltype)
/***************************************************************************
initialization phase for SUWRITE
***************************************************************************/
{
	char *temp;
	exParGetInt("CHOOSE1",&parms->ichoose);
	exParGetInt("XDR",&parms->do_xdr);
	exParGetString("CMND",&parms->cmnd);
	exParGetString("CMNDFILE",&temp);
	parms->cmndfile = suwrite_stringPrep(temp);
	exParGetString("DATAFILE",&temp);
	parms->datafile = suwrite_stringPrep(temp);

	parms->input_numsmp  = globalRuntime->numsmp;
	parms->input_samprat = globalRuntime->samprat;
	parms->tr    = (cwpsegy *)malloc(sizeof(cwpsegy));
	parms->fmt   = (int *)malloc(120*sizeof(int));
	parms->ind   = (int *)malloc(120*sizeof(int));
	parms->h     = (int *)malloc(120*sizeof(int)); 
	parms->first = 0;
	parms->itr   = 0;  
	suwrite_hdrinit(parms->fmt,parms->ind); 
	*lensav = NPARMS(parms);
	*itooltype = ISIMPLE;
	return;
}

void exec_suwrite_(float *trace, int *ihead, float *rhead)
/***************************************************************************
execution phase for SUWRITE
***************************************************************************/
{
	int j;
	cwpsegy *tr;

	if (globalRuntime->cleanup) {
		if(parms->ichoose==0) {
			j=pclose(parms->fp);
		}else{
			j=fclose(parms->fp);
		}
		(void) free(parms->h);
		(void) free(parms->ind);
		(void) free(parms->fmt);
		(void) free(parms->tr);
		return;
	}
	tr=parms->tr;

	if(parms->first==0) 
		parms->fp=suwrite_open(parms->ichoose,parms->cmnd,
			parms->datafile,parms->cmndfile);
	parms->first++;
	suwrite_geth(parms->h,parms->fmt, parms->ind,ihead,rhead);
	suwrite_hmap_2su(tr, parms->h);  
	tr->tracl=parms->first;
	tr->ns=parms->input_numsmp;
	tr->dt=1000*parms->input_samprat;
	tr->d2=1;
        if(parms->do_xdr) {
	   for(j=0;j<parms->input_numsmp;j++) (*((uint32_t *) (tr->data+j)))=htonl(*((uint32_t *) (trace+j)));
        } else {
	   for(j=0;j<parms->input_numsmp;j++) tr->data[j]=trace[j];
        }
        if(parms->do_xdr) {
            tr->tracl = htonl(tr->tracl);
            tr->tracr = htonl(tr->tracr);
            tr->fldr = htonl(tr->fldr);
            tr->tracf = htonl(tr->tracf);
            tr->ep = htonl(tr->ep);
            tr->cdp = htonl(tr->cdp);
            tr->trid = htons(tr->trid);
            tr->nvs = htons(tr->nvs);
            tr->nhs = htons(tr->nhs);
            tr->duse = htons(tr->duse);
            tr->offset = htonl(tr->offset);
            tr->gelev = htonl(tr->gelev);
            tr->selev = htonl(tr->selev);
            tr->sdepth = htonl(tr->sdepth);
            tr->gdel = htonl(tr->gdel);
            tr->sdel = htonl(tr->sdel);
            tr->swdep = htonl(tr->swdep);
            tr->gwdep = htonl(tr->gwdep);
            tr->scalel = htons(tr->scalel);
            tr->scalco = htons(tr->scalco);
            tr->sx = htonl(tr->sx);
            tr->sy = htonl(tr->sy);
            tr->gx = htonl(tr->gx);
            tr->gy = htonl(tr->gy);
            tr->counit = htons(tr->counit);
            tr->wevel = htons(tr->wevel);
            tr->swevel = htons(tr->swevel);
            tr->sut = htons(tr->sut);
            tr->gut = htons(tr->gut);
            tr->sstat = htons(tr->sstat);
            tr->gstat = htons(tr->gstat);
            tr->tstat = htons(tr->tstat);
            tr->laga = htons(tr->laga);
            tr->lagb = htons(tr->lagb);
            tr->delrt = htons(tr->delrt);
            tr->muts = htons(tr->muts);
            tr->mute = htons(tr->mute);
            tr->ns = htons(tr->ns);
            tr->dt = htons(tr->dt);
            tr->gain = htons(tr->gain);
            tr->igc = htons(tr->igc);
            tr->igi = htons(tr->igi);
            tr->corr = htons(tr->corr);
            tr->sfs = htons(tr->sfs);
            tr->sfe = htons(tr->sfe);
            tr->slen = htons(tr->slen);
            tr->styp = htons(tr->styp);
            tr->stas = htons(tr->stas);
            tr->stae = htons(tr->stae);
            tr->tatyp = htons(tr->tatyp);
            tr->afilf = htons(tr->afilf);
            tr->afils = htons(tr->afils);
            tr->nofilf = htons(tr->nofilf);
            tr->nofils = htons(tr->nofils);
            tr->lcf = htons(tr->lcf);
            tr->hcf = htons(tr->hcf);
            tr->lcs = htons(tr->lcs);
            tr->hcs = htons(tr->hcs);
            tr->year = htons(tr->year);
            tr->day = htons(tr->day);
            tr->hour = htons(tr->hour);
            tr->minute = htons(tr->minute);
            tr->sec = htons(tr->sec);
            tr->timbas = htons(tr->timbas);
            tr->trwf = htons(tr->trwf);
            tr->grnors = htons(tr->grnors);
            tr->grnofr = htons(tr->grnofr);
            tr->grnlof = htons(tr->grnlof);
            tr->gaps = htons(tr->gaps);
            tr->otrav = htons(tr->otrav);
            tr->d1 = htonl(tr->d1);
            tr->f1 = htonl(tr->f1);
            tr->d2 = htonl(tr->d2);
            tr->f2 = htonl(tr->f2);
            tr->ungpow = htonl(tr->ungpow);
            tr->unscale = htonl(tr->unscale);
            tr->mark = htons(tr->mark);
            tr->unass[0] = htons(tr->unass[0]);
            tr->unass[1] = htons(tr->unass[1]);
            tr->unass[2] = htons(tr->unass[2]);
            tr->unass[3] = htons(tr->unass[3]);
            tr->unass[4] = htons(tr->unass[4]);
            tr->unass[5] = htons(tr->unass[5]);
            tr->unass[6] = htons(tr->unass[6]);
            tr->unass[7] = htons(tr->unass[7]);
            tr->unass[8] = htons(tr->unass[8]);
            tr->unass[9] = htons(tr->unass[9]);
            tr->unass[10] = htons(tr->unass[10]);
            tr->unass[11] = htons(tr->unass[11]);
            tr->unass[12] = htons(tr->unass[12]);
            tr->unass[13] = htons(tr->unass[13]);
            tr->unass[14] = htons(tr->unass[14]);
            tr->unass[15] = htons(tr->unass[15]);
            tr->unass[16] = htons(tr->unass[16]);
            tr->unass[17] = htons(tr->unass[17]);
        }
	suwrite_putsu(parms->fp,tr,&parms->itr,&parms->nsegy);
	return;
}

FILE *suwrite_open(int ichoose,char *cmnd,char *datafile,char *cmndfile)
/**************************************************************************************
return a FILE pointer to  output stream whether from Unix command, file, or command file

int ichoose	0	write to output pipe
		1	write to output data file
		2	write to output command file (input to suget)
char *cmnd	command setting up output pipe
char *datafile	output data file
char *cmndfile	name of command file piping output on file descriptor
		given for argument 1 (usually for use with suget)

************************************************************************/
{
	FILE *fp;
	int pfd[2],j,k;
	char fdstr[10],prog[50];
	if(ichoose==0) {
		fprintf(stderr,"\nSUWRITE Pipe command:\n %s\n",cmnd);
		fp=popen(cmnd,"w");
		if(fp==NULL) 
			exErrFatal("suwrite_open: Could not open output pipe to %s\n",cmnd);
		return(fp);
	}
	if(ichoose==1) {
		fp=fopen(datafile,"w");
		if(fp==NULL) 
			exErrFatal("suwrite_open: Could not open output file %s\n",datafile);
		return (fp);
	}
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
		fprintf(stderr,"suwrite_open: Could not open pipe.\n");
		return(NULL);
		}
	switch(fork()){
	case -1: 
		fprintf(stderr,"suwrite_open: Fork failed to create child.\n");
		return(NULL);
	case 0:
		if(close(pfd[1])==-1)
			fprintf(stderr,"suwrite_open: Non-fatal close error in child.\n");
/*		fprintf(stderr,"suwrite_open: File descriptor passed from parent = %d\n",pfd[0]); */
		sprintf(fdstr,"%d",pfd[0]);
		execlp(cmndfile,prog,fdstr,NULL);
		fprintf(stderr,"suwrite_open: Failed to execute command file %s. Path = %s.\n",
			prog,cmndfile);
		return(NULL);
	}
	if(close(pfd[0])==-1) 
		fprintf(stderr,"suwrite_open: Non-fatal close error in parent.\n");
/*	fprintf(stderr,"suwrite_open: Parent will write on file descriptor %d\n",pfd[1]); */
	fp=fdopen(pfd[1],"w");
	return(fp);
}
void suwrite_hdrinit(int *fmt, int *ind)
/************************************************************************
initialize trace header map from ProMAX to SU in terms of format and indecies
************************************************************************/
{
	int j;
	j=0;
	while( (sumap[j].suoffs>-1) && (sumap[j].suoffs<240) ) {
		if(hdrExists(sumap[j].pkey)) {
			fmt[j]=hdrFormat(sumap[j].pkey);
			ind[j]=hdrIndex(sumap[j].pkey);
			fprintf(stderr,"  output SU hdr=%s   input ProMAX hdr=%s\n",
				sumap[j].sukey,sumap[j].pkey);
		}else{
			fmt[j]=HDRUNDEFINED;
			ind[j]=-1;
		}

		j++;
	}
	return;
}
void suwrite_geth(int *h, int *fmt, int *ind, int *ihead, float *rhead)
/************************************************************************
extract ProMAX trace header values into h array
************************************************************************/
{
	int j;
	j=0;
	while( (sumap[j].suoffs>-1) && (sumap[j].suoffs<240) ) {
		if(fmt[j]==HDRINT) {
			h[j]=ihead[ind[j]];
		}else if(fmt[j]==HDRFLOAT) {
			h[j]=rhead[ind[j]];
		}else{
			h[j]=0;
		}
		j++;
	}
	return;		
}

void suwrite_hmap_2su(cwpsegy *tr, int *h)
/************************************************************************
move header values into SU header from h array
************************************************************************/
{
tr->tracl  = h[0];
tr->tracr  = h[1];
tr->fldr   = h[2];
tr->tracf  = h[3];
tr->ep     = h[4];
tr->cdp    = h[5];
tr->cdpt   = h[6];
tr->trid   = h[7];
tr->nvs    = h[8];
tr->nhs    = h[9];
tr->duse   = h[10];
tr->offset = h[11];
tr->gelev  = h[12];
tr->selev  = h[13];
tr->sdepth = h[14];
tr->gdel   = h[15];
tr->sdel   = h[16];
tr->swdep  = h[17];
tr->gwdep  = h[18];
tr->scalel = 1;
tr->scalco = 1;
tr->sx     = h[21];
tr->sy     = h[22];
tr->gx     = h[23];
tr->gy     = h[24];
tr->counit = h[25];
tr->wevel  = h[26];
tr->swevel = h[27];
tr->sut    = h[28];
tr->gut    = h[29];
tr->sstat  = h[30];
tr->gstat  = h[31];
tr->tstat  = h[32];
tr->laga   = h[33];
tr->lagb   = h[34];
tr->delrt  = h[35];
tr->muts   = h[36];
tr->mute   = h[37];
tr->ns     = globalRuntime->numsmp;
tr->dt     = 1000*globalRuntime->samprat;
tr->gain   = h[40];
tr->igc    = h[41];
tr->igi    = h[42];
tr->corr   = h[43];
tr->sfs    = h[44];
tr->sfe    = h[45];
tr->slen   = h[46];
tr->styp   = h[47];
tr->stas   = h[48];
tr->stae   = h[49];
tr->tatyp  = h[50];
tr->afilf  = h[51];
tr->afils  = h[52];
tr->nofilf = h[53];
tr->nofils = h[54];
tr->lcf    = h[55];
tr->hcf    = h[56];
tr->lcs    = h[57];
tr->hcs    = h[58];
tr->year   = h[59];
tr->day    = h[60];
tr->hour   = h[61];
tr->minute = h[62];
tr->sec    = h[63];
tr->timbas = h[64];
tr->trwf   = h[65];
tr->grnors = h[66];
tr->grnofr = h[67];
tr->grnlof = h[68];
tr->gaps   = h[69];
tr->otrav  = h[70];
tr->d1     = globalRuntime->samprat;
tr->f1     = 0;
tr->d2     = 1;
tr->f2     = 0;
tr->ungpow = h[75];
tr->unscale= h[76];
tr->mark   = h[77];
tr->unass[0]  = h[78];
tr->unass[1]  = h[79];
tr->unass[2]  = h[80];
tr->unass[3]  = h[81];
tr->unass[4]  = h[82];
tr->unass[5]  = h[83];
tr->unass[6]  = h[84];
tr->unass[7]  = h[85];
tr->unass[8]  = h[86];
tr->unass[9]  = h[87];
tr->unass[10] = h[88];
tr->unass[11] = h[89];
tr->unass[12] = h[90];
tr->unass[13] = h[91];
tr->unass[14] = h[92];
tr->unass[15] = h[93];
tr->unass[16] = h[94];
return;
}

void suwrite_putsu(FILE *fp, cwpsegy *tp, long *itr, int *nsegy)
/************************************************************************
suwrite_putsu - 	put a segy trace on a file by descriptor
		(based on fputtr but made reentrant)
************************************************************************/
{
	if (*itr== 0)
		nsegy = HDRBYTES + sizeof(float) * ((parms->do_xdr) ? ntohs(tp->ns) : tp->ns);
	(void) fwrite(tp, 1, *nsegy, fp);
	*itr++;
	return;
}
char *suwrite_stringPrep(char *s)
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
