/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

/* Copyright (c) Colorado School of Mines, 2003.*/
/* All rights reserved.                       */

/* cwpsegy.h - include file for SEGY traces
 *
 * declarations for:
 *	typedef struct {} cwpsegy - the trace identification header
 *	typedef struct {} cwpbhed - binary header
*             typedef struct {} sumap - map between SU and ProMAX headers
 *
 * Note:
 *	If header words are added, run the makefile in this directory
 *	to recreate hdr.h.
 *
 * Reference:
 *	K. M. Barry, D. A. Cavers and C. W. Kneale, "Special Report:
 *		Recommended Standards for Digital Tape Formats",
 *		Geophysics, vol. 40, no. 2 (April 1975), P. 344-352.
 *	
 */ 

#ifndef HEADER_H
#define HEADER_H

#define SU_NKEYS        78      /* Number of key header words */
#define HDRBYTES        240     /* Bytes in the trace header */
#define MAXSEGY         131312

#endif

#ifndef SEGY_H
#define SEGY_H

#define SU_NFLTS	32768	/* Arbitrary limit on data array size	*/


/* TYPEDEFS */
typedef struct {	/* segy - trace identification header */

	int tracl;	/* trace sequence number within line */

	int tracr;	/* trace sequence number within reel */

	int fldr;	/* field record number */

	int tracf;	/* trace number within field record */

	int ep;	/* energy source point number */

	int cdp;	/* CDP ensemble number */

	int cdpt;	/* trace number within CDP ensemble */

	short trid;	/* trace identification code:
			1 = seismic data
			2 = dead
			3 = dummy
			4 = time break
			5 = uphole
			6 = sweep
			7 = timing
			8 = water break
			9---, N = optional use (N = 32,767)

			Following are CWP id flags:

			 9 = autocorrelation

			10 = Fourier transformed - no packing
			     xr[0],xi[0], ..., xr[N-1],xi[N-1]

			11 = Fourier transformed - unpacked Nyquist
			     xr[0],xi[0],...,xr[N/2],xi[N/2]

			12 = Fourier transformed - packed Nyquist
	 		     even N:
			     xr[0],xr[N/2],xr[1],xi[1], ...,
				xr[N/2 -1],xi[N/2 -1]
				(note the exceptional second entry)
			     odd N:
			     xr[0],xr[(N-1)/2],xr[1],xi[1], ...,
				xr[(N-1)/2 -1],xi[(N-1)/2 -1],xi[(N-1)/2]
				(note the exceptional second & last entries)

			13 = Complex signal in the time domain
			     xr[0],xi[0], ..., xr[N-1],xi[N-1]

			14 = Fourier transformed - amplitude/phase
			     a[0],p[0], ..., a[N-1],p[N-1]

			15 = Complex time signal - amplitude/phase
			     a[0],p[0], ..., a[N-1],p[N-1]

			16 = Real part of complex trace from 0 to Nyquist

			17 = Imag part of complex trace from 0 to Nyquist

			18 = Amplitude of complex trace from 0 to Nyquist

			19 = Phase of complex trace from 0 to Nyquist

			21 = Wavenumber time domain (k-t)

			22 = Wavenumber frequency (k-omega)

			23 = Envelope of the complex time trace

			24 = Phase of the complex time trace

			25 = Frequency of the complex time trace

			30 = Depth-Range (z-x) traces

			101 = Seismic data packed to bytes (by supack1)
			
			102 = Seismic data packed to 2 bytes (by supack2)
			*/

	short nvs;	/* number of vertically summed traces (see vscode
			   in bhed structure) */

	short nhs;	/* number of horizontally summed traces (see vscode
			   in bhed structure) */

	short duse;	/* data use:
				1 = production
				2 = test */

	int offset;	/* distance from source point to receiver
			   group (negative if opposite to direction
			   in which the line was shot) */

	int gelev;	/* receiver group elevation from sea level
			   (above sea level is positive) */

	int selev;	/* source elevation from sea level
			   (above sea level is positive) */

	int sdepth;	/* source depth (positive) */

	int gdel;	/* datum elevation at receiver group */

	int sdel;	/* datum elevation at source */

	int swdep;	/* water depth at source */

	int gwdep;	/* water depth at receiver group */

	short scalel;	/* scale factor for previous 7 entries
			   with value plus or minus 10 to the
			   power 0, 1, 2, 3, or 4 (if positive,
			   multiply, if negative divide) */

	short scalco;	/* scale factor for next 4 entries
			   with value plus or minus 10 to the
			   power 0, 1, 2, 3, or 4 (if positive,
			   multiply, if negative divide) */

	int  sx;	/* X source coordinate */

	int  sy;	/* Y source coordinate */

	int  gx;	/* X group coordinate */

	int  gy;	/* Y group coordinate */

	short counit;	/* coordinate units code:
				for previous four entries
				1 = length (meters or feet)
				2 = seconds of arc (in this case, the
				X values are longitude and the Y values
				are latitude, a positive value designates
				the number of seconds east of Greenwich
				or north of the equator */

	short wevel;	/* weathering velocity */

	short swevel;	/* subweathering velocity */

	short sut;	/* uphole time at source */

	short gut;	/* uphole time at receiver group */

	short sstat;	/* source static correction */

	short gstat;	/* group static correction */

	short tstat;	/* total static applied */

	short laga;	/* lag time A, time in ms between end of 240-
			   byte trace identification header and time
			   break, positive if time break occurs after
			   end of header, time break is defined as
			   the initiation pulse which maybe recorded
			   on an auxiliary trace or as otherwise
			   specified by the recording system */

	short lagb;	/* lag time B, time in ms between the time break
			   and the initiation time of the energy source,
			   may be positive or negative */

	short delrt;	/* delay recording time, time in ms between
			   initiation time of energy source and time
			   when recording of data samples begins
			   (for deep water work if recording does not
			   start at zero time) */

	short muts;	/* mute time--start */

	short mute;	/* mute time--end */

	unsigned short ns;	/* number of samples in this trace */

	unsigned short dt;	/* sample interval; in micro-seconds */

	short gain;	/* gain type of field instruments code:
				1 = fixed
				2 = binary
				3 = floating point
				4 ---- N = optional use */

	short igc;	/* instrument gain constant */

	short igi;		/* instrument early or initial gain */

	short corr;	/* correlated:
				1 = no
				2 = yes */

	short sfs;	/* sweep frequency at start */

	short sfe;	/* sweep frequency at end */

	short slen;	/* sweep length in ms */

	short styp;	/* sweep type code:
				1 = linear
				2 = cos-squared
				3 = other */

	short stas;	/* sweep trace length at start in ms */

	short stae;	/* sweep trace length at end in ms */

	short tatyp;	/* taper type: 1=linear, 2=cos^2, 3=other */

	short afilf;	/* alias filter frequency if used */

	short afils;	/* alias filter slope */

	short nofilf;	/* notch filter frequency if used */

	short nofils;	/* notch filter slope */

	short lcf;		/* low cut frequency if used */

	short hcf;	/* high cut frequncy if used */

	short lcs;	/* low cut slope */

	short hcs;	/* high cut slope */

	short year;	/* year data recorded */

	short day;	/* day of year */

	short hour;	/* hour of day (24 hour clock) */

	short minute;	/* minute of hour */

	short sec;	/* second of minute */

	short timbas;	/* time basis code:
				1 = local
				2 = GMT
				3 = other */

	short trwf;	/* trace weighting factor, defined as 1/2^N
			   volts for the least sigificant bit */

	short grnors;	/* geophone group number of roll switch
			   position one */

	short grnofr;	/* geophone group number of trace one within
			   original field record */

	short grnlof;	/* geophone group number of last trace within
			   original field record */

	short gaps;	/* gap size (total number of groups dropped) */

	short otrav;	/* overtravel taper code:
				1 = down (or behind)
				2 = up (or ahead) */

	/* local assignments */
	float d1;	/* sample spacing for non-seismic data */

	float f1;	/* first sample location for non-seismic data */

	float d2;	/* sample spacing between traces */

	float f2;	/* first trace location */

	float ungpow;	/* negative of power used for dynamic
			   range compression */

	float unscale;	/* reciprocal of scaling factor to normalize
			   range */

	short mark;	/* mark selected traces */

	short unass[17];	/* unassigned--NOTE: last entry causes 
			   a break in the word alignment, if we REALLY
			   want to maintain 240 bytes, the following
			   entry should be an odd number of short/UINT2
			   OR do the insertion above the "mark" keyword
			   entry */

	float  data[SU_NFLTS];

} cwpsegy;


