#include <math.h>
#include <stdlib.h>
#include "obrada.h"
#include "sr_fft.h"
#include "string.h"

#define PI 3.1415926535

double time_buffer[FFT_SIZE];
double fft_buffer[FFT_SIZE];
double in_delay[FFT_SIZE/2];
double out_delay[FFT_SIZE/2];

extern double window[FFT_SIZE];

void obrada(double *in, double *out, int N)
{
  int i;

  memcpy(time_buffer,in_delay,N*sizeof(double));

  memcpy(time_buffer+N,in,N*sizeof(double));

  for(i=0;i<2*N;i++)
	  time_buffer[i]*=window[i];

  fft(time_buffer,fft_buffer,FFT_ORDER);

  int k1=FFT_SIZE*300/44100;
  int k2=FFT_SIZE*4000/44100;

  for(i=0;i<2*N;i++)
	  if(i<2*k1 || i>2*k2)
		  fft_buffer[i]=0;

  for(i=0;i<N;i++)
  {
	  fft_buffer[2*i]=abs(fft_buffer[i])*cos(PI/4*(i^2));
	  fft_buffer[2*i+1]=abs(fft_buffer[i])*sin(PI/4*(i^2));
  }

  ifft(fft_buffer,time_buffer,FFT_ORDER);

  for(i=0;i<2*N;i++)
	  time_buffer[i]*=window[i];

  for(i=0;i<N;i++)
	  out[i]=out_delay[i]+time_buffer[i];

  for(i=0;i<N;i++)
	  out_delay[i]=time_buffer[i+N];

  for(i=0;i<N;i++)
	  in_delay[i]=in[i];
}
