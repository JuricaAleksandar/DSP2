//////////////////////////////////////////////////////////////////////////////
// *
// * VEŽBA 2 – Banke filtara
// * Godina: 2016
// *
// * Zadatak: Banke filtara
// * Autor:
// *
// *
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "fir.h"
#include "filter_coeff.h"
#include "processing.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L
#define GAIN 0

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(InputBufferMono,4)
Int16 InputBufferMono[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferMono,4)
Int16 OutputBufferMono[AUDIO_IO_SIZE];

Int16 d0[AUDIO_IO_SIZE/2];
Int16 d1[AUDIO_IO_SIZE/2];
Int16 d2[AUDIO_IO_SIZE/4];
Int16 d3[AUDIO_IO_SIZE/4];
Int16 d4[AUDIO_IO_SIZE/4];
Int16 d5[AUDIO_IO_SIZE/4];
Int16 d6[AUDIO_IO_SIZE/2];
Int16 d7[AUDIO_IO_SIZE/2];

Int16 h0[N_COEFF];
Int16 h1[N_COEFF];
Int16 h2[N_COEFF];
Int16 h3[N_COEFF];
Int16 h4[N_COEFF];
Int16 h5[N_COEFF];
Int16 h6[N_COEFF];
Int16 h7[N_COEFF];
Int16 h8[N_COEFF];
Int16 h9[N_COEFF];
Int16 h10[N_COEFF];
Int16 h11[N_COEFF];

void stereoToMono(Int16* inputL, Int16* inputR, Int16* output, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		output[i] = (inputL[i]>>1) + (inputR[i]>>1);
	}
}

void main( void )
{
	int i;
	int state=0;
	int dugmeNovo=0;
	int dugmeStaro=0;
    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti dugmeta*/
    EZDSP5535_SAR_init();

	printf("\n DTMF Predajnik\n");

    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();

    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();

    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, GAIN);


 	while(1)
	{
 		/* Promeniti stanje na osnovu pritisnutog tastera */
 		/* Za dobavljanje vrednosti tastera koristiti funkciju EZDSP5535_SAR_getKey */
 		/* Your code here */

 		aic3204_read_block(InputBufferL, InputBufferR);
 		stereoToMono(InputBufferL, InputBufferR, InputBufferMono, AUDIO_IO_SIZE);

 		/* Izvrsiti analizu signala InputBufferMono upotrebom filter banke 4. reda */
 		/* Your code here */
 		decimate(InputBufferMono,d0,h0,H0,2,AUDIO_IO_SIZE);
 		decimate(d0,d2,h2,H0,2,AUDIO_IO_SIZE/2);
 		decimate(d0,d3,h3,H1,2,AUDIO_IO_SIZE/2);
 		decimate(InputBufferMono,d1,h1,H1,2,AUDIO_IO_SIZE);
 		decimate(d1,d4,h4,H0,2,AUDIO_IO_SIZE/2);
 		decimate(d1,d5,h6,H1,2,AUDIO_IO_SIZE/2);
 		dugmeNovo=EZDSP5535_SAR_getKey();
 		if (dugmeNovo!=dugmeStaro && dugmeNovo==SW1)
 			if (state==4)
 				state=0;
 			else
 				state++;
 		dugmeStaro=dugmeNovo;
 		switch(state)
 		{
 		case 0:
 			interpolate(d2,d6,h7,H0,2,AUDIO_IO_SIZE/4);
 			interpolate(d2,d7,h8,H1,2,AUDIO_IO_SIZE/4);
 			for(i=0;i<AUDIO_IO_SIZE/2;i++)
 				d8[i]=d7[i]+d6[i];
 			break;
 		}
 		/* U zavisnosti od trenutnog stanja nulirati odredjene komponente signala */
 		/* Stanje 0 - nema obrade (komponente signala na ulazu nalaze se i na izlazu) */
 		/* Stanje 1 - samo frekvencijske komponente iz prvog podopsega */
 		/* Stanje 2 - samo frekvencijske komponente iz drugog podopsega */
 		/* Stanje 3 - samo frekvencijske komponente iz treceg podopsega */
 		/* Stanje 4 - samo frekvencijske komponente iz cetvrtog podopsega */
 		/* Your code here */

 		/* Izvrsiti sintezu signala i rezultat smestiti u OutputBufferMono */
 		/* Your code here */
		aic3204_write_block(OutputBufferMono, OutputBufferMono);
	}


	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}