typedef struct {	/* bhed - binary header */

	int jobid;	/* job identification number */

	int lino;	/* line number (only one line per reel) */

	int reno;	/* reel number */

	short ntrpr;	/* number of data traces per record */

        short nart;	/* number of auxiliary traces per record */

	short hdt;	/* sample interval in micro secs for this reel */

	short dto;	/* same for original field recording */

	short hns;	/* number of samples per trace for this reel */

	short nso;	/* same for original field recording */

	short format;	/* data sample format code:
				1 = floating point (4 bytes)
				2 = fixed point (4 bytes)
				3 = fixed point (2 bytes)
				4 = fixed point w/gain code (4 bytes) */

	short fold;	/* CDP fold expected per CDP ensemble */

	short tsort;	/* trace sorting code: 
				1 = as recorded (no sorting)
				2 = CDP ensemble
				3 = single fold continuous profile
				4 = horizontally stacked */

	short vscode;	/* vertical sum code:
				1 = no sum
				2 = two sum ...
				N = N sum (N = 32,767) */

	short hsfs;	/* sweep frequency at start */

	short hsfe;	/* sweep frequency at end */

	short hslen;	/* sweep length (ms) */

	short hstyp;	/* sweep type code:
				1 = linear
				2 = parabolic
				3 = exponential
				4 = other */

	short schn;	/* trace number of sweep channel */

	short hstas;	/* sweep trace taper length at start if
			   tapered (the taper starts at zero time
			   and is effective for this length) */

	short hstae;	/* sweep trace taper length at end (the ending
			   taper starts at sweep length minus the taper
			   length at end) */

	short htatyp;	/* sweep trace taper type code:
				1 = linear
				2 = cos-squared
				3 = other */

	short hcorr;	/* correlated data traces code:
				1 = no
				2 = yes */

	short bgrcv;	/* binary gain recovered code:
				1 = yes
				2 = no */

	short rcvm;	/* amplitude recovery method code:
				1 = none
				2 = spherical divergence
				3 = AGC
				4 = other */

	short mfeet;	/* measurement system code:
				1 = meters
				2 = feet */

	short polyt;	/* impulse signal polarity code:
				1 = increase in pressure or upward
				    geophone case movement gives
				    negative number on tape
				2 = increase in pressure or upward
				    geophone case movement gives
				    positive number on tape */

	short vpol;	/* vibratory polarity code:
				code	seismic signal lags pilot by
				1	337.5 to  22.5 degrees
				2	 22.5 to  67.5 degrees
				3	 67.5 to 112.5 degrees
				4	112.5 to 157.5 degrees
				5	157.5 to 202.5 degrees
				6	202.5 to 247.5 degrees
				7	247.5 to 292.5 degrees
				8	293.5 to 337.5 degrees */

	short hunass[170];	/* unassigned */

} cwpbhed;

