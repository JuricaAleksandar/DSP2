#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "obrada.h"
#include "mdct.h"

#define B 12

double time_buffer[MDCT_SIZE];
double mdct_buffer[MDCT_SIZE/2];
double in_delay[MDCT_SIZE/2];
double out_delay[MDCT_SIZE/2];
extern double window[MDCT_SIZE];
int histogram[(1<<B)];

void obrada(double *in, double *out, int N)
{
  int i,j;
  
  memcpy(time_buffer,in_delay,N*sizeof(double));

  memcpy(time_buffer+N,in,N*sizeof(double));

  for(i=0;i<2*N;i++)
	  time_buffer[i]*=window[i];

  mdct(time_buffer,mdct_buffer);
  //Nelin. transformacija
  for(i=0;i<N;i++)
  {
	  if (mdct_buffer[i]>=0)
		  mdct_buffer[i]=sqrt(fabs(mdct_buffer[i]));
	  else
		  mdct_buffer[i]=-1.0*sqrt(fabs(mdct_buffer[i]));
  }
  //Kvantizacija
  for(i=0;i<N;i++)
  {
	  mdct_buffer[i]=floor(mdct_buffer[i]/(1<<(13-B))+0.5);
  }

  for(i=0;i<(1<<B);i++)
	  histogram[i]=0;

  for(i=0;i<N;i++)
	  histogram[(int)mdct_buffer[i]+(1<<(B-1))]++;

  //Inv. kvant.
  for(i=0;i<N;i++)
  {
	  mdct_buffer[i]=mdct_buffer[i]*(1<<(13-B));
  }

  //Inv. nelin. transformacija
  for(i=0;i<N;i++)
  {
	  if(mdct_buffer[i]>=0)
		  mdct_buffer[i]=mdct_buffer[i]*mdct_buffer[i];
	  else
		  mdct_buffer[i]=-mdct_buffer[i]*mdct_buffer[i];
  }

  imdct(mdct_buffer,time_buffer);

  for(i=0;i<2*N;i++)
	  time_buffer[i]*=window[i];

  for(i=0;i<N;i++)
	  out[i]=out_delay[i]+time_buffer[i];

  for(i=0;i<N;i++)
	  out_delay[i]=time_buffer[i+N];

  for(i=0;i<N;i++)
	  in_delay[i]=in[i];
}

void statistika()
{
	int i;
	for(i=0;i<(1<<B);i++)
		printf("%i\n",histogram[i]);
}
