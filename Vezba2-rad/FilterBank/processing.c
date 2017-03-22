/*
 * processing.c
 *
 *  Created on: 29.02.2016.
 *      Author: Dejan Bokan (dejan.bokan@rt-rk.com)
 */

#include "processing.h"
#include "filter_coeff.h"
#include "fir.h"
#include "ezdsp5535_aic3204_dma.h"

Int16 pom[AUDIO_IO_SIZE];

void downsample(Int16 *input, Int16 *output, int M, int N)
{
	int i;
	for(i=0;i<N/M;i++)
		output[i]=input[i*M];
}

void decimate(Int16 *input, Int16 *output,Int16 *history,Int16 * filter, int M, int N)
{
	int i;
	for(i=0;i<N_COEFF;i++)
		history[i]=0;
	for(i=0;i<N;i++)
		pom[i]=fir_basic(input[i], filter, history, N_COEFF);
	downsample(pom,output,M,N);
}

void upsample(Int16 *input, Int16 *output, int L, int N)
{
	int i;
	for(i=0;i<N*L;i++)
		output[i]=0;
	output[0]=input[0];
	for(i=1;i<N;i++)
		output[i*L]=input[i];
}

void interpolate(Int16 *input, Int16 *output,Int16 *history,Int16 * filter, int L, int N)
{
	int i;
	upsample(input,pom,L,N);
	for(i=0;i<N_COEFF;i++)
		history[i]=0;
	for(i=0;i<N*L;i++)
		output[i]=fir_basic(pom[i], filter, history, N_COEFF);
}