/* The following refer to the trid field in segy.h		*/
/* CHARPACK represents byte packed seismic data from supack1	*/
#define		CHARPACK	101
/* SHORTPACK represents 2 byte packed seismic data from supack2	*/
#define		SHORTPACK	102
/* TREAL represents real time traces 				*/
#define		TREAL		1
/* TDEAD represents dead time traces 				*/
#define		TDEAD		2
/* TDUMMY represents dummy time traces 				*/
#define		TDUMMY		3
/* TBREAK represents time break traces 				*/
#define		TBREAK		4
/* UPHOLE represents uphole traces 				*/
#define		UPHOLE		5
/* SWEEP represents sweep traces 				*/
#define		SWEEP		6
/* TIMING represents timing traces 				*/
#define		TIMING		7
/* WBREAK represents timing traces 				*/
#define		WBREAK		8

/* TCMPLX represents complex time traces 			*/
#define		TCMPLX		13
/* TAMPH represents time domain data in amplitude/phase form	*/
#define		TAMPH		15
/* FPACK represents packed frequency domain data 		*/
#define		FPACK		12
/* FUNPACKNYQ represents complex frequency domain data 		*/
#define		FUNPACKNYQ	11
/* FCMPLX represents complex frequency domain data 		*/
#define		FCMPLX		10
/* FAMPH represents freq domain data in amplitude/phase form	*/
#define		FAMPH		14
/* REALPART represents the real part of a trace to Nyquist	*/
#define		REALPART	16
/* IMAGPART represents the real part of a trace to Nyquist	*/
#define		IMAGPART	17
/* AMPLITUDE represents the amplitude of a trace to Nyquist	*/
#define		AMPLITUDE	18
/* PHASE represents the phase of a trace to Nyquist		*/
#define		PHASE		19
/* KT represents wavenumber-time domain data 			*/
#define		KT		21
/* KOMEGA represents wavenumber-frequency domain data		*/
#define		KOMEGA		22
/* ENVELOPE represents the envelope of the complex time trace	*/
#define		ENVELOPE	23
/* INSTPHASE represents the phase of the complex time trace	*/
#define		INSTPHASE	24
/* INSTFREQ represents the frequency of the complex time trace	*/
#define		INSTFREQ	25
/* DEPTH represents traces in depth-range (z-x)			*/
#define		TRID_DEPTH	30

