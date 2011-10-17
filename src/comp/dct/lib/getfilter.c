/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */


/*********************** self documentation **********************/
/****************************************************************************
GETFILTER - GET wavelet FILTER type

waveGetfilter - Select the filter for wavelet transform
waveFreefilter - free the filter

****************************************************************************
Function Prototypes:

waveFilter *waveGetfilter(int id);
void waveFreefilter(waveFilter *filter);
****************************************************************************
waveGetfilter:
Input:
int id		filter to use
		0-9	for Daubechies' wavelet
		10-12 	for Coiflet
waveFreefilter:
Input:
waveFilter *filter	the wavelet filter
****************************************************************************
Author:		Tong Chen, 05/18/94
***************************************************************************/
/**************** end self doc ********************************/

#include "comp.h"
#ifndef NWAVETYPE
#define NWAVETYPE 15
#endif

static int 
	waveLen[NWAVETYPE]={2,4,6,8,10,12,14,16,18,20,6,12,18,24,30};

static int 
	waveIshift[NWAVETYPE]={0,0,0,0,0,0,0,0,0,0,-2,-4,-6, -8, -10};

static float 

	/* Daubechies' wavelets */
	waveTab1[2] = {.707106781,.707106781},

	waveTab2[4] = {0.4829629131445341, 0.8365163037378077,
			 0.2241438680420134, -0.1294095225512603},

	waveTab3[6] = {0.3326705529500825, 0.8068915093110924,
			0.4598775021184914, -0.1350110200102546,
			-0.0854412738820267, 0.0352262918857095},

	waveTab4[8] = {0.2303778133088964, 0.7148465705529154,
			0.6308807679298587, -0.0279837694168599,
			-0.1870348117190931, 0.0308413818355607,
			0.0328830116668852, -0.0105974017850690},

	waveTab5[10] = {0.1601023979741929, 0.6038292697971895,
			0.7243085284377726, 0.1384281459013203,
			-0.2422948870663823, -0.0322448695846381,
			0.0775714938400459, -0.0062414902127983,
			-0.0125807519990820, 0.0033357252854738},

	waveTab6[12] = {0.1115407433501095, 0.4946238903984533,
			0.7511339080210959, 0.3152503517091982,
			-0.2262646939654400, -0.1297668675672625,
			0.0975016055873225, 0.0275228655303053,
			-0.0315820393174862, 0.0005538422011614,
			0.0047772575109455, -0.0010773010853085},

	waveTab7[14] = {0.0778520540850037, 0.3965393194818912,
			0.7291320908461957, 0.4697822874051889,
			-0.1439060039285212, -0.2240361849938412,
			0.0713092192668272, 0.0806126091510774,
			-0.0380299369350104, -0.0165745416306655,
			0.0125509985560986, 0.0004295779729214,
			-0.0018016407040473, 0.0003537137999745},

	waveTab8[16] = {0.0544158422431072, 0.3128715909143166,
			0.6756307362973195, 0.5853546836542159,
			-0.0158291052563823, -0.2840155429615824,
			0.0004724845739124, 0.1287474266204893,
			-0.017369301001809, -0.0440882539307971,
			0.0139810279174001, 0.0087460940474065,
			-0.0048703529934520, -0.0003917403733770,
			0.0006754494064506, -0.0001174767841248},

	waveTab9[18] = {0.0380779473638778, 0.2438346746125858,
			0.6048231236900955, 0.6572880780512736,
			0.1331973858249883, -0.2932737832791663,
         		-0.0968407832229492, 0.1485407493381256,
			0.0307256814793385, -0.0676328290613279,
			0.0002509471148340, 0.0223616621236798,
			-0.0047232047577518, -0.0042815036824635,
			0.0018476468830563, 0.0002303857635232,
			-0.0002519631889427, 0.0000393473203163},

	waveTab10[20] = {0.0266700579005473, 0.1881768000776347,
			0.5272011889315757, 0.6884590394534363,
			0.2811723436605715, -0.2498464243271598,
			-0.1959462743772862, 0.1273693403357541,
			0.0930573646035547, -0.0713941471663501,
			-0.0294575368218399, 0.0332126740593612,
			0.0036065535669870, -0.0107331754833007,
			0.0013953517470688, 0.0019924052951925,
			-0.0006858566949564, -0.0001164668551285,
			0.0000935886703202, -0.0000132642028945},

	/* Coiflets */
	waveTab11[6] = {0.038580777747887, -0.12696912539621,
			-0.077161555495774, 0.60749164138568,
			0.74568755893443, 0.22658426519707},

	waveTab12[12] = { 0.016387336464, -0.041464936782, 
			-0.067372554722, 0.386110066823, 
			 0.812723635450, 0.416998396426, 
			-0.076488599079, -0.059434418647, 
			 0.023680171946, 0.005611434819, 
			-0.001823208871, -0.000720549445}, 

        waveTab13[18] = {-.0037935128643778759, .0077825964256797869,
			    .023452696142119103, -.065771911281431228,
			    -.061123390002955698, .40517690240961679,
			    .79377722262562034, .42848347637761869,
			    -.071799821619170590, -.082301927106320283,
			    .034555027573344464, .015880544863615901,
			    -.0090079761367322896, -.0025745176881279692,
			    .0011175187708269618, .00046621695982014403,
			    -.000070983302505704928, -.000034599773197402695},

        waveTab14[24] = {.00089231366822027571, -.001629492013110849,
			    -.0073461663276562349, .016068943964069236,
			    .026682300155628804, -.081266699680313054,
			    -.05607731331647195, .41530840703043015,
			    .78223893092049879, .43438605649146839,
			    -.066627474263000752, -.096220442033563697,
			    .039334427122913219, .025082261845146933,
			    -.015211731527239149, -.005658286685946038,
			    .0037514361569249027, .0012665619286795187,
			    -.00058902075681143784, -.00025997455231942175,
			    .000062339033865764618, .000031229876078043358,
			    -.0000032596804448576129, .0000017849845586999338},

        waveTab15[30] = {-.00021208086333630681, .0003585896772556986,
			    .0021782363048412847, -.0041593587816039935,
			    -.010131117538045594, .023408156761592795,
			    .028168029062141497, -.091920010548806413,
			    -.05204316321623739, .42156620672876544,
			    .77428960374028455, .43799162622836413,
			    -.062035963905608969, -.10557420870583534,
			    .041289208740734169, .032683574283249535,
			    -.019761779011723959, -.0091642311530462268,
			    .00676418541866332, .0024333732092240538,
			    -.0016628637690858134, -.00063813129615137752,
			    .00030225951979184068, .00014054114890107723,
			    -.000041340484491956856, -.000021315014062244917,
			    .0000037345967496715605, .0000020638063902331633,
			    -.00000016740829374930063, 
			    -.000000095157917046829356};


