/* Copyright (c) Colorado School of Mines, 2010.*/
/* All rights reserved.                       */

#include "cwp.h"

/*********************** self documentation **********************/

/**********************************************************************
SYMMEIGEN - Functions solving the eigenvalue problem for symmetric matrices
***********************************************************************
eig_jacobi - find eigenvalues and corresponding eigenvectors via 
         the jacobi algorithm for symmetric matrices
sort_eigenvalues -  sort eigenvalues and corresponding eigenvectors
			in descending order

***********************************************************************
Function Prototypes:
void eig_jacobi(float **a, float d[], float **v, int n);
void sort_eigenvalues(float d[], float **v, int n);
***********************************************************************
 (inspired by Press et. al., 1996)                     
**********************************************************************/

/* Macro used internally */
#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
    a[k][l]=h+s*(g-h*tau);

void eig_jacobi(float **a, float d[], float **v, int n)
/**********************************************************************
eig_jacobi - find eigenvalues and corresponding eigenvectors via 
         the jacobi algorithm for symmetric matrices

***********************************************************************
Function Prototype:
void eig_jacobi(float **a, float d[], float **v, int n);
***********************************************************************
 (inspired by Press et. al., 1996)                     
**********************************************************************/
{
    int j,iq,ip,i;
    float tresh,theta,tau,t,sm,s,h,g,c,*b,*z;

    /* allocate space temporarily */
    b=alloc1float(n); b-=1;
    z=alloc1float(n); z-=1;
    
    /* initialize v to the identity matrix */
    for (ip=1;ip<=n;ip++) {
        for (iq=1;iq<=n;iq++) v[ip][iq]=0.0;
        v[ip][ip]=1.0;
    }
    /* initialilize to the diagonal on matrix a */
    for (ip=1;ip<=n;ip++) {
        b[ip]=d[ip]=a[ip][ip];
        z[ip]=0.0;
    }

    /* main iteration loop */
    for (i=1;i<=50;i++) {

        sm=0.0;
        for (ip=1;ip<=n-1;ip++) {
            for (iq=ip+1;iq<=n;iq++)
                sm += fabs(a[ip][iq]);
        }
        /* normal return */
        if (sm == 0.0) {
            z+=1; free1float(z);
            b+=1; free1float(b);
            return;
        }
        /* tresh values for first 3 sweeps and therafter */
        if (i < 4)
            tresh=0.2*sm/(n*n);
        else
            tresh=0.0;
        for (ip=1;ip<=n-1;ip++) {
            for (iq=ip+1;iq<=n;iq++) {
                g=100.0*fabs(a[ip][iq]);
                if (i > 4 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip])
                    && (float)(fabs(d[iq])+g) == (float)fabs(d[iq]))
                    a[ip][iq]=0.0;
                else if (fabs(a[ip][iq]) > tresh) {
                    h=d[iq]-d[ip];
                    if ((float)(fabs(h)+g) == (float)fabs(h))
                        t=(a[ip][iq])/h;
                    else {
                        theta=0.5*h/(a[ip][iq]);
                        t=1.0/(fabs(theta)+sqrt(1.0+theta*theta));
                        if (theta < 0.0) t = -t;
                    }
                    c=1.0/sqrt(1+t*t);
                    s=t*c;
                    tau=s/(1.0+c);
                    h=t*a[ip][iq];
                    z[ip] -= h;
                    z[iq] += h;
                    d[ip] -= h;
                    d[iq] += h;
                    a[ip][iq]=0.0;
                    
                    /* Jacobi rotations */
                    for (j=1;j<=ip-1;j++) {
                        ROTATE(a,j,ip,j,iq)
                    }
                    for (j=ip+1;j<=iq-1;j++) {
                        ROTATE(a,ip,j,j,iq)
                    }
                    for (j=iq+1;j<=n;j++) {
                        ROTATE(a,ip,j,iq,j)
                    }
                    for (j=1;j<=n;j++) {
                        ROTATE(v,j,ip,j,iq)
                    }
                }
            }
        }
        for (ip=1;ip<=n;ip++) {
            b[ip] += z[ip];
            d[ip]=b[ip];
            z[ip]=0.0;
        }
    }
    /* this will not happen, hopefully */
    fprintf(stderr,"jacobi iteration does not converge\n");
}


void sort_eigenvalues(float d[], float **v, int n)
/***********************************************************************
sort_eigenvalues -  sort eigenvalues and corresponding eigenvectors
			in descending order

***********************************************************************
Function Prototypes:
void sort_eigenvalues(float d[], float **v, int n);
***********************************************************************
 (inspired by Press et. al., 1996)                     
**********************************************************************/
{
    int k,j,i;
    float p;

    for (i=1;i<n;i++) {
        p=d[k=i];
        for (j=i+1;j<=n;j++)
            if (d[j] >= p) p=d[k=j];
        if (k != i) {
            d[k]=d[i];
            d[i]=p;
            for (j=1;j<=n;j++) {
                p=v[j][i];
                v[j][i]=v[j][k];
                v[j][k]=p;
            }
        }
    }
}