#define ISSEISMIC(id) ( (id)==0 || (id)==TREAL || (id)==TDEAD || (id)==TDUMMY )

#endif


#define INT2 0
#define INT4 1
#define IREAL4 2
#define UINT2 3

static struct 
{char *sukey; 	char sutype;	int suoffs;	char *pkey;	char ptype;	char *desc;} 
sumap[] = {
{"tracl",		INT4,		0,	"TRACENO ",	INT4,		"Trace number in seismic line    "},
{"tracr",		INT4,		4,	"SOURCE  ",	INT4,		"Live source number(usr-defined) "},
{"fldr",		INT4,		8,	"FFID    ",	INT4,		"Field file ID number            "},
{"tracf",		INT4,		12,	"CHAN    ",	INT4,		"Recording channel number        "},
{"ep",		INT4,		16,	"SIN     ",	INT4,		"Source index number (internal)  "},
{"cdp",		INT4,		20,	"CDP     ",	INT4,		"CDP bin number                  "},
{"cdpt",		INT4,		24,	"SEQNO   ",	INT4,		"Sequence number in ensemble     "},
{"trid",		INT2,		28,	"TRC_TYPE",	INT4,		"Trace type (data, aux, etc.)    "},
{"nvs",		INT2,		30,	"SU_NVS  ",	INT4,		"SU header: number vert stack    "},
{"nhs",		INT2,		32,	"TR_FOLD ",	IREAL4,		"Actual trace fold               "},
{"duse",		INT2,		34,	"SU_DUSE ",	INT4,		"SU header: data use: 1=prod     "},
{"offset",		INT4,		36,	"OFFSET  ",	IREAL4,		"Signed source-receiver offset   "},
{"gelev",		INT4,		40,	"REC_ELEV",	IREAL4,		"Receiver elevation              "},
{"selev",		INT4,		44,	"SOU_ELEV",	IREAL4,		"Source elevation                "},
{"sdepth",	INT4,		48,	"DEPTH   ",	IREAL4,		"Source depth                    "},
{"gdel",		INT4,		52,	"SU_GDEL ",	IREAL4,		"SU header: rec datum elev       "},
{"sdel",		INT4,		56,	"SU_SDEL ",	IREAL4,		"SU header: source datum elev    "},
{"swdep",	INT4,		60,	"SOU_H2OD",	IREAL4,		"Water depth at source           "},
{"gwdep",	INT4,		64,	"REC_H2OD",	IREAL4,		"Water depth at receiver         "},
{"scalel",		INT2,		68,	"SUSCALE1",	IREAL4,		"SU header: elevation scaler     "},
{"scalco",	INT2,		70,	"SUSCALCO",	IREAL4,       	"SU header: coordinate scaler    "},
{"sx",		INT4,		72,	"SOU_X   ",	IREAL4,       	"Source X coordinate             "},
{"sy",		INT4,		76,	"SOU_Y   ",	IREAL4,       	"Source Y coordinate             "},
{"gx",		INT4,		80,	"REC_X   ",	IREAL4,       	"Receiver X coordinate           "},
{"gy",		INT4,		84,	"REC_Y   ",	IREAL4,		"Receiver Y coordinate           "},
{"counit",		INT2,		88,	"SUCOUNIT",	INT4,		"SU header: coordinate units     "},
{"wevel",		INT2,		90,	"REC_SLOC",	INT4,		"Receiver surface location number"},
{"swevel",	INT2,		92,	"FB_PICK ",	IREAL4,		"First break pick time           "},
{"sut",		INT2,		94,	"UPHOLE  ",	IREAL4,		"Source uphole time              "},
{"gut",		INT2,		96,	"AOFFSET ",	IREAL4,		"Absolute value of offset        "},
{"sstat",		INT2,		98,	"SOU_STAT",	IREAL4,		"Total static for source         "},
{"gstat",		INT2,		100,	"REC_STAT",	IREAL4,		"Total static for receiver       "},
{"tstat",		INT2,		102,	"TOT_STAT",	IREAL4,		"Total static for this trace     "},
{"laga",		INT2,		104,	"SOU_SLOC",	INT4,		"Nearest surface loc. to source  "},
{"lagb",		INT2,		106,	"NCHANS  ",	INT4,		"Number of channels of source    "},
{"delrt",		INT2,		108,	"REC_NFLD",	INT4,		"Receiver fold                   "},
{"muts",		INT2,		110,	"TLIVE_S ",	IREAL4,		"Start time of live samples      "},
{"unass[0]",	INT2,		210,	"TLIVE_E",	IREAL4,		"End time of live samples        "},
{"ns",		UINT2,		114,	"SU_NS   ",	INT4,		"SU header: number of samples    "},
{"dt",		UINT2,		116,	"SU_DT   ",	INT4,		"SU header: sample rate usec     "},
{"gain",		INT2,		118,	"NA_STAT ",	IREAL4,		"Portion of static not applied   "},
{"igc",		INT2,		120,	"SKEWSTAT",	IREAL4,		"Multiplex skew static           "},
{"igi",		INT2,		122,	"END_ENS ",	INT4,		"End-of-ensemble flag            "},
{"corr",		INT2,		124,	"EOJ     ",	INT4,		"End of job flag                 "},
{"sfs",		INT2,		126,	"LEN_SURG",	IREAL4,		"Length of surgical mute taper   "},
{"sfe",		INT2,		128,	"AMP_NORM",	IREAL4,		"Amplitude normalization factor  "},
{"slen",		INT2,		130,	"SR_AZIM ",	IREAL4,		"Source to receiver azimuth      "},
{"styp",		INT2,		132,	"GEO_COMP",	INT4,		"Geophone component (x,y,z)      "},
{"stas",		INT2,		134,	"NMO_STAT",	IREAL4,		"NMO datum static (don't apply)  "},
{"stae",		INT2,		136,	"FNL_STAT",	IREAL4,		"Static to move to final datum   "},
{"tatyp",		INT2,		138,	"SEQ_DISK",	INT4,		"Trace sequence number from disk "},
{"afilf",		INT2,		140,	"REPEAT  ",	INT4,		"REPEATED data copy number       "},
{"afils",		INT2,		142,	"CS_STAT ",	IREAL4,		"Corr. autostatics source static "},
{"nofilf",		INT2,		144,	"CR_STAT ",	IREAL4,		"Corr. autostatics recvr static  "},
{"nofils",		INT2,		146,	"PS_STAT ",	IREAL4,		"Power autostatics source static "},
{"lcf",		INT2,		148,	"PR_STAT ",	IREAL4,		"Power autostatics recvr static  "},
{"hcf",		INT2,		150,	"TRIMSTAT",	IREAL4,		"Trim static                     "},
{"lcs",		INT2,		152,	"LSEG_END",	INT4,		"Line segment end                "},
{"hcs",		INT2,		154,	"LSEG_SEQ",	INT4,		"Line segment sequence number    "},
{"year",		INT2,		156,	"DMOOFF  ",	IREAL4,		"Offset bin for DMO              "},
{"day",		INT2,		158,	"SU_DAY  ",	IREAL4,		"SU header: day data recorded      "},
{"hour",		INT2,		160,	"SU_HOUR ",	IREAL4,		"Receiver depth below surface    "},
{"minute",	INT2,		162,	"SOU_COMP",	INT4,		"Source component (x,y,z)        "},
{"sec",		INT2,		164,	"DS_SEQNO",	INT4,		"Input dataset sequence number   "},
{"timbas",	INT2,		166,	"OFB_CNTR",	IREAL4,		"Offset bin center               "},
{"trwf",		INT2,		168,	"OFB_NO  ",	INT4,		"Offset bin number               "},
{"grnors",	INT2,		170,	"LINE_NO ",	INT4,		"Line number (hashed line name)  "},
{"grnofr",		INT2,		172,	"FRN_TRNO",	INT4,		"Foreign trace-number-within line"},
{"grnlof",		INT2,		174,	"ILINE_NO",	INT4,		"3D inline number                "},
{"gaps",		INT2,		176,	"XLINE_NO",	INT4,		"3D crossline number             "},
{"otrav",		INT2,		178,	"DISKITER",	INT4,		"Disk Data Input iteration       "},
{"d1",		IREAL4,		180,	"SU_D1   ",	IREAL4,		"SU header: increment dim 1      "},
{"f1",		IREAL4,		184,	"SU_F1   ",	IREAL4,		"SU header: first value dim 1    "},
{"d2",		IREAL4,		188,	"SU_D2   ",	IREAL4,		"SU header: increment dim 2      "},
{"f2",		IREAL4,		192,	"SU_F2   ",	IREAL4,		"SU header: first value dim 2    "},	
{"ungpow",	IREAL4,		196,	"SU_UNGPO",	IREAL4,		"SU header: gpower               "},
{"unscale",	IREAL4,		200,	"SU_UNSCA",	IREAL4,		"SU header: unscale              "},
{"mark",		INT2,		204,	"SU_MARK ",	IREAL4,		"SU header: mark                 "},
{"ens_end",	INT2,		206,	"END_ENS ",	INT4,		"End-of-ensemble flag*           "},
{"mute",		INT2,		112,	"SU_MUTE ",	INT4,		"SU header: mute time            "},
{"    ",	       	-1,	        -1,	"        ",	-1,		"                                "},
};
