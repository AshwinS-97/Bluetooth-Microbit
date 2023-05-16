#ifndef ADC_H
#define ADC_H
#include <stdint.h>

#ifndef IOREG32
#define IOREG32(addr) (*(volatile unsigned long *) (addr))
#endif

/* Register offsets */
#define ADC_START           IOREG32(0x40007000)
#define ADC_SAMPLE          IOREG32(0x40007004)
#define ADC_STOP            IOREG32(0x40007008)
#define ADC_EVENTS_END      IOREG32(0x40007104) // sampling has filled up the result buffer
#define ADC_EVENTS_DONE     IOREG32(0x40007108) // conversion task completed
#define ADC_EVENTS_RESDONE  IOREG32(0x4000710C) // result ready for transfer to RAM
#define ADC_INTEN           IOREG32(0x40007300) // to enable/disable interupt 
#define ADC_INTENSET        IOREG32(0x40007304) // to enable interupt
#define ADC_STATUS          IOREG32(0x40007400) // indicates if busy or conversion done
#define ADC_ENABLE          IOREG32(0x40007500) // enable/disable ADC
#define ADC_PSELP(n)        IOREG32(0x40007510 + n * 0x10)
                                                // channel n pin: ain0 to ain7 (not port pins)
#define ADC_CONFIG(n)       IOREG32(0x40007518 + n * 0x10)
                                                // channel n configuration
#define ADC_RESOLUTION      IOREG32(0x400075F0) // bits per sample
#define     ADC_RES8BIT     0
#define     ADC_RES10BIT    1
#define     ADC_RES12BIT    2
#define     ADC_RES14BIT    3

#define ADC_SAMPLERATE      IOREG32(0x400075F8) // sampling rate
#define     ADC_SAMPLEMODE_LOCAL    (1 << 12)   // controlled by local timer
#define ADC_RESULT_PTR      IOREG32(0x4000762C) // pointer to write the result
#define ADC_RESULT_CNT      IOREG32(0x40007630) // number of samples to write
#define ADC_RESULT_AMT      IOREG32(0x40007634) // number of samples to write

#define CLK_16MHZ   16000000UL

#define MIC_SAMPLE_RATE 8000

/* Initialize ADC */
void adc_init(uint32_t pin);

/* Read one sample */
uint16_t adc_in(void);

/* Read a buffer of samples */
void adc_read(uint16_t buf[], uint32_t len);

#endif  /* ADC_H */
