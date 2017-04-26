#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "obrada.h"
#include "mdct.h"

double time_buffer[MDCT_SIZE];
double mdct_buffer[MDCT_SIZE/2];
double in_delay[MDCT_SIZE/2];
double out_delay[MDCT_SIZE/2];
extern double window[MDCT_SIZE];

void obrada(double *in, double *out, int N)
{
  int i;

  for (i=0; i<N; i++)
    out[i] = in[i];
}

void statistika()
{
}