static float *waveTab[NWAVETYPE] 
	= {waveTab1, waveTab2, waveTab3, waveTab4, waveTab5, waveTab6,
	      waveTab7, waveTab8, waveTab9, waveTab10, 
	      waveTab11, waveTab12, waveTab13, waveTab14, waveTab15};


waveFilter *waveGetfilter(int id)
/***************************************************************************
Select the filter for wavelet transform
****************************************************************************
id		filter to use
		0-9	for Daubechies' wavelet
		10-12 	for Coiflet
****************************************************************************
Author:		Tong Chen, 05/18/94
***************************************************************************/
{
	waveFilter *filter;
	int len, i, j;

	if(id >= NWAVETYPE) return NULL;

	/* allocate space for the filter */
	filter = (waveFilter *)	malloc(sizeof(waveFilter));

	/* assign the parameters */
	filter->id = id;
	len = waveLen[id];
	filter->len = len;
	filter->ishift = waveIshift[id];
	
	/* allocate space for the filter coefficients */
	filter->filterh = alloc1float(len);
	filter->filterg = alloc1float(len);

	/* get the filter coefficients */
	for(i=0;i<len;i++) filter->filterh[i] = waveTab[id][i];
	for(i=0,j=len-1;i<len;i++,j--) filter->filterg[i] = waveTab[id][j];
	for(i=1;i<len;i+=2) filter->filterg[i] = -filter->filterg[i];

	return filter;
}

void waveFreefilter(waveFilter *filter)
{
	free1float(((waveFilter *)filter)->filterh);
	free1float(((waveFilter *)filter)->filterg);
	free(filter);
}


/* Test */
/*
main()
{
	int i,j;
	waveFilter *filter;

	for(i=0;i<10;i++){
		filter = waveGetfilter(i);
		fprintf(stderr,"length[%d]=%d\n",i,filter->len);
		for(j=0;j<filter->len;j++){
		    fprintf(stderr,"coeff[%d]=%f\n",j,filter->filterh[j]);
		    fprintf(stderr,"coeff[%d]=%f\n",j,filter->filterg[j]);
		}
		waveFreefilter(filter);
	}
}
*/
