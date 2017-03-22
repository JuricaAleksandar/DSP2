/*
 * processing.c
 *
 *  Created on: 29.02.2016.
 *      Author: Dejan Bokan (dejan.bokan@rt-rk.com)
 */

#include "processing.h"
#include "filter_coeff.h"
#include "fir.h"
#include "signal.h"

Int16 int_history[N_COEFF];
Int16 int_opt_history[N_COEFF];
Int16 history1[N_COEFF];
Int16 history2[N_COEFF];
Int16 int_pom[SIGNAL2_LENGTH*4];
Int16 int_opt_pom[SIGNAL2_LENGTH*4];
Int16 pom1[SIGNAL1_LENGTH];
Int16 pom2[SIGNAL1_LENGTH];

void downsample(Int16 *input, Int16 *output, int M, int N)
{
	int i;
	for(i=0;i<N/M;i++)
		output[i]=input[i*M];
}

void decimate(Int16 *input, Int16 *output, int M, int N)
{
	int i;
	for(i=0;i<N_COEFF;i++)
		history1[i]=0;
	for(i=0;i<N;i++)
		pom1[i]=fir_basic(input[i], lp_filter, history1, N_COEFF);
	downsample(pom1,output,M,N);
}

void decimate_opt(Int16 *input, Int16 *output, int M, int N)
{
	int i;
	for(i=0;i<N_COEFF;i++)
		history2[i]=0;
	for(i=0;i<N;i++)
		if(i%M)
			pom2[i]=fir_basic(input[i], lp_filter, history2, N_COEFF);
		else
			pom2[i]=fir_basic_opt_dec(input[i], lp_filter, history2, N_COEFF);
	downsample(pom2,output,M,N);
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

void interpolate(Int16 *input, Int16 *output, int L, int N)
{
	int i;
	upsample(input,int_pom,L,N);
	for(i=0;i<N_COEFF;i++)
		int_history[i]=0;
	for(i=0;i<N*L;i++)
		output[i]=fir_basic(int_pom[i], lp_filter, int_history, N_COEFF,i);
}

void opt_interpolate(Int16 *input, Int16 *output, int L, int N)
{
	int i;
	upsamle(input,int_opt_pom,L,N);
	for(i=0;i<N_COEFF;i++)
		int_opt_history[i]=0;
	for(i=0;i<N*L;i++)
		output[i]=fir_basic_opt_int(int_opt_pom[i], lp_filter, int_opt_history, N_COEFF);
}

void resample(Int16 *input, Int16 *output, int L, int M, int N)
{
	interpolate(input,pom,L,N);
	downsample(pom,output,M,N*L);
}
