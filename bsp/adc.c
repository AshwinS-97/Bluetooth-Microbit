#include <stdint.h>
#include <string.h>
#include "adc.h"
#include "../rtx/cmsis_os2.h"
/*
 * We will use an ADC to read inputs from the microphone.
 */


/* We are using only a subset of registers relevant to us to keep things
 * simple. Hence all registers are not defined below.
 */



void adc_init(uint32_t ain)
{
    uint16_t cc;    // compare counter for sampling

    /* Enable ADC */
    ADC_ENABLE = 1;

    /*
     * Connect channel 0 to the specified analog input pin ain.
     * (value 0 for disconnected and ain + 1 to connect to ain pin)
     */
    ADC_PSELP(0) = ain + 1;

    /* Configure parameters */
    ADC_CONFIG(0) = ((7 << 8) | (2 << 16));   // gain = 4, tacq = 10us
    cc = CLK_16MHZ / MIC_SAMPLE_RATE;
    ADC_SAMPLERATE = ADC_SAMPLEMODE_LOCAL | cc;

    /* Set ADC parameters */
    ADC_RESOLUTION = ADC_RES14BIT;
}

/* Read one sample from ADC */
uint16_t adc_in(void)
{
    volatile uint16_t sample[1];    // changed by DMA

    adc_read((uint16_t *) sample, 1);

    return sample[0];
}

/* Read a buffer of samples from ADC */
void adc_read(uint16_t buf[], uint32_t len)
{
#if 0   // for testing: corrupt the buffer to ensure that
        // the values were really written by the ADC.
    int i;

    for (i = 0; i < len; i++)
        buf[len] = 0xabcd;
#endif

    /* Set up result buffer for DMA */
    ADC_RESULT_PTR = (uint32_t) buf;
    ADC_RESULT_CNT = len;

    /* Start the ADC and start sampling the first input. */
    ADC_START = 1;
    ADC_SAMPLE = 1;

    /* Wait until all the samples are written into memory. */
    // while (ADC_EVENTS_END == 0)
    //     ;
    osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);
    //ADC_EVENTS_END = 0;     // clear the event

#if 0   // testing
    printf("%d number of samples\n", ADC_RESULT_AMT);
    for (i = 0; i < len; i++)
        printf("%x\n", buf[i]);
#endif

    return;
}
